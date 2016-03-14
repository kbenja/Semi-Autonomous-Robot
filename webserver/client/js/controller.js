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

