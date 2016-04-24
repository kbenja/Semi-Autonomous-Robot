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
        var markers = [];
        $scope.rects.forEach(function(rect, num) {
            count++;
            if (rect.height > rect.width * 2 && rect.y > 200) {
                console.log("RECT: " + num + " CENTER: (" + rect.x , rect.y + rect.height/2 + ") HEIGHT =", rect.height, "WIDTH =", rect.width);
                markers.push(rect);
                // console.log("RECT:", rect.x + rect.width/2);
            }
            if (num == $scope.rects.length - 1) {
                // console.log("NUMBER OF RECTANGLES:", count);
                if (markers.length > 1) {
                    if(markers[0].y + 20 > markers[1].y && markers[0].y - 20 < markers[1].y) {
                        var breaker_center = (markers[0].x + markers[0].width/2 + markers[1].x + markers[1].width/2)/2;
                        var breaker_status_color = (breaker_center < 322 && breaker_center > 318 ? "yellow" : "red");
                        $scope.rects.push({
                            "width": 2,
                            "height": 480,
                            "x": 319,
                            "y": 0,
                            "color":"green"
                        });
                        $scope.rects.push({
                            "width": 4,
                            "height": 480,
                            "x": breaker_center - 2,
                            "y": 0,
                            "color": breaker_status_color
                        });
                    }
                }
            }
        });

    });
    setInterval(function() {
        tracking.track('#canvas-video', tracker);
        $scope.$apply();
    }, 100);
});
