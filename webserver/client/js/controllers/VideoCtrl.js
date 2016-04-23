angular.module('Video', ['CommunicationService']).controller('VideoCtrl', function($scope) {
    // computer vision
    $scope.rects = [];
    tracking.ColorTracker.registerColor('red', function(r, g, b) {
      if (r > 100 && g < 75 && b < 75) {
        return true;
      }
      return false;
    });
    var tracker = new tracking.ColorTracker(['yellow']);
    tracker.on('track', function(event) {
        $scope.rects = event.data;
        var count = 0;
        $scope.rects.forEach(function(rect, num) {
            count++;
            if (rect.height > rect.width * 2.5) {
                console.log("RECT: " + num + " CENTER: (" + rect.x , rect.y + rect.height/2 + ") HEIGHT =", rect.height, "WIDTH =", rect.width);
            }
            if (num == $scope.rects.length - 1) {
                console.log("NUMBER OF RECTANGLES:", count);
            }
        });

    });
    setInterval(function() {
        tracking.track('#canvas-video', tracker);
        $scope.$apply();
    }, 100);
});
