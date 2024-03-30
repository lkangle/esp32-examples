#include <Arduino.h>
#include <WiFi.h>
#include <Arduino_GFX_Library.h>

// 背光
#define PIN_BLO 9
#define TFT_DC 11
#define TFT_CS 12
#define PIN_WR 14
#define PIN_RD 13
#define PIN_RST 10
#define PIN_D0 18
#define PIN_D1 8
#define PIN_D2 17
#define PIN_D3 7
#define PIN_D4 16
#define PIN_D5 6
#define PIN_D6 15
#define PIN_D7 5
#define PIN_D8 41
#define PIN_D9 4
#define PIN_D10 42
#define PIN_D11 3
#define PIN_D12 45
#define PIN_D13 2
#define PIN_D14 46
#define PIN_D15 1

Arduino_DataBus *bus = new Arduino_ESP32LCD8(
    TFT_DC, TFT_CS, PIN_WR, PIN_RD,
    PIN_D0, PIN_D1, PIN_D2, PIN_D3, PIN_D4, PIN_D5, PIN_D6, PIN_D7);
Arduino_ST7796 *gfx = nullptr;

void setup()
{
    Serial.begin(9600);
    WiFi.mode(WIFI_OFF);

    gfx = new Arduino_ST7796(bus, PIN_RST, 3, true, 320, 480);

    gfx->begin();
    gfx->fillScreen(BLACK);

    gfx->setTextSize(2);
    gfx->setTextColor(WHITE);
    gfx->println("Hello World");

    delay(500);

    pinMode(PIN_BLO, OUTPUT);
    digitalWrite(PIN_BLO, 1);
}

void loop()
{
}