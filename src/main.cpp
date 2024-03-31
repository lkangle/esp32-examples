#include <Arduino.h>
#include "lcd.h"
#include "net.h"
#include <JPEGDEC.h>

LGFX gfx;
JPEGDEC djpeg;

int JPEGDraw(JPEGDRAW *pDraw)
{
    gfx.pushImage(pDraw->x, pDraw->y, pDraw->iWidth, pDraw->iHeight, pDraw->pPixels);
    return 1;
}

void handleFrame(uint8_t *data, size_t len)
{
    void *buf = heap_caps_malloc(sizeof(uint8_t) * len, MALLOC_CAP_DEFAULT);
    memcpy(buf, data, len);

    long start = millis();

    if (djpeg.openRAM((uint8_t *)buf, len, JPEGDraw))
    {
        djpeg.setPixelType(RGB565_BIG_ENDIAN);
        djpeg.decode(0, 0, 0);
        djpeg.close();
    }
    else
    {
        Serial.println("JPEG decode failed");
    }

    Serial.printf("jpeg handle time %ldms\n", millis() - start);

    heap_caps_free(buf);
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

    void *fp = (void *)handleFrame;
    xTaskCreatePinnedToCore(readVideoStream, "video_stream", 8192 * 10, fp, 0, NULL, 0);
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