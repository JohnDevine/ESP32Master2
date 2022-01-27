# ESP32Master2
This is a framework master for my ESP32 projects NOTE NOT ESP8266.
It is setup for 
    1. MAC mini Catalina Ver:10.15.7
    2. Visual Studio Code Version: 1.63.2
    3. Platformio extension Core 5.2.4·Home 3.4.0
    
The objective is to have a base for an ESP32 that can
1. Look for a wifi network and connect to it. (NOTE .. The first time   it starts up it will go straight to the webserver)
    a. If it cannot find the one coded in then open a web server and allow the user to input the SSID and Password then to keep that entry and use that for subsequent boots.
    b. NOTE the credentials are held in NVR and the only way to totally remove them is to manually clear the NVR on the ESP
        i: Start CLI ... To get to CLI in LHS menu select PIO (Ant Icon)
        then in "QUICK ACCESS MENU" select under Miscellaneous PlatformIO Core CLI
        ii: pio run --target erase
        iii: (may take more than one attempt)
2. Allow for OTA (Over The Air) updates by having a webserver on url:
     http://(YourESP32IP)/update 
     eg 
     http://192.168.2.42/update
     that will load a .bin up to the ESP32 and reboot.
3. Connect to an MQTT server so messages can be published and also
    recieved.For the MQTT connection:
        a: The maximum message size, including header, is 256 bytes by default. This is configurable via MQTT_MAX_PACKET_SIZE in PubSubClient.h or can be changed by calling PubSubClient::setBufferSize(size).
        b:The keepalive interval is set to 15 seconds by default. This is configurable via MQTT_KEEPALIVE in PubSubClient.h or can be changed by calling PubSubClient::setKeepAlive(keepAlive).
    Set These entries to match your requirements
        const char *mqtt_server = "192.168.1.3";
        char fullTopic[200] = "test/temperature";
4. Send messages via:
    PubSubClient client(espClient); // MQTT client for PubSubClient library
    client.publish(fullTopic, jsonOutput);

4. Provide a blink status display via 
    void blinkLED(int pin, int blinkcount, bool forever)

5. Provide a connection to a timeserver so the correct time can be found. 
    Set it up via
        String myNTPServer = "pool.ntp.org";
        uint16_t myNTPInterval = 120 * 60; // Contact the server every 120 minutes (uses time in secs)
    Get the time via a call:
        String fullDateTime = myTZ.dateTime(ISO8601);
    See: https://github.com/ropg/ezTime
6. Provide a basic CONSOLE debug facility via ArduinoTrace
    a. See full description here:
        https://github.com/bblanchon/ArduinoTrace
    b. Turn on/off via the entry:
        #define ARDUINOTRACE_ENABLE true // Enable ArduinoTrace (false = disable, true = enable)


Needed Libraries:
monitor_speed = 115200
lib_deps = 
	ayushsharma82/AsyncElegantOTA@^2.2.6
	knolleary/PubSubClient@^2.8
	ropg/ezTime@^0.8.3
	bblanchon/ArduinoTrace@^1.2.0
    khoih-prog/ESPAsync_WiFiManager@^1.10.0

    NOTE!!!!!!!! There are 2 libraries with the same name 
    ESPAsync_WiFiManager
    get the second one khoih-prog/ESPAsync_WiFiManager (note the -) or you will get duplicate definition errors
