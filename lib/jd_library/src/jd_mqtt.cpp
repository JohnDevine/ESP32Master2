#include "jd_mqtt.h"

WiFiClient espWiFiClient;
PubSubClient MQTTclient(espWiFiClient);
bool initMQTT( IPAddress ip, uint16_t port, const char *clientID,const char *user, const char *password,  MQTT_CALLBACK_SIGNATURE)
{
    TRACE();
    bool result = false;
    
    // Set up MQTT client
    MQTTclient.setServer(ip, port);
    MQTTclient.setCallback(callback);
    result = MQTTclient.connect(clientID, user, password);

    // MQTTclient.setWill(topic, 0, false, "Offline");
    // MQTTclient.subscribe(subTopic);
    DUMP(result);
    return result;

}
bool publishMqtt(const char *topic, const char *payload)
{
    TRACE();
    bool result = false;
    result = MQTTclient.publish(topic, payload);
    DUMP(result);
    return result;
}