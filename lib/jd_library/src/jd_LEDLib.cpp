#include "jd_LEDLib.h"
// In initialise the LED
bool init_led(int pin)
{
  DUMP(pin);
  pinMode(pin, OUTPUT);
  return true;
}
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
void blinkLED(int pin, int blinkcount, bool forever)
{
  TRACE();
  DUMP(pin);
  DUMP(blinkcount);
  DUMP(forever);
  
  if (blinkcount == PIN_LOW)
  {
    digitalWrite(pin, LOW);
  }
  else if (blinkcount == PIN_HIGH)
  {
    digitalWrite(pin, HIGH);
  }
  else
  {
    if (forever)
    {
      while (true)
      {
        for (int i = 0; i < blinkcount; i++)
        {
          digitalWrite(pin, HIGH);
          delay(BLINK_DELAY);
          digitalWrite(pin, LOW);
          delay(BLINK_DELAY);
        }
        delay(BLINK_REPEAT_DELAY);
      }
    }
    else
    {
      for (int i = 0; i < blinkcount; i++)
      {
        digitalWrite(pin, HIGH);
        delay(BLINK_DELAY);
        digitalWrite(pin, LOW);
        delay(BLINK_DELAY);
      }
      delay(BLINK_REPEAT_DELAY);
    }
  }
}