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

    // var client = new WebSocket("ws://155.41.119.205:8084/");
    var socket = new WebSocket("ws://localhost:8084/");
    function read_socket() {

    }
    var test = "hello world";
    return {
        socket: socket
    }
}]);
