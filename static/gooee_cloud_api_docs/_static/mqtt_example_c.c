#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "MQTTClient.h"

#define ADDRESS "ssl://mqtt.gooee.io:8883"
#define CLIENTID "sample_gooee_client"
#define API_TOKEN "0011223344556677b1c2bcaa625fefc5469ca43caa1"
#define TOPIC "devices/1de83fdc-6b4b-cff8-be07-cea1a7dd6343/meta"
#define PAYLOAD "[{\"name\": \"als\", \"value\": 37}]"
#define QOS 1
#define SERVER_CA_FILE "client.pem"
#define TIMEOUT 10000L

int main(int argc, char* argv[])
{
    MQTTClient client;
    MQTTClient_connectOptions conn_opts = MQTTClient_connectOptions_initializer;
    MQTTClient_message pubmsg = MQTTClient_message_initializer;
    MQTTClient_deliveryToken token;
    int rc;
    int status;

    MQTTClient_create(&client, ADDRESS, CLIENTID, MQTTCLIENT_PERSISTENCE_NONE, NULL);
    MQTTClient_SSLOptions ssl_opts = MQTTClient_SSLOptions_initializer;
    ssl_opts.trustStore = SERVER_CA_FILE;
    ssl_opts.enableServerCertAuth = 0;

    conn_opts.keepAliveInterval = 30;
    conn_opts.cleansession = 1;
    conn_opts.username = "authorization";
    conn_opts.password = API_TOKEN;
    conn_opts.ssl = &ssl_opts;

    if ((rc = MQTTClient_connect(client, &conn_opts)) != MQTTCLIENT_SUCCESS)
    {
        printf("Failed to connect, return code %d\n", rc);
        exit(-1);
    }

    pubmsg.payload = PAYLOAD;
    pubmsg.payloadlen = strlen(PAYLOAD);
    pubmsg.qos = QOS;
    pubmsg.retained = 0;

    status=MQTTClient_publishMessage(client, TOPIC, &pubmsg, &token);
    printf("status is %d \n", status);
    rc = MQTTClient_waitForCompletion(client, token, TIMEOUT);
    printf("Message with delivery token %d delivered\n", token);
    MQTTClient_disconnect(client, 10000);
    MQTTClient_destroy(&client);
    return rc;
}
