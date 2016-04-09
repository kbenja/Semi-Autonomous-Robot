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
            controller: "ModeCtrl"
        }).then(function(modal) {
            modal.element.modal();
            modal.close.then(function(result) {
                $scope.message = "You said " + result;
            });
        });
    };
});

app.controller('ModeCtrl', function($scope, $rootScope, close, communication) {
    $scope.close = function(result) {
        close(result, 500); // close, but give 500ms for bootstrap to animate
    };
    $scope.change_mode = function(mode) {
        $rootScope.mode = mode;
        console.log("Toggling mode", $rootScope.mode);
    }
});

app.controller('InputCtrl', function($scope, $rootScope, communication) {
    $scope.testing_status = function() {
        console.log($rootScope.mode);
        if($rootScope.mode === "TESTING") {
            communication.send_data({mode: 4, code: 4});
            console.log("sending", {mode: 4, code: 4});
        }
    }
    $scope.auto_status = function() {
        console.log($rootScope.mode);
        if($rootScope.mode === "AUTO") {
            communication.send_data({mode: 2, code: 2});
            console.log("sending", {mode: 2, code: 2});
        }
    }
    $scope.intake_status = function() {
        console.log($rootScope.mode);
        if($rootScope.mode === "INTAKE") {
            communication.send_data({mode: 3, code: 3});
            console.log("sending", {mode: 3, code: 3});
        }
    }
});

