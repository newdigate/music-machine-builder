#include <Arduino.h>
extern "C" void setup() {
    Serial.printf("setup %d\n", 666);
}

extern "C" void loop() {
    Serial.printf("loop %d\n", 666);
    delayMilliseconds(1000);
}