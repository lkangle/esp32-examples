#include <Arduino.h>
#include "lcd.h"
#include "net.h"

LGFX gfx;

void handleJpeg(uint8_t *data, size_t len)
{
}

void setup()
{
    Serial.begin(9600);
    WiFi.mode(WIFI_STA);

    if (!gfx.begin())
    {
        Serial.println("屏幕初始化失败！");
        return;
    }

    gfx.setRotation(3);
    gfx.setBrightness(255);
    gfx.setColorDepth(lgfx::color_depth_t::palette_8bit);
    gfx.fillScreen(TFT_BLACK);

    connectWiFi();
    readVideoStream();
}

void loop()
{
    // gfx.fillRect(0, 0, 480, 40, TFT_BLUE);
    // gfx.fillRect(0, 40, 480, 40, TFT_RED);
    // gfx.fillRect(0, 80, 480, 40, TFT_GREEN);
    // gfx.fillRect(0, 120, 480, 40, TFT_CYAN);
    // gfx.fillRect(0, 160, 480, 40, TFT_WHITE);
    // gfx.fillRect(0, 200, 480, 40, TFT_DARKGRAY);
    // gfx.fillRect(0, 240, 480, 40, TFT_YELLOW);
    // gfx.fillRect(0, 280, 480, 40, TFT_PINK);
}