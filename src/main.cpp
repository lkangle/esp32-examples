#include <Arduino.h>
#include "hardware/flash.h"

using namespace arduino;

#define LED_PIN 25

int count = 0;

void setup()
{
    Serial.begin(9600);
    Serial.println("Hello World");

    pinMode(LED_PIN, OUTPUT);
}

void loop()
{
    void *m = malloc(40960);
    if (m != nullptr)
    {
        Serial.println("Memory allocated ok.");
        free(m);
    }
    else
    {
        Serial.println("Memory allocation failed");
    }

    Serial.println(String("--> Loop count: ") + String(count++));

    for (int i = 0; i < 256; i++)
    {
        analogWrite(LED_PIN, i);
        delay(1);
    }

    for (int i = 255; i >= 0; i--)
    {
        analogWrite(LED_PIN, i);
        delay(1);
    }

    delay(500);
}