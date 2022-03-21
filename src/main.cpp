#define ARDUINOTRACE_ENABLE true // Enable ArduinoTrace ((#define ARDUINOTRACE_ENABLE = true) = do trace,(#define ARDUINOTRACE_ENABLE = false) = don't trace)

#include "jd_global.h"
#include "jd_IDLib.h"
#include "jd_LEDLib.h"
#include <jd_timeFunctions.h>
#include <ArduinoJson.h>
#include <AsyncElegantOTA.h>

#if !(defined(ESP32))
#error This code is intended to run on the ESP32 platform! Please check your Tools->Board setting.
#endif
#include <ESPAsync_WiFiManager.h> //https://github.com/khoih-prog/ESPAsync_WiFiManager
// #include <ESPAsync_WiFiManager-Impl.h> //https://github.com/khoih-prog/ESPAsync_WiFiManager
// #include <ESPAsync_WiFiManager.hpp>
// Use from 0 to 4. Higher number, more debugging messages and memory usage.
#define _ESPASYNC_WIFIMGR_LOGLEVEL_ 4
// From v1.0.10 to permit disable/enable StaticIP configuration in Config Portal from sketch. Valid only if DHCP is used.
// You'll loose the feature of dynamically changing from DHCP to static IP, or vice versa
// You have to explicitly specify false to disable the feature.
#define USE_STATIC_IP_CONFIG_IN_CP false

// Use false to disable NTP config. Advisable when using Cellphone, Tablet to access Config Portal.
// See Issue 23: On Android phone ConfigPortal is unresponsive (https://github.com/khoih-prog/ESP_WiFiManager/issues/23)
// Also for this app I will use EzTime which does its own thing with NTP
// DO NOT USE THE NTP here as it is setup separately later
#define USE_ESP_WIFIMANAGER_NTP false

// The OTA webserver is set up on this
AsyncWebServer webServer(80);
DNSServer dnsServer;

// JSON Stuff
const int JSON_BUFFER_SIZE = 256; // Size is calculated from arduinojson.org/v6/assistant/
const int MAX_PAYLOAD_LEN = 256;  // Maximum Length of the buffer we use to write to mqtt

// MQTT stuff
#include "jd_mqtt.h"
const uint16_t mqttPort = 1883;
IPAddress mqttServer(192, 168, 1, 3);
const int MAXUSERLEN = 20;
char mqttUser[MAXUSERLEN];
const int MAXMQTTPASSLEN = 20;
char mqttPassword[MAXMQTTPASSLEN];
const int MAXMQTTIDLEN = 20;
char mqttClientID[MAXMQTTIDLEN];
const char *MQTT_USER_PREFIX = "ESP_"; // This format pointer means data CANNOT be changed
const char *MQTT_PASSWORD_PREFIX = "JD_";
const char *MQTT_CLIENT_PREFIX = "CL_";
const char *MQTT_TOPIC_PREFIX = "JHD/BaanFarang/tele/SolarMobile/";
const int MAX_DEVICE_ID_LEN = 20;
char deviceID[MAX_DEVICE_ID_LEN];

// WiFi SSID & password are set on Setup
const int MAXSSIDLEN = 32;          // Note this is 31 + null terminator
char ssid[MAXSSIDLEN];              // Content can be changed
const int MAXPASSLEN = 64;          // Note this is 63 + null terminator
char esp_password[MAXPASSLEN];      // Content can be changed
const char *SSID_PREFIX = "ESZ_";   // This format pointer means data CANNOT be changed
const char *PASSWORD_PREFIX = "JD"; // This format pointer means data CANNOT be changed

// Global Variables
float calc_volts = 19.01;

void mqttCallback(char *topic, byte *payload, unsigned int length)
{
  // handle message arrived
  TRACE()

  DUMP(length);
}

void setupOTA()
{
  TRACE();
  // Set up ElegantOTA server
  webServer.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
               { request->send(200, "text/plain", "Hi! I am a JD ESP32."); });

  AsyncElegantOTA.begin(&webServer); // Start ElegantOTA
  webServer.begin();
