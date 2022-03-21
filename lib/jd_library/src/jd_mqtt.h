#include "jd_global.h" 
// #include <ESPAsync_WiFiManager.h>      //https://github.com/khoih-prog/ESPAsync_WiFiManager
// #include <ESPAsync_WiFiManager-Impl.h> 
#include <ESPAsync_WiFiManager.hpp>
#include <PubSubClient.h>
const int MAX_TOPIC_LEN = 50;

bool initMQTT( IPAddress ip, uint16_t port, const char *clientID,const char *user, const char *password,  MQTT_CALLBACK_SIGNATURE);
bool publishMqtt(const char *topic_prefix, const char *topic, const char *payload);