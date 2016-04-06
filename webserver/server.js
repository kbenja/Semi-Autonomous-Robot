var development = false;

var express = require('express');
var app = express();
var http = require('http');
var childProcess = require('child_process');
var ws = require('ws');

var createIP = require('./createIP');
var ipc = require('node-ipc');

var static_port = 8080;
var comm_port   = 8088;
var stream_port = 8082;
var socket_port = 8084;

if(!development) {
    createIP();
}

/*
 *  CLIENT & SERVER COMMUNICATION -  TCP/IP SOCKET
 */
var tcp_socket = false;
var to_send = false;
var comm_socket = new(ws.Server)({port: comm_port});
console.log('SOCKET client - server listening on PORT ' + comm_port);
comm_socket.on('connection', function(socket) {
    tcp_socket = socket; // set global socket object
    console.log('New WebSocket Connection (' + comm_socket.clients.length + ' total)');
    var counter = 0;
    socket.on("message", function(command) {
        command = JSON.parse(command);
        commands.push([command.mode, command.code]);
        comm_socket.broadcast(JSON.stringify({count: to_send}));
        console.log(JSON.stringify({count: counter}));
    })
    socket.on('close', function(code, message) {
        tcp_socket = false;
        console.log('Disconnected WebSocket (' + comm_socket.clients.length + ' total)');
    });
    setInterval(function(){
        comm_socket.broadcast(JSON.stringify({count: to_send}));
    }, 1000);
});

comm_socket.broadcast = function(data) {
    for (var i in this.clients) {
        if (this.clients[i].readyState == 1) {
            this.clients[i].send(data);
            console.log("data being sent: ", data);
        } else {
            console.log('Error: Client (' + i + ') not connected.');
        }
    }
};

/*
*   SERVER & C++ PROGRAM COMMUNICATION - UNIX SOCKET
*/
var unix_socket = false; // set global unix socket

var commands = [];
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
        if(!tcp_socket) {
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
        to_send = data.toString('hex').split('');
        console.log(data.toString('hex').split(''));
        heartbeat++;
        unix_socket_emit()
    });
});
ipc.server.start();

/*
*    VIDEO STREAMING
*/

var STREAM_MAGIC_BYTES = 'jsmp'; // Must be 4 bytes
var width = 640;
var height = 480;

// Create socket for video stream
var wsServer = new(ws.Server)({ port: socket_port });
console.log('SOCKET communication for video stream PORT ' + socket_port);

wsServer.on('connection', function(socket) {
    console.log('VIDEO STREAM connection OPENED (' + wsServer.clients.length + ' total)');

    socket.on('close', function(code, message) {
        console.log('VIDEO STREAM connection CLOSED(' + wsServer.clients.length + ' total)');
    });

    var streamHeader = new Buffer(8);
    streamHeader.write(STREAM_MAGIC_BYTES);
    streamHeader.writeUInt16BE(width, 4);
    streamHeader.writeUInt16BE(height, 6);
    socket.send(streamHeader, { binary: true });
});
// Broadcast video to clients
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
    console.log('SOCKET listening for video stream on PORT ' + stream_port);
    start_video();

});


/*
 *  STATIC FILES SERVER
 */
app.set('port', static_port);
app.use(express.static("./client"));
http.createServer(app).listen(app.get('port'), function() {
    console.log('HTTP server listening on PORT ' + app.get('port'));
});

var video_stream;
function start_video() {
    if(!development) {
        video_stream = childProcess.exec('./bin/do_ffmpeg.sh');
        video_stream.on('exit', function (code) {
            setTimeout(function() {
                console.log('VIDEO STREAM EXITED, RESTARTING');
                start_video();
            }, 500);
        });
    }
}

