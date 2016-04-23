angular.module('CommunicationService', []).factory('communication', [function($http, $rootScope) {
    // create socket for server-client communication

    var comm_socket = new WebSocket("ws://168.122.5.160:8088/");
    // var comm_socket = new WebSocket("ws://localhost:8088/");

    // create socket for video stream

    var stream_socket = new WebSocket("ws://168.122.5.160:8084/");
    // var stream_socket = new WebSocket("ws://localhost:8084/");

    // canvas for video stream
    var canvas = document.getElementById('canvas-video');
    var ctx = canvas.getContext('2d');
    ctx.fillText('Loading...', canvas.width / 2 - 30, canvas.height / 3);

    var player = new jsmpeg(stream_socket, { canvas: canvas });

    var test = "hello world";

    return {
        comm_socket: comm_socket,
        send_data: function(data) {
            comm_socket.send(JSON.stringify(data));
        },
        read_data: function(callback) {
            comm_socket.onmessage = function(evt) {
                callback(evt);
            };
        }
    }

    // helpful example : https://gist.github.com/ae6rt/7865161
}]);
