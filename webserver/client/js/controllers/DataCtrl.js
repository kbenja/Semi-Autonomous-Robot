angular.module('Data', ['CommunicationService']).controller('DataCtrl', function($scope, communication) {
    $scope.data = 123456;
    $scope.update = function() {
        $scope.data += 1;
    }
    $scope.message = "...";
    $scope.read_data = function() {
        communication.read_data(function(evt) {
            $scope.$apply(function() {
                var hex = JSON.parse(evt.data);
                console.log(hex);
                $scope.message = parseInt(hex.count[2] + hex.count[3] + hex.count[0] + hex.count[1], 16);
            });
        });
    }
    $scope.read_data();
});
