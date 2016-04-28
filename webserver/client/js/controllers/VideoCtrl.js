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
    var h = 480;
    var w = 640;
    var goal_x = 320 - 13;
    var goal_y = 288 - 7;
    var stop_distance_x = 10;
    var stop_distance_y = 10;

    tracker.on('track', function(event) {
        $scope.rects = event.data;
        var markers = [];
        $scope.rects.forEach(function(rect, num) {
            // console.log("RECT: " + num + " CENTER: (" + rect.x , rect.y + rect.height/2 + ") HEIGHT =", rect.height, "WIDTH =", rect.width);
            if (rect.height > rect.width * 2 && rect.y > 150) {
                markers.push(rect);
            }
            if (num == $scope.rects.length - 1) {
                if (markers.length > 1) {
                    if(markers[0].y + 50 > markers[1].y && markers[0].y - 50 < markers[1].y) {
                        error_count = 0;
                        var breaker_center = (markers[0].x + markers[0].width/2 + markers[1].x + markers[1].width/2)/2;
                        var breaker_height = (markers[0].y + markers[1].y)/2;
                        var breaker_status_color = (breaker_center < goal_x + stop_distance_x && breaker_center > goal_x - stop_distance_x ? "rgba(0,255,0,0.8)" : "rgba(255,0,0,0.5)");
                        var breaker_height_status = (breaker_height < goal_y + stop_distance_y && breaker_height > goal_y - stop_distance_y ? "rgba(0,255,0,0.8)" : "rgba(255,0,0,0.5)");
                        // target X
                        // current X
                        $scope.rects.push({
                            "width": 2,
                            "height": h,
                            "x": breaker_center - 1,
                            "y": 0,
                            "color": breaker_status_color
                        });
                        // current Y
                        $scope.rects.push({
                            "width": 640,
                            "height": 2,
                            "x": 0,
                            "y": breaker_height,
                            "color":breaker_height_status
                        })
                        if ($rootScope.mode === "AUTO") {
                            $scope.rects.push({
                                "width": 4,
                                "height": 100,
                                "x": goal_x - 2,
                                "y": goal_y - 50,
                                "color": "rgba(0,255,0,0.8)"
                            });
                            // target Y
                            $scope.rects.push({
                                "width": 100,
                                "height": 4,
                                "x": goal_x - 50,
                                "y": goal_y - 2,
                                "color": "rgba(0,255,0,0.8)"
                            });
                        }
                        if ($rootScope.mode === "AUTO") {
                            $scope.instruction = get_direction(breaker_center, breaker_height);
                            if($scope.instruction !== last_instruction) {
                                // console.log("SENDING", $scope.instruction);
                                communication.send_data({mode: 2, code: $scope.instruction});
                            }
                        }
                    }
                } else {
                    // canot detect breaker
                    if ($rootScope.mode === "AUTO") {
                        error_count++;
                        // console.log(error_count);
                        if (error_count > 90) {
                            if($scope.instruction !== last_instruction) {
                                console.log("SENDING STOP SIGNAL");
                                communication.send_data({mode: 2, code: -1}); // send stop instruction
                            }
                        }
                    }
                }
                last_instruction = $scope.instruction;
            }
        });
    });

    function get_direction(current_x, current_y) {
        var temp_x;
        var temp_y;
        var difference_x = current_x  - goal_x;
        var difference_y = current_y  - goal_y;
        // check for X
        if(difference_x < stop_distance_x && difference_x > -stop_distance_x) {
            console.log("X ALIGNED");
            temp_x = "00";
        } else {
            if (difference_x > 0) {
                // need to move right
                temp_x = "01";
            } else if (difference_x < 0) {
                // need to move left
                temp_x = "10";
            }
        }
        // check for Y
        if(difference_y < stop_distance_y && difference_y > -stop_distance_y) {
            console.log("Y ALIGNED");
            temp_y = "00";
        } else {
            if (difference_y < 0) {
                // need to move forward
                temp_y = "01";
            } else if (difference_y > 0) {
                // need to move backward
                temp_y = "10";
            }
        }
        console.log("final = temp_x", temp_x,"+ temp_y", temp_y);
        console.log(parseInt(temp_y + temp_x, 2));
        /*
        0 = stop X, stop Y
        1 = right X, stop Y
        2 = left X, stop Y
        4 = stop X, forward Y
        5 = right X, forward Y
        6 = left X, forward Y
        8 = stop X, backward Y
        9 = right X, backward Y
        10 = left X, backward Y
        */
        return parseInt(temp_y + temp_x, 2);
    }
    setInterval(function() {
        last_instruction = -4;
    }, 1000);

    setInterval(function() {
        tracking.track('#canvas-video', tracker);
        $scope.$apply();
    }, 17);
});
