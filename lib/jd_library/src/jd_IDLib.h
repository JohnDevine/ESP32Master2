#include "jd_global.h" 
/** The true ESP32 chip ID is essentially its MAC address.
 * This function provides an alternate chip ID that matches
 * the output of the ESP.getChipId() function on ESP8266
 *
 * The Format of a MAC Address:
 * MAC addresses are 12 digit, 6-byte (48-bits) in length,
 * and are written in MM:MM:MM:SS:SS:SS format.
 * The first 3-bytes are ID number of the manufacturer,
 * which is assigned by an Internet standards body.
 * The second 3-bytes are serial number assigned by the manufacturer.
 *
 */

uint64_t ESP32_getChipId();

bool getUniqueID(char *uniqID, int uniqIDMaxLen, const char *prefix_to_add);



