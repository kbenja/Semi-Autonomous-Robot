angular.module('CommunicationService', []).factory('communication', [function($http, $rootScope) {
    // var socket = function() {
    //     return $http({
    //         method: 'GET',
    //         url: '../../ip.txt'
    //     }).then(function(response) {
    //         return "hello";
    //     }, function(error) {
    //         return "error";
    //     });
    // }


    // create socket for server-client communication
    // var socket = new WebSocket("ws://155.41.119.205:8088/");
    var comm_socket = new WebSocket("ws://localhost:8088/");

    // create socket for video stream
    // var socket = new WebSocket("ws://155.41.119.205:8084/");
    var stream_socket = new WebSocket("ws://localhost:8084/");

    // canvas for video stream
    var canvas = document.getElementById('canvas-video');
    var ctx = canvas.getContext('2d');
    ctx.fillText('Loading...', canvas.width / 2 - 30, canvas.height / 3);

    var player = new jsmpeg(stream_socket, { canvas: canvas });
    var read_socket = function() {

    }
    var test = "hello world";
    return {
        comm_socket: comm_socket
    }
}]);
