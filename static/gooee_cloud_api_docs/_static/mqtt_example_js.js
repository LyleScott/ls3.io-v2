var mqtt = require('mqtt');

// Define Device Meta topic
var deviceTopic = 'devices/1de83fdc-6b4b-cff8-be07-cea1a7dd6343/meta';
var options = {
    host: "mqtt.gooee.io",
    port: 8883,
    username: "authorization",
    password: "0011223344556677b1c2bcaa625fefc5469ca43caa121ca7f0ced65cdc2bdf2a",
    ca: "client.pem"
};

var client = mqtt.createSecureClient(options);

client.on('connect', function () { // Called once when connected
    console.log("Connected successfully!");
});

// update a Device Meta(Set dim to 50)
client.publish(deviceTopic, '[{"name": "dim", "value": 50}]');

// Let's close the connection.
client.end();
