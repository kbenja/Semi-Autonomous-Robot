var util  = require('util');
var exec = require('child_process').exec;

ls = exec('ipconfig getifaddr en1');
ls.stdout.on('data', function (data) {
  console.log('stdout: ' + data);
});

ls.stderr.on('data', function (data) {
  console.log('stderr: ' + data);
});

// ls.on('exit', function (code) {
//   console.log('child process exited with code ' + code);
// });
