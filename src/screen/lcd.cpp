#include "lcd.h"
#include <JPEGDEC.h>

LGFX gfx;

int JPEGDraw(JPEGDRAW *pDraw)
{
    gfx.pushImage(pDraw->x, pDraw->y, pDraw->iWidth, pDraw->iHeight, pDraw->pPixels);
    return 1;
}

void displayTask(void *p)
{
    xQueueHandle *queue = (xQueueHandle *)p;

    if (!gfx.begin())
    {
        Serial.println("屏幕初始化失败！");
        goto end;
    }

    gfx.setRotation(3);
    gfx.setBrightness(255);
    gfx.setColorDepth(lgfx::color_depth_t::palette_8bit);
    gfx.fillScreen(TFT_BLACK);

    // 解码器
    JPEGDEC djpeg;

    JpegFrame jf;

    while (1)
    {
        if (xQueueReceive(*queue, &jf, 10) == pdTRUE && jf.data != NULL)
        {
            if (djpeg.openRAM(jf.data, jf.length, JPEGDraw))
            {
                djpeg.setPixelType(RGB565_BIG_ENDIAN);
                djpeg.decode(0, 0, 0);
                djpeg.close();
            }
            else
            {
                Serial.println("JPEG decode failed");
            }

            heap_caps_free(jf.data);
        }

        // 喂狗
        vTaskDelay(1);
    }

end:
    vTaskDelete(NULL);
}