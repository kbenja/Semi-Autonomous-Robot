var display_manual = true;

function change_mode() {
    if (display_manual) {
        $(".manual").css("display", "none");
        $(".auto").css("display", "block");
    } else {
        $(".manual").css("display", "block");
        $(".auto").css("display", "none");
    }
    display_manual = !display_manual;
}


$(document).ready(function() {
    // init canvas and display loading text
    var canvas = document.getElementById('canvas-video');
    var ctx = canvas.getContext('2d');
    ctx.fillStyle = '#333';
    ctx.fillText('Loading...', canvas.width / 2 - 30, canvas.height / 3);

    getFile("../ip.txt", function(response) {
        // initialize websocket
        var wsUrl = response;
        var client = new WebSocket(wsUrl);
        var player = new jsmpeg(client, { canvas: canvas });

        // initialize controller
        var controller = new Controller(client);
        controller.initialize();

        function cycle(elapsed) {
            controller.readGamePad();
        }

        // initialize 60fps loop
        loopManager.run(cycle)

        // tracking colors:
        var colors = new tracking.ColorTracker(['magenta', 'cyan', 'yellow']);

        colors.on('track', function(event) {
            if (event.data.length === 0) {
                console.log("did not find any colors!");
            } else {
                event.data.forEach(function(rect) {
                    console.log(rect.x, rect.y, rect.height, rect.width, rect.color);
                });
            }
        });

        tracking.track('#canvas-video', colors);
    });
})
