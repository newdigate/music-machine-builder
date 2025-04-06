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
int x = 0;
void loop()
{
    Tlc.set((x-1) % (NUM_TLCS*16), 0);
    Tlc.set(x % (NUM_TLCS*16), 0xFFFF);
    Tlc.update();
    delayMilliseconds(50);
    x++;
}