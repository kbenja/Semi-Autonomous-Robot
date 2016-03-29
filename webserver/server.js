var express = require("express");
var app = express();
var http = require('http');
var childProcess = require('child_process');
var ws = require('ws');

var createIP = require('./createIP');
var ipc = require('node-ipc');

var static_port = 8080;
var stream_port = 8082;
var socket_port = 8084;

var development = false;
if(!development) {
    createIP();
}

/// VIDEO STREAMING
var STREAM_MAGIC_BYTES = 'jsmp'; // Must be 4 bytes
var width = 320;
var height = 240;

// WEBSOCKET SERVER
var wsServer = new(ws.Server)({ port: socket_port });
console.log('WebSocket server listening on port ' + socket_port);

/*
*    UNIX SOCKET – SERVER & C++ PROGRAM
*/
var commands = [];
var client_socket = false;
var unix_socket = false; // set global unix socket
var mode = -1;
var heartbeat = 0;
var last_heartbeat = 0;
var check_status = setInterval(function(){
    if(!development) {
        if(last_heartbeat === heartbeat) {
            unix_socket = false;
            console.log("UNIX SOCKET NOT CONNECTED");
        }
        heartbeat = heartbeat%1000;
        last_heartbeat = heartbeat;
    }
}, 500);

function unix_socket_emit() {
    if(unix_socket) {
        if(!client_socket) {
            ipc.server.emit(unix_socket,[-1,0]);
        } else if(commands.length) {
            console.log("sending: ",commands[0]);
            ipc.server.emit(unix_socket,commands[0]);
            commands.splice(0,1);
        } else {
            // idle mode or same as last command
            ipc.server.emit(unix_socket,[0,0]);
        }
    }
}
ipc.config.appspace = "breakerbot.";
ipc.config.id = 'socket';
ipc.config.retry = 1500;
ipc.config.rawBuffer = true;
ipc.config.encoding = 'hex';
ipc.config.silent = true;
ipc.serve(function() {
    ipc.server.on('connect', function(socket){
        unix_socket = socket;
        commands = [];
        console.log("UNIX SOCKET CONNECTED");
    });
    ipc.server.on('data', function(data,socket){
        heartbeat++;
        unix_socket_emit()
    });
});
ipc.server.start();

/*
*    TCP/IP SOCKET – CLIENT & SERVER
*/
wsServer.on('connection', function(socket) {
    console.log('New WebSocket Connection (' + wsServer.clients.length + ' total)');
    client_socket = true;
    socket.on("message", function(command) {
        command = JSON.parse(command);
        commands.push([command.mode, command.code]);
    })
    socket.on('close', function(code, message) {
        console.log('Disconnected WebSocket (' + wsServer.clients.length + ' total)');
        client_socket = false;
    });

    var streamHeader = new Buffer(8);
    streamHeader.write(STREAM_MAGIC_BYTES);
    streamHeader.writeUInt16BE(width, 4);
    streamHeader.writeUInt16BE(height, 6);
    socket.send(streamHeader, { binary: true });
});

// HTTP STATIC SERVER
app.set('port', static_port);
app.use(express.static("./client"));
http.createServer(app).listen(app.get('port'), function() {
    console.log('HTTP server listening on port ' + app.get('port'));
});

// HTTP SERVER FOR MPEG1 STREAM
wsServer.broadcast = function(data, opts) {
    for (var i in this.clients) {
        if (this.clients[i].readyState == 1) {
            this.clients[i].send(data, opts);
        } else {
            console.log('Error: Client (' + i + ') not connected.');
        }
    }
};

http.createServer(function(req, res) {
    console.log('CLIENT VIDEO STREAM CONNECTED: ' + req.socket.remoteAddress + ':' + req.socket.remotePort + ' size: ' + width + 'x' + height);
    req.on('data', function(data) {
        wsServer.broadcast(data, { binary: true });
    });
}).listen(stream_port, function() {
    console.log('Listening for video stream on port ' + stream_port);
    var video_stream = childProcess.exec('./bin/do_ffmpeg.sh');
    video_stream.on('exit', function (code) {
        console.log('VIDEO STREAM EXITED, RESTARTING');
        video_stream = childProcess.exec('./bin/do_ffmpeg.sh');
    });
});

