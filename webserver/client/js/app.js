var app = angular.module('Breakerbot', ['Gamepad',
    'Data',
    'Video',
    'CommunicationService',
    'angularModalService'
]);

app.controller('OpenModalCtrl', function($scope, ModalService, $rootScope) {
    $rootScope.mode = 'MANUAL';
    $scope.show = function() {
        ModalService.showModal({
            templateUrl: 'modal.html',
            controller: "ModalController"
        }).then(function(modal) {
            modal.element.modal();
            modal.close.then(function(result) {
                $scope.message = "You said " + result;
            });
        });
    };
});

app.controller('ModalController', function($scope, $rootScope, close) {
    $scope.close = function(result) {
        close(result, 500); // close, but give 500ms for bootstrap to animate
    };
    $scope.change_mode = function(mode) {
        $rootScope.mode = mode;
        console.log("Toggling mode", $rootScope.mode);
    }

});
