// app message sent successfully
//
function appMessageAck(e) {
	console.log("App message sent successfully");
}
// app message failed to send
//
function appMessageNack(e) {
	console.log("App message failed to send: " + e.error.message);
}
Pebble.addEventListener("ready",
  function(e) {
    console.log("PebbleKit JS ready!");
  }
);
Pebble.addEventListener("showConfiguration",
  function(e) {
    //Load the remote config page
	var config = "https://dl.dropboxusercontent.com/u/34767519/typeb-config.html";
	var settings = encodeURIComponent(localStorage.getItem("settings"));
	console.log("Opening Config: " + config);
	Pebble.openURL(config + "?settings=" + settings);
  }
);
Pebble.addEventListener("webviewclosed",
  function(e) {
	var settings;
	try {
		settings = JSON.parse(decodeURIComponent(e.response));
		localStorage.clear();
		localStorage.setItem("settings", JSON.stringify(settings));
		console.log("Settings: " + localStorage.getItem("settings"));
		Pebble.sendAppMessage(settings, appMessageAck, appMessageNack);
	} catch(err) {
		settings = false;
		console.log("No JSON response or received Cancel event");
	}
  }
);