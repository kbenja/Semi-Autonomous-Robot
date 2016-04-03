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

        // computer vision
        var rects;
        var tracker = new tracking.ColorTracker(['yellow']);
        tracker.on('track', function(event) {
            rects = document.querySelector('.rect');
            if (rects) {
                $('.rect').remove(); // jquery way
                // javascript way change to this if we use angular
                // Array.prototype.forEach.call(rects, function(node) {
                //     node.parentNode.removeChild(node);
                // });
            }
            if (event.data.length !== 0) {
                event.data.forEach(function(rect) {
                    var bounding_box = document.createElement('div');
                    document.querySelector('#canvas-container').appendChild(bounding_box);
                    bounding_box.classList.add('rect');
                    bounding_box.style["border"] = '2px solid ' + rect.color;
                    bounding_box.style["position"] = 'absolute';
                    bounding_box.style["width"] = rect.width + 'px';
                    bounding_box.style["height"] = rect.height + 'px';
                    bounding_box.style["left"] = rect.x + 'px';
                    bounding_box.style["top"] = rect.y + 'px';
                });
            }
        });
        setInterval(function() {
            tracking.track('#canvas-video', tracker);
        }, 100);
    });
})
