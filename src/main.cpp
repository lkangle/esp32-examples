#include <Arduino.h>
#include <font/u8g2_font_unifont_t_chinese4.h>
#include <Arduino_GFX_Library.h>

#define BLO 2

Arduino_DataBus *bus = new Arduino_HWSPI(26, 15, 14, 13, -1);
Arduino_ST7789 gfx(bus, 27, 0, true, 240, 240);

void setup()
{
    Serial.begin(9600);

    // 背光亮起来
    pinMode(BLO, OUTPUT);
    digitalWrite(BLO, HIGH);

    if (!gfx.begin())
    {
        Serial.println("ST7789 failed!");
    }
    Serial.println("ST7789 initialization done!");

    gfx.fillScreen(BLACK);

    gfx.setFont(u8g2_font_unifont_t_chinese4);
    gfx.setUTF8Print(true);
    gfx.setTextColor(WHITE);

    gfx.print("你好，世界！");
}

void loop()
{
}