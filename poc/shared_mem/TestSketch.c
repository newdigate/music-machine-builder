#include <stdio.h>
#include <unistd.h>
extern int magicNumber;

void setup() {
   printf("setup %d\n", magicNumber);
}

void loop() {
   printf("loop %d\n", magicNumber);
   sleep(1);
}