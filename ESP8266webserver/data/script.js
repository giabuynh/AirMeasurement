var time = 1000;
var flame = 1;

setInterval(function() {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200)
      document.getElementById("temperature").innerHTML = this.responseText;
  };
  xhttp.open("GET", "/temperature", true);
  xhttp.send();
}, time);

setInterval(function() {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200)
      document.getElementById("humidity").innerHTML = this.responseText;
  };
  xhttp.open("GET", "/humidity", true);
  xhttp.send();
}, time);

setInterval(function() {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200)
      document.getElementById("alcohol").innerHTML = this.responseText;
  };
  xhttp.open("GET", "/alcohol", true);
  xhttp.send();
}, time);

setInterval(function() {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200)
    {
      var newF = this.responseText;
      console.log(newF + " " + flame);
      if (newF == 0 && newF != flame)
        document.getElementById('flame-modal').style.display = "flex";
      flame = newF;
    }
  };
  xhttp.open("GET", "/flame", true);
  xhttp.send();
}, time);
