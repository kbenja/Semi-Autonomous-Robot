function getFile(filepath, callback) {
  var xobj = new XMLHttpRequest();
  xobj.open('GET', filepath, true);
  xobj.onreadystatechange = function () {
    if (xobj.readyState == 4 && xobj.status == "200") callback(xobj.responseText);
  };
  xobj.send(null);
}
