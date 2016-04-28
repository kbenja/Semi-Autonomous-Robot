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
                    $rootScope.current_status = $scope.get_current_status(object.data);
                    // console.log($rootScope.current_status);
                    $scope.last_communication = 0;
                    $rootScope.connected = true;
                } else {
                    $rootScope.connected = false;
                }
            });
        });
    }
    $scope.get_current_status = function(data){
        if ($rootScope.mode === "AUTO") {
            var indicator = parseInt(data[6] + data[7] + data[4] + data[5], 16);
            if (indicator === 1) {
                return "ALIGN-Z";
            } else if (indicator === 2) {
                return "ALIGN-X";
            } else if (indicator === 3) {
                return "ALIGN-Y";
            } else if (indicator === 4) {
                return "ALIGNED";
            } else {
                return "WAITING";
            }
        }
    }
    $scope.read_data();
});
