angular.module('Video', ['CommunicationService']).controller('VideoCtrl', function($scope) {
    // computer vision
    $scope.rects = [];
    var tracker = new tracking.ColorTracker(['yellow']);
    tracker.on('track', function(event) {
        // $scope.rects = document.querySelector('.rect');
        // if (rects) {
        //     // $('.rect').remove(); // jquery way
        //     // javascript way change to this if we use angular
        //     // Array.prototype.forEach.call(rects, function(node) {
        //     //     node.parentNode.removeChild(node);
        //     // });
        // }
        $scope.rects = event.data;
        $scope.rects.forEach(function(rect) {

            console.log(rect);
            // rect.classList.add('rect');
            // rect.style["border"] = '2px solid ' + rect.color;
            // rect.style["position"] = 'absolute';
            // rect.style["width"] = rect.width + 'px';
            // rect.style["height"] = rect.height + 'px';
            // rect.style["left"] = rect.x + 'px';
            // rect.style["top"] = rect.y + 'px';
        });
    });
    setInterval(function() {
        tracking.track('#canvas-video', tracker);
        $scope.$apply();
    }, 100);
});
