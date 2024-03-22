#include <Arduino.h>

void setup()
{
    Serial.begin(9600);
    Serial.printf("Setup done: %d\n", 111);
}

void loop()
{
    Serial.println("1111");

    delay(500);
}