angular.module('Gamepad', ['CommunicationService']).controller("GamepadCtrl", function ($scope, $rootScope, communication) {
    // $scope.status = "Please connect or reconnect the controller";
    $scope.keyboard_controls = false;
    $scope.breaking = false;
    $scope.gamepad_connected = false;
    $scope.gamepad = {};
    $scope.command = {
        mode: 0,
        code: 0
    };

    var keysDown = {};
    var last_state = {};
    var modes = {
        "IDLE": 0,
        "MANUAL": 1,
        "AUTO": 2,
        "INTAKE": 3,
        "TESTING": 4
    }

    function send_command(data) {
        communication.send_data({mode: modes[$rootScope.mode], code: data});
    }

    function keyPressed(key) {
        if (key === "b_button" || key === 32 && !$scope.breaking) {
            $scope.breaking = true;
            $scope.command.code = 0;
            send_command($scope.command.code);
        }
        if(!$scope.breaking) {
            // put $scope only on game pad connection
            if (key === "up" || key === 38) {
                if(!keysDown[key]) {
                    $scope.command.code = 1;
                }
                keysDown[key] = true;
            }
            if (key === "left" || key === 37) {
                if(!keysDown[key]) {
                    $scope.command.code = 2;
                }
                keysDown[key] = true;
            }
            if (key === "down" || key === 40) {
                if(!keysDown[key]) {
                    $scope.command.code = 3;
                }
                keysDown[key] = true;
            }
            if (key === "right" || key === 39) {
                if(!keysDown[key]) {
                    $scope.command.code = 4;
                }
                keysDown[key] = true;
            }
            if (key === "r2_button") {
                if(!keysDown[key]) {
                    $scope.command.code = 5;
                }
                keysDown[key] = true;
            }
            if (key === "l2_button") {
                if(!keysDown[key]) {
                    $scope.command.code = 6;
                }
                keysDown[key] = true;
            }
            send_command($scope.command.code);
        }
    }

    function keyReleased(key) {
        if (key === "b_button" || key === 32) { // Player releases break
            keysDown[key] = false;
            $scope.breaking = false;
        }
        if (!$scope.breaking) {
            if (key === "up" || key === 38) {
                keysDown[key] = false;
                $scope.command.code = 0;
            }
            if (key === "left" || key === 37) {
                keysDown[key] = false;
                $scope.command.code = 0;
            }
            if (key === "down" || key === 40) {
                keysDown[key] = false;
                $scope.command.code = 0;
            }
            if (key === "right" || key === 39) {
                keysDown[key] = false;
                $scope.command.code = 0;
            }
            if (key === "l2_button") {
                keysDown[key] = false;
                $scope.command.code = 0;
            }
            if (key === "r2_button") {
                keysDown[key] = false;
                $scope.command.code = 0;
            }
            send_command($scope.command.code);
        }
    }

    function initialize() {
        if($scope.keyboard_controls) {
            addEventListener("keydown", function(e) {
                keyPressed(e.keyCode, e);
            }, false);

            addEventListener("keyup", function(e) {
                keyReleased(e.keyCode, e);
            }, false);
        }

        addEventListener("gamepadconnected", function(e) {
            $scope.gamepad_connected = true;
            $scope.gamepad = navigator.getGamepads()[0];
            $scope.command.mode = 1;
            send_command($scope.command.code);
            console.log("Gamepad connected at index %d: %s. %d buttons, %d axes.", $scope.gamepad.index, $scope.gamepad.id, $scope.gamepad.buttons.length, $scope.gamepad.axes.length);
        }, false);

        addEventListener("gamepaddisconnected", function(e) {
            $scope.gamepad_connected = false;
            $scope.gamepad = {};
            console.log("Gamepad disconnected");
            $scope.command.mode = 0;
            send_command($scope.command.code);
        }, false);
    }

    function readGamePad() {
        if($scope.gamepad_connected) {
            $scope.gamepad = navigator.getGamepads()[0];
            $scope.gamepad.buttons.forEach(function(number, it){
                var button = getButton(it);
                if(check_mode(button)) {
                    if(number.pressed) {
                        if(last_state[it] === false) {
                            console.log(it);
                            keyPressed(button);
                        }
                        last_state[it] = true;
                        $("." + button).css("background-color","#EEEEEE");
                    } else {

                        if(last_state[it]) {
                            keyReleased(button);
                        }
                        last_state[it] = false;
                        $("." + button).css("background-color","#AAAAAA");
                    }
                }
            })
            // control joystick GUI
            $(".left_joystick").css("left",$scope.gamepad.axes[0]*15);
            $(".left_joystick").css("top",$scope.gamepad.axes[1]*15);
            $(".right_joystick").css("left",$scope.gamepad.axes[2]*15);
            $(".right_joystick").css("top",$scope.gamepad.axes[3]*15);
            // $scope.left_left = $scope.gamepad.axes[0]*15;
            // $scope.left_top = $scope.gamepad.axes[1]*15;
            // $scope.right_left = $scope.gamepad.axes[2]*15;
            // $scope.right_top = $scope.gamepad.axes[3]*15;
        }
    }

    function check_mode(btn) {
        if($rootScope.mode === "MANUAL") {
            return registeredButton(btn);
        } else if ($rootScope.mode === "INTAKE") {
            return registeredIntakeButton(btn);
        } else {
            return false;
        }
    }
    function registeredButton(btn) {
        return (btn === "up" || btn === "left" || btn === "right" || btn === "down" || btn==="b_button" || btn==="l2_button" || btn==="r2_button") ? true : false;
    }

    function registeredIntakeButton(btn) {
        return (btn === "up" || btn === "down") ? true : false;
    }
    function getButton(btn) {
        switch(btn) {
            case 0:
                return("a_button");
                break;
            case 1:
                return("b_button");
                break;
            case 2:
                return("x_button");
                break;
            case 3:
                return("y_button");
                break;
            case 4:
                return("l1_button");
                break;
            case 5:
                return("r1_button");
                break;
            case 6:
                return("l2_button");
                break;
            case 7:
                return("r2_button");
                break;
            case 8:
                return("back_button");
                break;
            case 9:
                return("start_button");
                break;
            case 10:
                return("left_analog");
                break;
            case 11:
                return("right_analog");
                break;
            case 12:
                return("up");
                break;
            case 13:
                return("down");
                break;
            case 14:
                return("left");
                break;
            case 15:
                return("right");
                break;
            case 16:
                return("x_button");
                break;
            default:
                console.log(btn);
                return("");
                break;
        }
    }

    function cycle(elapsed) {
        readGamePad();
    }

    // initialize 60fps loop
    initialize()
    loopManager.run(cycle)
});
