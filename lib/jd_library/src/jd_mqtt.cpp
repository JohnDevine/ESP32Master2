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
bool publishMqtt(const char *topic_prefix, const char *topic, const char *payload)
{
    TRACE();
    char full_topic[MAX_TOPIC_LEN];
    bool result = false;
    if (strlen(topic) + strlen(topic_prefix) < MAX_TOPIC_LEN)
    {
        strcpy(full_topic, topic_prefix);
        strcat(full_topic, topic);
        result = MQTTclient.publish(full_topic, payload);
    }    
    DUMP(result);
    return result;
}