#include "jd_global.h" 

#define BLINK_DELAY 300         // Delay between flashes in milliseconds
#define BLINK_REPEAT_DELAY 5000 // Delay between repeats in milliseconds


//***************************Blink Status Setup for LED on ESP Board***************************************************************************************
// Status Blinks
#define BLINK_OFF 0 // TURN LED
#define BLINK_ON 1 // Turn LED ON
#define BLINK_WIFI_FAIL 2 // WiFi Did not initialise
#define BLINK_SENT_MQTT 3 // Message sent off to MQTT server
#define BLINK_COUNT_01 4  // Light Sensor 1 failed
#define BLINK_COUNT_02 5  // Light Sensor 2 failed
#define BLINK_COUNT_03 6  // Servo Across failed
#define BLINK_COUNT_04 7 // Servo UpDown failed

/**
 * Initialise the LED pin
 *
 * @param pin  The pin the LED is on (See the LED pins above, usually ESP32_LED_BUILTIN)
 *
 * @return  
 *          - true    
 *          
 */
bool init_led(int pin);

/**
 * Blink the ESP32_LED_BUILTIN blinkcount times 
 * and if forever is true then blink forever
 * 
 * @param pin The pin the LED is on (See the LED pins above, usually ESP32_LED_BUILTIN)
 * @param blinkcount The number of times to blink (If PIN_LOW the just turn the LED OFF or If PIN_HIGH the just turn the LED ON)
 * @param forever If true then halt & blink forever
 * 
 * @return  
 *          - none
 * 
 */
void blinkLED(int pin, int blinkcount, bool forever);
