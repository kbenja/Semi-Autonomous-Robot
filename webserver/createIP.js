var childProcess = require('child_process');
var fs = require('fs');

var ip_command = 'configure_edison --showWiFiIP';

function createIP() {
    var getIP = childProcess.exec(ip_command);
    getIP.stdout.on('data', function (data) {
        var ip = 'ws://'+data.toString().slice(0,-1)+':8084/';
        fs.writeFile("./client/ip.txt", ip, function(err) {
            if(!err) {
                console.log(ip, "saved to file");
            }
        });
    });
    getIP.stderr.on('err', function(err) {
        console.log("Error finding IP!");
    })
}

module.exports = createIP;
