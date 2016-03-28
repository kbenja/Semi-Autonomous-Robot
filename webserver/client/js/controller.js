var Controller = function(client) {
    this.keyboard_controls = false;
    this.breaking = false;
    this.gamepad_connected = false;
    this.keysDown = {};
    this.gamepad = {};
    this.last_state = {};
    this.command = {
        mode: 0,
        code: 0
    };
    this.client = client;
}

Controller.prototype.keyPressed = function(key) {
    console.log(key);
    if (key === "b_button" || key === 32 && !this.breaking) {
        this.breaking = true;
        this.command.code = 0;
        this.client.send(JSON.stringify(this.command));
    }
    if(!this.breaking) {
        // put this only on game pad connection
        if (key === "up" || key === 38) {
            if(!this.keysDown[key]) {
                this.command.code = 1;
            }
            this.keysDown[key] = true;
        }
        if (key === "left" || key === 37) {
            if(!this.keysDown[key]) {
                this.command.code = 2;
            }
            this.keysDown[key] = true;
        }
        if (key === "down" || key === 40) {
            if(!this.keysDown[key]) {
                this.command.code = 3;
            }
            this.keysDown[key] = true;
        }
        if (key === "right" || key === 39) {
            if(!this.keysDown[key]) {
                this.command.code = 4;
            }
            this.keysDown[key] = true;
        }
        this.client.send(JSON.stringify(this.command));
        console.log(this.command);
    }
}

Controller.prototype.keyReleased = function(key) {
    if (key === "b_button" || key === 32) { // Player releases break
        this.keysDown[key] = false;
        this.breaking = false;
    }
    if (!this.breaking) {
        if (key === "up" || key === 38) {
            this.keysDown[key] = false;
            this.command.code = 0;
        }
        if (key === "left" || key === 37) {
            this.keysDown[key] = false;
            this.command.code = 0;
        }
        if (key === "down" || key === 40) {
            this.keysDown[key] = false;
            this.command.code = 0;
        }
        if (key === "right" || key === 39) {
            this.keysDown[key] = false;
            this.command.code = 0;
        }
        this.client.send(JSON.stringify(this.command));
    }
}

Controller.prototype.initialize = function() {
    var that = this;
    if(this.keyboard_controls) {
        addEventListener("keydown", function(e) {
            that.keyPressed(e.keyCode, e);
        }, false);

        addEventListener("keyup", function(e) {
            that.keyReleased(e.keyCode, e);
        }, false);
    }

    addEventListener("gamepadconnected", function(e) {
        that.gamepad_connected = true;
        that.gamepad = navigator.getGamepads()[0];
        that.command.mode = 1;
        that.client.send(JSON.stringify(that.command));
        console.log("Gamepad connected at index %d: %s. %d buttons, %d axes.", that.gamepad.index, that.gamepad.id, that.gamepad.buttons.length, that.gamepad.axes.length);
        $(".controls").css("opacity","1");
        $(".connect").css("display","none");
    }, false);

    addEventListener("gamepaddisconnected", function(e) {
        that.gamepad_connected = false;
        that.gamepad = {};
        console.log("Gamepad disconnected");
        that.command.mode = 0;
        that.client.send(JSON.stringify(that.command));
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

Controller.prototype.readGamePad = function() {
    if(this.gamepad_connected) {
        var that = this;
        this.gamepad = navigator.getGamepads()[0];
        this.gamepad.buttons.forEach(function(number, it){
            var button = getButton(it);
            if(number.pressed) {
                if(that.last_state[it] === false && registeredButton(button)) {
                    that.keyPressed(button);
                }
                that.last_state[it] = true;
                $("." + button).css("background-color","#CCCCCC");
            } else {
                if(that.last_state[it]) {
                    console.log(number,it);
                    that.keyReleased(button);
                }
                that.last_state[it] = false;
                $("." + button).css("background-color","#AAAAAA");
            }
        })
        // control joystick GUI
        $(".left_joystick").css("left",this.gamepad.axes[0]*15);
        $(".left_joystick").css("top",this.gamepad.axes[1]*15);
        $(".right_joystick").css("left",this.gamepad.axes[2]*15);
        $(".right_joystick").css("top",this.gamepad.axes[3]*15);
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

