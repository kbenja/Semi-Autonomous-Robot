angular.module('Data', ['CommunicationService']).controller('DataCtrl', function($scope, communication){
    $scope.data = 123456;
    $scope.update = function() {
        $scope.data += 1;

    }
});
