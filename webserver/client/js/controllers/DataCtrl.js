angular.module('Data', ['CommunicationService']).controller('DataCtrl', function($scope, $rootScope, communication) {

    function transform_data(starting_word) {
        console.log("WIP read bytes properly");
    }

    $rootScope.connected = false;
    $scope.orientation = "?";
    $scope.last_communication = 0;

    $scope.last_charge= "?";

    $scope.update_battery = function() {
        start_charge();
        console.log("hello", $scope.last_charge);
    }
    function start_charge() {
        $scope.last_charge = 0;
        setInterval(function() {
            $scope.last_charge += 1;
        }, 1000*60);
    }

    setInterval(function() {
        $scope.last_communication += 1;
    }, 1000);
    $scope.read_data = function() {
        communication.read_data(function(evt) {
            $scope.$apply(function() {
                var object = JSON.parse(evt.data);
                if(object.data) {
                    // console.log("Hex", object.data);
                    $scope.orientation = parseInt(object.data[2] + object.data[3] + object.data[0] + object.data[1], 16);
                    $rootScope.status = parseInt(object.data[6] + object.data[7] + object.data[4] + object.data[5], 16);
                    console.log($rootScope.status);
                    $scope.last_communication = 0;
                    $rootScope.connected = true;
                } else {
                    $rootScope.connected = false;
                }
            });
        });
    }
    $scope.read_data();
});
