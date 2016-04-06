angular.module('Video', ['CommunicationService']).controller('VideoCtrl', function($scope) {
    // computer vision
    $scope.rects = [];
    tracking.ColorTracker.registerColor('red', function(r, g, b) {
      if (r > 100 && g < 75 && b < 75) {
        return true;
      }
      return false;
    });
    var tracker = new tracking.ColorTracker(['yellow','red']);
    tracker.on('track', function(event) {
        $scope.rects = event.data;
        $scope.rects.forEach(function(rect) {

            // console.log(rect);
        });
    });
    setInterval(function() {
        tracking.track('#canvas-video', tracker);
        $scope.$apply();
    }, 100);
});
