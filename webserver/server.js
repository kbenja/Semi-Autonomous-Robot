/*
*
*   SET THIS FLAG TO TRUE IF RUNNING THIS ON EDISON
*
*/
var development = true;

var express = require("express");
var app = express();
var path = require("path");
var fs = require('fs');
var http = require('http');
var childProcess = require('child_process');
var morgan = require('morgan');
var ws = require('ws');
// configuration files
var configServer = require('./lib/config/server');

var ip_command = 'ipconfig getifaddr en1';
if(!development) {
    ip_command = 'configure_edison --showWiFiIP';
}

var getIP = childProcess.exec(ip_command);
getIP.stdout.on('data', function (data) {
    var ip = 'ws://'+data.toString().slice(0,-1)+':8084/';
    fs.writeFile("./client/ip.txt", ip, function(err) {
        if(err) {
            return console.log(err);
        }
        console.log(ip, "saved to file");
    });
});
getIP.stderr.on('err', function(err) {
    console.log("Error finding IP!");
})

// app parameters
app.set('port', configServer.httpPort);
app.use(express.static(configServer.staticFolder));
app.use(morgan('dev'));

// serve index
require('./lib/routes').serveIndex(app, configServer.staticFolder);

// HTTP server
http.createServer(app).listen(app.get('port'), function() {
    console.log('HTTP server listening on port ' + app.get('port'));
});

/// Video streaming section
// Reference: https://github.com/phoboslab/jsmpeg/blob/master/stream-server.js

var STREAM_MAGIC_BYTES = 'jsmp'; // Must be 4 bytes
var width = 320;
var height = 240;

// WebSocket server
var wsServer = new(ws.Server)({
    port: configServer.wsPort
});

console.log('WebSocket server listening on port ' + configServer.wsPort);

// var commands = [];
// var process_executing = false;
// function execute_next_item_in_queue () {
//     if (!process_executing && commands.length){
//         process_executing = true;
//         childProcess.exec('echo ' + commands[0] + ' > /dev/ttymcu0', function() {
//             commands.splice(0,1);
//             process_executing = false;
//             execute_next_item_in_queue();
//         });
//     }
// };

wsServer.on('connection', function(socket) {
    socket.on("message", function(code) {
        if(!development){
            console.log(code);
            // commands.push(code);
            // execute_next_item_in_queue(code);
            // childProcess.exec('echo ' + commands[0] + ' > /dev/ttymcu0');
        }
    })
    var streamHeader = new Buffer(8);

    streamHeader.write(STREAM_MAGIC_BYTES);
    streamHeader.writeUInt16BE(width, 4);
    streamHeader.writeUInt16BE(height, 6);
    socket.send(streamHeader, {
        binary: true
    });

    console.log('New WebSocket Connection (' + wsServer.clients.length + ' total)');

    socket.on('close', function(code, message) {
        console.log('Disconnected WebSocket (' + wsServer.clients.length + ' total)');
        if(!development){
            childProcess.exec('echo "0000" > /dev/ttymcu0');
        }
    });
});

wsServer.broadcast = function(data, opts) {
    for (var i in this.clients) {
        if (this.clients[i].readyState == 1) {
            this.clients[i].send(data, opts);
        } else {
            console.log('Error: Client (' + i + ') not connected.');
        }
    }
};

// HTTP server to accept incoming MPEG1 stream
http.createServer(function(req, res) {
    console.log(
        'Stream Connected: ' + req.socket.remoteAddress +
        ':' + req.socket.remotePort + ' size: ' + width + 'x' + height
    );
    req.on('data', function(data) {
        wsServer.broadcast(data, {
            binary: true
        });
    });
}).listen(configServer.streamPort, function() {
    console.log('Listening for video stream on port ' + configServer.streamPort);

    if(!development) {
        childProcess.exec('./bin/init)pins.sh');
    }
    // Run do_ffmpeg.sh from node
    childProcess.exec('./bin/do_ffmpeg.sh');
});

module.exports.app = app;
