#include "jd_global.h" 

/**
 * Blink the ESP32_LED_BUILTIN blinkcount times 
 * and if forever is true then blink forever
 * 
 * @param pin The pin the LED is on (See the LED pins above, usually ESP32_LED_BUILTIN)
 * @param blinkcount The number of times to blink (If PIN_LOW the just turn the LED OFF or If PIN_HIGH the just turn the LED ON)
 * @param forever If true then halt & blink forever
 * 
 * @return  
 *          - 64 bit Chip ID
 * 
 */
uint64_t ESP32_getChipId();
/**
 * Blink the ESP32_LED_BUILTIN blinkcount times 
 * and if forever is true then blink forever
 * 
 * @param uniqID the buffer that the unique ID will be written to
 * @param uniqIDMaxLen the maximum length of the buffer (Dont forget to add 1 for the null terminator)
 * @param prefix_to_add the prefix to add to the unique ID
 * 
 * @return  
 *         - true if successful
 *         - false if not successful
 * 
 */
bool getUniqueID(char *uniqID, int uniqIDMaxLen, const char *prefix_to_add);



