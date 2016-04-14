angular.module('Data', ['CommunicationService']).controller('DataCtrl', function($scope, communication) {
    $scope.data = 123456;
    $scope.update = function() {
        $scope.data += 1;
    }
    $scope.orientation = 45;
    $scope.read_data = function() {
        communication.read_data(function(evt) {
            $scope.$apply(function() {
                var object = JSON.parse(evt.data);
                if(object.data) {
                    console.log("Hex", object.data);
                    $scope.orientation = parseInt(object.data[2] + object.data[3] + object.data[0] + object.data[1], 16);
                }
                // console.log("received", evt.data);
            });
        });
    }
    $scope.read_data();
});
