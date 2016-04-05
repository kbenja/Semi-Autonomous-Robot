angular.module('Data', ['CommunicationService']).controller('DataCtrl', function($scope, communication) {
    $scope.data = 123456;
    $scope.update = function() {
        $scope.data += 1;
    }
    $scope.message = "...";
    $scope.read_data = function() {
        communication.read_data(function(evt) {
            $scope.$apply(function() {
                $scope.message = evt.data;
            });
        });
    }
    $scope.read_data();
});
