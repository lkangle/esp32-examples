#include "lcd.h"
#include <JPEGDEC.h>

SPIClass spi(1);
Arduino_ST7789 *gfx = NULL;

int JPEGDraw(JPEGDRAW *pDraw)
{
    gfx->draw16bitBeRGBBitmap(pDraw->x + 20, pDraw->y + 60,  pDraw->pPixels, pDraw->iWidth, pDraw->iHeight);
    return 1;
}

void displayTask(void *p)
{
    xQueueHandle *queue = (xQueueHandle *)p;

    // 解码器
    JPEGDEC djpeg;

    Arduino_DataBus *bus = new Arduino_HWSPI(39, 40, 48, 47, -1, &spi);
    gfx = new Arduino_ST7789(bus, 38, 0, true, 240, 240);

    gfx->begin();
    gfx->fillScreen(BLACK);

    gfx->setTextSize(2);
    gfx->setTextColor(WHITE);
    gfx->setCursor(72, 20);
    gfx->printf("ESP32 S3");

    Serial.println("Lcd Setup Done");

    JpegData jd;

    while (1)
    {
        if (xQueueReceive(*queue, &jd, 10))
        {
            if (jd.buf == NULL)
            {
                continue;
            }

            if (djpeg.openRAM(jd.buf, jd.len, JPEGDraw))
            {
                djpeg.setPixelType(RGB565_BIG_ENDIAN);
                djpeg.decode(0, 0, JPEG_SCALE_QUARTER);
                djpeg.close();
            }
            else
            {
                Serial.println("JPEG decode failed");
            }

            heap_caps_free(jd.buf);
        }

        // 喂狗
        vTaskDelay(1);
    }

    vTaskDelete(NULL);
}