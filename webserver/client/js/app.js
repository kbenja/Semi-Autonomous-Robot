var app = angular.module ('Breakerbot',
    ['Gamepad',
     'Data',
     'Video',
     'CommunicationService'
    ]);

app.controller('DevCtrl', function($scope, $rootScope){
    $rootScope.manual_mode = true;
    $scope.status = $rootScope.manual_mode;
    $scope.toggle_mode = function() {
        $rootScope.manual_mode = ($rootScope.manual_mode ? false : true);
        $scope.status = $rootScope.manual_mode;
        console.log("Toggling mode", $rootScope.manual_mode);
    }
});
