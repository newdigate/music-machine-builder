#include "Tlc5940.h"
#include "tlc_shifts.h"

// which analog pin to use
#define ANALOG_PIN      0

// which pin to clear the LEDs with
#define CLEAR_PIN      12

// how many millis for one full revolution over all the LEDs
#define SCOPE_PERIOD    (2000 * NUM_TLCS)
#define LED_PERIOD      SCOPE_PERIOD / (NUM_TLCS * 16)

void setup()
{
    pinMode(CLEAR_PIN, INPUT);
    digitalWrite(CLEAR_PIN, HIGH); // enable pull-up
    Tlc.init();
}

void loop()
{
    // shiftUp returns the value shifted off the last pin
    uint16_t sum = tlc_shiftUp() + analogRead(ANALOG_PIN) * 4;
    if (digitalRead(CLEAR_PIN) == LOW || sum > 4095)
        sum = 0;
    Tlc.set(0, sum);
    Tlc.update();
    delay(LED_PERIOD);
}