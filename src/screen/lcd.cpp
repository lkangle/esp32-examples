#include "lcd.h"
#include <JPEGDEC.h>
#include <img_converters.h>

LGFX gfx;

int JPEGDraw(JPEGDRAW *pDraw)
{
    // gfx.pushImage(pDraw->x, pDraw->y, pDraw->iWidth, pDraw->iHeight, pDraw->pPixels);
    // Serial.printf("x: %d, y: %d, %dx%d\n", pDraw->x, pDraw->y, pDraw->iWidth, pDraw->iHeight);
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
        long start = millis();
        if (xQueueReceive(*queue, &jf, 10) == pdTRUE && jf.data != NULL)
        {
            uint8_t *rgb565 = (uint8_t *)heap_caps_malloc(sizeof(uint8_t) * 320 * 480 * 3, MALLOC_CAP_DEFAULT);

            if (jpg2rgb565(jf.data, jf.length, rgb565, JPG_SCALE_NONE))
            {
                Serial.printf("JPEG decode success. use time %ldms\n", millis() - start);
            }
            else
            {
                Serial.println("JPEG decode failed");
            }

            // if (djpeg.openRAM(jf.data, jf.length, JPEGDraw))
            // {
            //     djpeg.setPixelType(RGB565_BIG_ENDIAN);
            //     djpeg.decode(0, 0, 0);
            //     djpeg.close();

            //     Serial.printf("jpeg decoder use time %ldms\n", millis() - start);
            // }
            // else
            // {
            //     Serial.println("JPEG decode failed");
            // }

            heap_caps_free(rgb565);
            heap_caps_free(jf.data);
        }

        // 喂狗
        vTaskDelay(1);
    }

end:
    vTaskDelete(NULL);
}