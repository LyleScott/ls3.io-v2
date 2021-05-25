import json

import paho.mqtt.client as mqtt

# The user Api Token
API_TOKEN = "0011223344556677b1c2bcaa625fefc5469ca43caa1"

# Topic to publish to(Device's Meta)
MQTT_TOPIC = "devices/1de83fdc-6b4b-cff8-be07-cea1a7dd6343/meta"

# Message to Send(Meta Updates)
MQTT_MSG = [{"name": "als", "value": 87}, {"name": "temperature", "value": 35}]

# Initiate MQTT Client
client = mqtt.Client()

# Set TLS secure connection
client.tls_set("/home/gooee/client.pem")

# Authenticate
client.username_pw_set("authorization", API_TOKEN)

# Connect to the MQTT Broker
client.connect("mqtt.gooee.io", 8883)

# Publish message to MQTT Broker
client.publish(MQTT_TOPIC, json.dumps(MQTT_MSG))

# Disconnect from MQTT_Broker
client.disconnect()
