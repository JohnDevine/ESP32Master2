#include "jd_IDLib.h"

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

uint64_t ESP32_getChipId()
{
    TRACE();
    uint64_t chipId = ESP.getEfuseMac();
    uint8_t mac_len = 6;
    uint8_t mac[mac_len];
    if (chipId == 0)
    {
        TRACE();
        // ESP32 chip ID is not set. Chip ID is mainly used for Wifi MAC address.
        // So we use the MAC address to generate a unique chip ID.

        esp_efuse_mac_get_default(mac);
        chipId = mac[2];
        chipId = (chipId << 8) | mac[3];
        chipId = (chipId << 8) | mac[4];
        chipId = (chipId << 8) | mac[5];
    }

    TRACE();
    return chipId;
}
// Get a unique identifier from the ESP32 chip and add a prefix
bool getUniqueID(char *uniqID, int uniqIDMaxLen, const char *prefix_to_add)
{
    TRACE();
    uint64_t chipId = ESP32_getChipId();
    char chipIdStr[13];
    char chipIdStr2[13];

    // Check to see if input uniqIDMaxLen is large enough
    if (uniqIDMaxLen < (strlen(prefix_to_add) + 12 + 1)) // allow for trailing null
    {
        return false;
    }

 
    // sprintf the ESP32_getChipId() to a string and concatinate it to uniqID

    //   %[flags][width][.precision][length]specifier
    sprintf(chipIdStr, "%012llX", chipId);
    chipIdStr2[0] = chipIdStr[10];
    chipIdStr2[1] = chipIdStr[11];
    chipIdStr2[2] = chipIdStr[8];
    chipIdStr2[3] = chipIdStr[9];
    chipIdStr2[4] = chipIdStr[6];
    chipIdStr2[5] = chipIdStr[7];
    chipIdStr2[6] = chipIdStr[4];
    chipIdStr2[7] = chipIdStr[5];
    chipIdStr2[8] = chipIdStr[2];
    chipIdStr2[9] = chipIdStr[3];
    chipIdStr2[10] = chipIdStr[0];
    chipIdStr2[11] = chipIdStr[1];
    chipIdStr2[12] = '\0';
    sprintf(uniqID, "%s%s", prefix_to_add, chipIdStr2);
    DUMP(uniqID);
    return true;
}