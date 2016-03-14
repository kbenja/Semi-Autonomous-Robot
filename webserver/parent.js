var fs = require('fs');
var cp = require('child_process');
var childout = cp.spawn('child.cpp', {}, function(error, stdout, stderr) {
    console.log("stdout :: " + stdout);
    console.log("stderror :: " + stderr);
    console.log("error :: " + error);
});
