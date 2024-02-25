#include <Arduino.h>
void setup() {
    Serial.printf("setup\n");
}

void loop() {
    Serial.printf("Please enter a string value:\n");

    //delayMilliseconds(1000);
    String input = Serial.readString();
    Serial.printf("recieved: %s\n", input.c_str());
}