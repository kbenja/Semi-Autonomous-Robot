angular.module('Video', ['CommunicationService']).controller('VideoCtrl', function($scope, $rootScope, communication) {
    // computer vision
    $scope.rects = [];
    tracking.ColorTracker.registerColor('red', function(r, g, b) {
      if (r > 100 && g < 75 && b < 75) {
        return true;
      }
      return false;
    });
    var tracker = new tracking.ColorTracker(['yellow']);
    var last_instruction = -1;
    var error_count = 0;
    tracker.on('track', function(event) {
        $scope.rects = event.data;
        var count = 0;
        var markers = [];
        $scope.rects.forEach(function(rect, num) {
            count++;
            if (rect.height > rect.width * 2 && rect.y > 200) {
                // console.log("RECT: " + num + " CENTER: (" + rect.x , rect.y + rect.height/2 + ") HEIGHT =", rect.height, "WIDTH =", rect.width);
                markers.push(rect);
                // console.log("RECT:", rect.x + rect.width/2);
            }
            if (num == $scope.rects.length - 1) {
                // console.log("NUMBER OF RECTANGLES:", count);
                if (markers.length > 1) {
                    if(markers[0].y + 20 > markers[1].y && markers[0].y - 20 < markers[1].y) {
                        var breaker_center = (markers[0].x + markers[0].width/2 + markers[1].x + markers[1].width/2)/2;
                        var breaker_height = (markers[0].y + markers[1].y)/2;
                        var breaker_status_color = (breaker_center < 322 && breaker_center > 318 ? "yellow" : "red");
                        var breaker_height_status = (breaker_height < 290 && breaker_height > 286 ? "yellow" : "red");
                        console.log(breaker_height);
                        $scope.rects.push({
                            "width": 2,
                            "height": 480,
                            "x": 319,
                            "y": 0,
                            "color":"green"
                        });
                        $scope.rects.push({
                            "width": 680,
                            "height": 2,
                            "x": 0,
                            "y": breaker_height,
                            "color":breaker_height_status
                        })
                        $scope.rects.push({
                            "width": 4,
                            "height": 480,
                            "x": breaker_center - 2,
                            "y": 0,
                            "color": breaker_status_color
                        });
                        if ($rootScope.mode === "AUTO") {
                            $scope.instruction = get_direction(breaker_center);
                            if($scope.instruction !== last_instruction) {
                                console.log("SENDING", $scope.instruction);
                                communication.send_data({mode: 1, code: $scope.instruction});
                            }
                        }
                    }
                } else {
                    // canot detect breaker
                    if ($rootScope.mode === "AUTO") {
                        $scope.instruction = get_direction(-1);
                        console.log(error_count);
                        if ($scope.instruction === -2) {
                            console.log("SENDING STOP SIGNAL");
                            communication.send_data({mode: 1, code: 0}); // send stop instruction
                        }
                    }
                }
                last_instruction = $scope.instruction;
            }
        });
    });

    function get_direction(current_center) {
        if (current_center === -1) {
            error_count++;
            return (error_count > 15 ? -2 : -1);
        } else {
            error_count = 0;
            var absolute_center = 320;
            var difference = current_center - absolute_center;
            var slow_down_distance = 150;
            // breaker is to the left, needs to move right
            if(difference < 8 && difference > -8) {
                console.log("STOP – ALIGNED WITH BREAKER");
                $scope.instruction = 0;
                return 0;
            } else {
                if (difference > 0) {
                    if (difference < slow_down_distance) {
                        console.log("MOVE RIGHT, SPEED = SLOW");
                        return 1;
                    } else {
                        console.log("MOVE RIGHT, SPEED = FAST");
                        return 2;
                    }
                } else if (difference < 0) {
                    if (difference > -slow_down_distance) {
                        console.log("MOVE LEFT, SPEED = SLOW");
                        return 3;
                    } else {
                        console.log("MOVE LEFT, SPEED = FAST");
                        return 4;
                    }
                }
            }
        }
    }

    setInterval(function() {
        tracking.track('#canvas-video', tracker);
        $scope.$apply();
    }, 17);
});
