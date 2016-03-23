$(document).ready(function(){
    // init canvas and display loading text
    var canvas = document.getElementById('canvas-video');
    var ctx = canvas.getContext('2d');
    ctx.fillStyle = '#333';
    ctx.fillText('Loading...', canvas.width/2-30, canvas.height/3);

    var command = {
        mode: "1",
        code: "0"
    }

    var breaking = false;
    var gamepad_connected = false;
    var keysDown = {};
    var gamepad = {};
    var last_state = {};

    function setLastState(it,bool) {
        last_state[it] = bool;
        return last_state;
    }

    function replaceBit(string, index, value) {
        return string.substr(0,index-1) + value + string.substr(index,string.length);
    }

    function convert_command(object) {
        console.log("command = ", object);
        var final_string = "";
        final_string += object.mode + "\n";
        final_string += object.code + "\n";
        return final_string;
    }

    getIP(function(response){
        var wsUrl = response;
        var client = new WebSocket(wsUrl);
        var player = new jsmpeg(client, { canvas:canvas });

        addEventListener("keydown", function(e) {
            keyPressed(e.keyCode, e);
        }, false);

        addEventListener("keyup", function(e) {
            keyReleased(e.keyCode, e);
        }, false);

        addEventListener("gamepadconnected", function(e) {
            gamepad_connected = true;
            gamepad = navigator.getGamepads()[0];
            console.log("Gamepad connected at index %d: %s. %d buttons, %d axes.", gamepad.index, gamepad.id, gamepad.buttons.length, gamepad.axes.length);
            $(".controls").css("opacity","1");
            $(".connect").css("display","none");
        }, false);

        addEventListener("gamepaddisconnected", function(e) {
            gamepad_connected = false;
            gamepad = {};
            console.log("Gamepad disconnected");
            command.code = 0;
            client.send(convert_command(command));
            $(".controls").css("opacity","0.2");
            $(".connect").css("display","block");
        }, false);


        function keyPressed(key) {
            console.log(key);
            if (key === "b_button" || key === 32 && !breaking) {
                breaking = true;
                command.code = 0;
                client.send(convert_command(command));
            }
            if(true) {
                if (key === "up" || key === 38) {
                    if(!keysDown[key]) {
                        command.code = 1;
                    }
                    keysDown[key] = true;
                }
                if (key === "left" || key === 37) {
                    if(!keysDown[key]) {
                        command.code = 2;
                    }
                    keysDown[key] = true;
                }
                if (key === "down" || key === 40) {
                    if(!keysDown[key]) {
                        command.code = 3;
                    }
                    keysDown[key] = true;
                }
                if (key === "right" || key === 39) {

                    if(!keysDown[key]) {
                        command.code = 4;
                    }
                    keysDown[key] = true;
                }
                client.send(convert_command(command));
            }
        }

        function keyReleased(key) {
            if (key === "b_button" || key === 32) { // Player releases break
                keysDown[key] = false;
                breaking = false;
            }
            if (!breaking) {
                if (key === "up" || key === 38) {
                    keysDown[key] = false;
                    command.code = 0;
                }
                if (key === "left" || key === 37) {
                    keysDown[key] = false;
                    command.code = 0;
                }
                if (key === "down" || key === 40) {
                    keysDown[key] = false;
                    command.code = 0;
                }
                if (key === "right" || key === 39) {
                    keysDown[key] = false;
                    command.code = 0;
                }
                client.send(convert_command(command));
            }
        }

        function readGamePad() {
            if(gamepad_connected) {
                gamepad = navigator.getGamepads()[0];
                gamepad.buttons.forEach(function(number, it){
                    var button = getButton(it);
                    if(number.pressed) {
                        if(last_state[it] === false && registeredButton(button)) {
                            keyPressed(button);
                        }
                        setLastState(it,true);
                        $("." + button).css("background-color","#CCCCCC");
                    } else {
                        if(last_state[it] === true) {
                            console.log(number,it);
                            keyReleased(button);
                        }
                        setLastState(it,false);
                        $("." + button).css("background-color","#AAAAAA");
                    }
                })
                // control joystick GUI
                $(".left_joystick").css("left",gamepad.axes[0]*15);
                $(".left_joystick").css("top",gamepad.axes[1]*15);
                $(".right_joystick").css("left",gamepad.axes[2]*15);
                $(".right_joystick").css("top",gamepad.axes[3]*15);

            }
        }

        //60fps cycle
        function cycle(elapsed) {
            readGamePad();
        }
        loopManager.run(cycle)
    });
})

function getIP(callback) {
  var path = "../ip.txt";
  var xobj = new XMLHttpRequest();
  xobj.open('GET', path, true);
  xobj.onreadystatechange = function () {
    if (xobj.readyState == 4 && xobj.status == "200") {
      callback(xobj.responseText);
    }
  };
  xobj.send(null);
}
