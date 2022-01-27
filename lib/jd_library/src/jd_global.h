#if !(defined(JD_GLOBAL_H))
    #define JD_GLOBAL_H

    #if !(defined(ESP32))
    #error This code is intended to run on the ESP32 platform! Please check your Tools->Board setting.
    #endif

    #ifndef Arduino_h
    #include <Arduino.h>
    #endif

    #define ARDUINOTRACE_ENABLE true // Enable ArduinoTrace (false = disable, true = enable)
    #include <ArduinoTrace.h>

#endif
