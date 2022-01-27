#include <jd_IDLib.h>
#define ARDUINOTRACE_ENABLE true // Enable ArduinoTrace (false = disable, true = enable)

// Use from 0 to 4. Higher number, more debugging messages and memory usage.
#define _ESPASYNC_WIFIMGR_LOGLEVEL_ 4

#include <ezTime.h>
#include <ArduinoTrace.h>
#include <AsyncElegantOTA.h>

// SSID & password are set on Setup
const int MAXSSIDLEN = 32;              // Note this is 31 + null terminator
char ssid[MAXSSIDLEN];         // Content can be changed
const int MAXPASSLEN = 64;              // Note this is 63 + null terminator
char esp_password[MAXPASSLEN]; // Content can be changed
const char *SSID_PREFIX = "ESZ_";       // This format pointer means data CANNOT be changed
const char *PASSWORD_PREFIX = "JD";     // This format pointer means data CANNOT be changed

// Time
Timezone myTZ;
#define MY_TIMEZONE "Asia/Bangkok"

/****************************************************************************************************************************
  Async_AutoConnect_ESP32_minimal.ino
  For ESP8266 / ESP32 boards
  Built by Khoi Hoang https://github.com/khoih-prog/ESPAsync_WiFiManager
  Licensed under MIT license
 *****************************************************************************************************************************/
#if !(defined(ESP32))
#error This code is intended to run on the ESP32 platform! Please check your Tools->Board setting.
#endif
#include <ESPAsync_WiFiManager.h>      //https://github.com/khoih-prog/ESPAsync_WiFiManager
#include <ESPAsync_WiFiManager-Impl.h> //https://github.com/khoih-prog/ESPAsync_WiFiManager

// From v1.0.10 to permit disable/enable StaticIP configuration in Config Portal from sketch. Valid only if DHCP is used.
// You'll loose the feature of dynamically changing from DHCP to static IP, or vice versa
// You have to explicitly specify false to disable the feature.
#define USE_STATIC_IP_CONFIG_IN_CP false

// Use false to disable NTP config. Advisable when using Cellphone, Tablet to access Config Portal.
// See Issue 23: On Android phone ConfigPortal is unresponsive (https://github.com/khoih-prog/ESP_WiFiManager/issues/23)
// Also for this app I will use EzTime which does its own thing with NTP
#define USE_ESP_WIFIMANAGER_NTP false

AsyncWebServer webServer(80);

DNSServer dnsServer;


void testTime()
{
  TRACE();
#if (ARDUINOTRACE_ENABLE)
  Serial.println();
  Serial.println("UTC:             " + UTC.dateTime());

  Serial.println();
  Serial.println("Time in various internet standard formats ...");
  Serial.println();
  Serial.println("ATOM:        " + myTZ.dateTime(ATOM));
  Serial.println("COOKIE:      " + myTZ.dateTime(COOKIE));
  Serial.println("IS8601:      " + myTZ.dateTime(ISO8601));
  Serial.println("RFC822:      " + myTZ.dateTime(RFC822));
  Serial.println("RFC850:      " + myTZ.dateTime(RFC850));
  Serial.println("RFC1036:     " + myTZ.dateTime(RFC1036));
  Serial.println("RFC1123:     " + myTZ.dateTime(RFC1123));
  Serial.println("RFC2822:     " + myTZ.dateTime(RFC2822));
  Serial.println("RFC3339:     " + myTZ.dateTime(RFC3339));
  Serial.println("RFC3339_EXT: " + myTZ.dateTime(RFC3339_EXT));
  Serial.println("RSS:         " + myTZ.dateTime(RSS));
  Serial.println("W3C:         " + myTZ.dateTime(W3C));
  Serial.println();
  Serial.println(" ... and any other format, like \"" + myTZ.dateTime("l ~t~h~e jS ~o~f F Y, g:i A") + "\"");
#endif
}

// Set up timezone and NTP connetion and debug level from:
// 	NONE,
//	ERROR,
//	INFO,
//	DEBUG
void initNTPsetTimezone(String in_tz, ezDebugLevel_t debug_level)
{
  TRACE();
  // Set up time server
  // Uncomment the line below to see what it does behind the scenes
  // setDebug(INFO);
#if (ARDUINOTRACE_ENABLE)
  setDebug(debug_level);
#else
  setDebug(NONE);
#endif
  setInterval(60 * 60); // Set the query to NTP server in seconds (60*60 = 1 hour)
  waitForSync();        // Wait for NTP time sync
  // Provide official timezone names
  // https://en.wikipedia.org/wiki/List_of_tz_database_time_zones
  myTZ.setLocation(in_tz);
  // Wait a little bit to not trigger DDoS protection on server
  // See https://github.com/ropg/ezTime#timezonedropnl
  delay(5000);
  testTime(); // Note that ARDUINOTRACE_ENABLE must be set to true in the sketch to see this output
}
// Test myTZ time routines
// Note that ARDUINOTRACE_ENABLE must be set to true in the sketch to see this output

// Setup the OTA server
void setupOTA()
{
  TRACE();
  // Set up ElegantOTA server
  webServer.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
               { request->send(200, "text/plain", "Hi! I am JDESP32."); });

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
  // Set the SSID & Password up
  getUniqueID(ssid,MAXSSIDLEN - 1, SSID_PREFIX ); 
  getUniqueID(esp_password, MAXPASSLEN - 1, PASSWORD_PREFIX);

  ESPAsync_WiFiManager ESPAsync_wifiManager(&webServer, &dnsServer, "Async_AutoConnect");
  ///////
  // ESPAsync_wifiManager.startConfigPortal((const char *)ssid.c_str(), password.c_str());
  //////
  // ESPAsync_wifiManager.resetSettings();   //reset saved settings
  // ESPAsync_wifiManager.setAPStaticIPConfig(IPAddress(192, 168, 132, 1), IPAddress(192, 168, 132, 1), IPAddress(255, 255, 255, 0));
  ESPAsync_wifiManager.autoConnect((const char*)ssid, (const char*)esp_password); // This is the name of the SSID used when running as web server
  // ESPAsync_wifiManager.autoConnect("AutoConnectAP");
  if (WiFi.status() == WL_CONNECTED)
  {
#if (ARDUINOTRACE_ENABLE)
    Serial.print(F("Connected. Local IP: "));
    Serial.println(WiFi.localIP());
#endif
  }
  else
  {
#if (ARDUINOTRACE_ENABLE)
    Serial.println(ESPAsync_wifiManager.getStatus(WiFi.status()));
#endif
  }
  // Set up time stuff
  initNTPsetTimezone(MY_TIMEZONE, DEBUG); // Debug level can be set to NONE, ERROR, INFO, DEBUG
                                          // Set up ElegantOTA
  // setupOTA();
}

void loop()
{
  TRACE();
  events(); // Allow EzTime to get to NTP server and update time as well as service time events

  delay(10000);
}