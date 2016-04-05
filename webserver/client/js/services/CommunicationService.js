angular.module('CommunicationService', []).factory('communication', [function($http, $rootScope) {
    // create socket for server-client communication

    var comm_socket = new WebSocket("ws://155.41.119.205:8088/");
    // var comm_socket = new WebSocket("ws://localhost:8088/");

    // create socket for video stream

    var stream_socket = new WebSocket("ws://155.41.119.205:8084/");
    // var stream_socket = new WebSocket("ws://localhost:8084/");

    // canvas for video stream
    var canvas = document.getElementById('canvas-video');
    var ctx = canvas.getContext('2d');
    ctx.fillText('Loading...', canvas.width / 2 - 30, canvas.height / 3);

    var player = new jsmpeg(stream_socket, { canvas: canvas });
    var read_socket = function() {

    }
    var test = "hello world";

    return {
        comm_socket: comm_socket,
        read_data: function(callback) {
            comm_socket.onmessage = function(evt) {
                console.log("evt!", evt);
                callback(evt);
            };
        }
    }


    // helpful example : https://gist.github.com/ae6rt/7865161
}]);