#if (ARDUINOTRACE_ENABLE)
  Serial.println("Elegant HTTP server started");
#endif
}
void setup()
{
  TRACE();
#if (ARDUINOTRACE_ENABLE)
  Serial.begin(115200);
  while (!Serial)
    ;
  delay(200);

  Serial.print("\nStarting on " + String(ARDUINO_BOARD));
  Serial.println(ESP_ASYNC_WIFIMANAGER_VERSION);
#endif
  // Initialise the led pin as an output
  init_led(ESP32_LED_BUILTIN);

  // Set the SSID. Password and deviceid up
  getUniqueID(ssid, MAXSSIDLEN - 1, SSID_PREFIX);
  getUniqueID(esp_password, MAXPASSLEN - 1, PASSWORD_PREFIX);
  getUniqueID(deviceID, MAX_DEVICE_ID_LEN - 1, "");

      // Create WiFiManager instance
      ESPAsync_WiFiManager ESPAsync_wifiManager(&webServer, &dnsServer, "Async_AutoConnect");
  ///////
  // ESPAsync_wifiManager.startConfigPortal((const char *)ssid.c_str(), password.c_str());
  //////
  // ESPAsync_wifiManager.resetSettings();   //reset saved settings
  // ESPAsync_wifiManager.setAPStaticIPConfig(IPAddress(192, 168, 132, 1), IPAddress(192, 168, 132, 1), IPAddress(255, 255, 255, 0));

  // Set config portal settings
  ESPAsync_wifiManager.autoConnect((const char *)ssid, (const char *)esp_password); // This is the name of the SSID used when running as web server
  // ESPAsync_wifiManager.autoConnect("AutoConnectAP");
  if (WiFi.status() == WL_CONNECTED)
  {
#if (ARDUINOTRACE_ENABLE)
    Serial.print(F("Connected. Local IP: "));
    Serial.println(WiFi.localIP());
#endif
    blinkLED(ESP32_LED_BUILTIN, PIN_HIGH, false);
  }
  else
  {
#if (ARDUINOTRACE_ENABLE)
    Serial.println(ESPAsync_wifiManager.getStatus(WiFi.status()));
#endif
    blinkLED(ESP32_LED_BUILTIN, PIN_LOW, false);
  }
  // Set up time stuff
  initNTPsetTimezone(DEBUG); // Debug level can be set to NONE, ERROR, INFO, DEBUG
                                          // Set up ElegantOTA
  setupOTA();

  // Set up MQTT
  getUniqueID(mqttClientID, MAXMQTTIDLEN - 1, MQTT_CLIENT_PREFIX);
  getUniqueID(mqttUser, MAXUSERLEN - 1, MQTT_USER_PREFIX);
  getUniqueID(mqttPassword, MAXMQTTPASSLEN - 1, MQTT_PASSWORD_PREFIX);
  getUniqueID(deviceID, MAX_DEVICE_ID_LEN - 1, "");

  initMQTT(mqttServer, mqttPort, mqttClientID, mqttUser, mqttPassword, mqttCallback);
}

void loop()
{
  char DateTime[30]; // Buffer to hold current data & time in ISO8601 format + 10
  char output[MAX_PAYLOAD_LEN];

  StaticJsonDocument<JSON_BUFFER_SIZE> doc;

  events(); // Allow EzTime to get to NTP server and update time as well as service time events

  jd_getCurrentTime(DateTime, MAX_PAYLOAD_LEN - 1, ISO8601);

// Create the JSON document
  doc["Time"] = DateTime;
  doc["DeviceId"] = deviceID;
  JsonObject sensor_ADS1115 = doc["sensor"].createNestedObject("ADS1115");
  sensor_ADS1115["Volts"] = calc_volts;
  sensor_ADS1115["Amps"] = 12.34;
  doc["ver"] = 1;

  serializeJson(doc, output);

  // strncpy(payload, "Hello World!", MAX_PAYLOAD_LEN - 1);
  // Publish an MQTT message
  publishMqtt(MQTT_TOPIC_PREFIX, "", output);
  delay(10000);
  calc_volts = calc_volts + 0.01;

}