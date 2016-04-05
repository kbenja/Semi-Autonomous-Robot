angular.module('Gamepad', ['CommunicationService']).controller("GamepadCtrl", function ($scope, communication) {
    // $scope.status = "Please connect or reconnect the controller";
    $scope.keyboard_controls = false;
    $scope.breaking = false;
    $scope.gamepad_connected = false;
    $scope.keysDown = {};
    $scope.gamepad = {};
    $scope.last_state = {};
    $scope.command = {
        mode: 0,
        code: 0
    };
    $scope.socket = communication.comm_socket;

    function keyPressed(key) {
        if (key === "b_button" || key === 32 && !$scope.breaking) {
            $scope.breaking = true;
            $scope.command.code = 0;
            $scope.socket.send(JSON.stringify($scope.command));
        }
        if(!$scope.breaking) {
            // put $scope only on game pad connection
            if (key === "up" || key === 38) {
                if(!$scope.keysDown[key]) {
                    $scope.command.code = 1;
                }
                $scope.keysDown[key] = true;
            }
            if (key === "left" || key === 37) {
                if(!$scope.keysDown[key]) {
                    $scope.command.code = 2;
                }
                $scope.keysDown[key] = true;
            }
            if (key === "down" || key === 40) {
                if(!$scope.keysDown[key]) {
                    $scope.command.code = 3;
                }
                $scope.keysDown[key] = true;
            }
            if (key === "right" || key === 39) {
                if(!$scope.keysDown[key]) {
                    $scope.command.code = 4;
                }
                $scope.keysDown[key] = true;
            }
            $scope.socket.send(JSON.stringify($scope.command));
            console.log($scope.command);
        }
    }

    function keyReleased(key) {
        if (key === "b_button" || key === 32) { // Player releases break
            $scope.keysDown[key] = false;
            $scope.breaking = false;
        }
        if (!$scope.breaking) {
            if (key === "up" || key === 38) {
                $scope.keysDown[key] = false;
                $scope.command.code = 0;
            }
            if (key === "left" || key === 37) {
                $scope.keysDown[key] = false;
                $scope.command.code = 0;
            }
            if (key === "down" || key === 40) {
                $scope.keysDown[key] = false;
                $scope.command.code = 0;
            }
            if (key === "right" || key === 39) {
                $scope.keysDown[key] = false;
                $scope.command.code = 0;
            }
            $scope.socket.send(JSON.stringify($scope.command));
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
            $scope.socket.send(JSON.stringify($scope.command));
            console.log("Gamepad connected at index %d: %s. %d buttons, %d axes.", $scope.gamepad.index, $scope.gamepad.id, $scope.gamepad.buttons.length, $scope.gamepad.axes.length);
            $(".controls").css("opacity","1");
            $(".connect").css("display","none");
        }, false);

        addEventListener("gamepaddisconnected", function(e) {
            $scope.gamepad_connected = false;
            $scope.gamepad = {};
            console.log("Gamepad disconnected");
            $scope.command.mode = 0;
            $scope.socket.send(JSON.stringify($scope.command));
            $(".controls").css("opacity","0.2");
            $(".connect").css("display","block");
        }, false);
    }

    function convert_command(object) {
        var final = " ";
        final += object.mode + " ";
        final += object.code + " ";
        return final;
    }

    function readGamePad() {
        if($scope.gamepad_connected) {
            $scope.gamepad = navigator.getGamepads()[0];
            $scope.gamepad.buttons.forEach(function(number, it){
                var button = getButton(it);
                if(number.pressed) {
                    if($scope.last_state[it] === false && registeredButton(button)) {
                        keyPressed(button);
                    }
                    $scope.last_state[it] = true;
                    $("." + button).css("background-color","#CCCCCC");
                } else {
                    if($scope.last_state[it]) {
                        console.log(number,it);
                        keyReleased(button);
                    }
                    $scope.last_state[it] = false;
                    $("." + button).css("background-color","#AAAAAA");
                }
            })
            // control joystick GUI
            $(".left_joystick").css("left",$scope.gamepad.axes[0]*15);
            $(".left_joystick").css("top",$scope.gamepad.axes[1]*15);
            $(".right_joystick").css("left",$scope.gamepad.axes[2]*15);
            $(".right_joystick").css("top",$scope.gamepad.axes[3]*15);
        }
    }

    function registeredButton(btn) {
        return (btn === "up" || btn === "left" || btn === "right" || btn === "down" || btn==="b_button") ? true : false;
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
