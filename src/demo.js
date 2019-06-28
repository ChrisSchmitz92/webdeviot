var mqtt;
var reconnectTimeout = 2000;
var host = "139.6.184.34";
var port = 8000;

function onConnect() {
  console.log("Connected.");
  //mqtt.subscribe("webdeviot/bestboiz");
  message = new Paho.MQTT.Message("456+-123/* cola");
  message.destinationName = "webdeviot/bestboiz";
  mqtt.send(message);

  msg = new Paho.MQTT.Message(myNodelist);
  message.destinationName = "webdeviot/bestboiz";
  mqtt.send(msg);
}

function MQTTconnect() {
  console.log("connecting to " + host + "and " + port);
  mqtt = new Paho.MQTT.Client(host, port, "clientjs");
  var options = {
    timeout: 3,
    onSuccess: onConnect,
    onFailure: onFailure
  };

  mqtt.onMessageArrived = onMessageArrived;

  mqtt.connect(options);
}

function onFailure(message) {
  console.log("Connection Attempt to Host " + host);
  setTimeout(MQTTconnect, reconnectTimeout);
}

function onMessageArrived(msg) {
  out_msg = "Message received " + msg.payloadString + "<br>";
  out_msg = out_msg + "Message received Topic " + msg.destinationName;
  console.log(out_msg);
}

MQTTconnect();

// Create a "close" button and append it to each list item
var myNodelist = document.getElementsByTagName("LI");
var i;
for (i = 0; i < myNodelist.length; i++) {
  var span = document.createElement("SPAN");
  var txt = document.createTextNode("\u00D7");
  span.className = "close";
  span.appendChild(txt);
  myNodelist[i].appendChild(span);
}

// Click on a close button to hide the current list item
var close = document.getElementsByClassName("close");
var i;
for (i = 0; i < close.length; i++) {
  close[i].onclick = function() {
    var div = this.parentElement;
    div.style.display = "none";
  };
}

// Add a "checked" symbol when clicking on a list item
var list = document.querySelector("ul");
list.addEventListener(
  "click",
  function(ev) {
    if (ev.target.tagName === "LI") {
      ev.target.classList.toggle("checked");
    }
  },
  false
);

// Create a new list item when clicking on the "Add" button
function newElement() {
  var li = document.createElement("li");
  var inputValue = document.getElementById("myInput").value;
  var t = document.createTextNode(inputValue);
  li.appendChild(t);
  if (inputValue === "") {
    alert("You must write something!");
  } else {
    document.getElementById("myUL").appendChild(li);
  }
  document.getElementById("myInput").value = "";

  var span = document.createElement("SPAN");
  var txt = document.createTextNode("\u00D7");
  span.className = "close";
  span.appendChild(txt);
  li.appendChild(span);

  for (i = 0; i < close.length; i++) {
    close[i].onclick = function() {
      var div = this.parentElement;
      div.style.display = "none";
    };
  }
}

MQTTconnect();