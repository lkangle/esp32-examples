#include <Arduino.h>
#include <Arduino_GFX_Library.h>
#include <JPEGDEC.h>
#include <SPI.h>
#include <esp_jpg_decode.h>
#include "camera.h"
#include "scale.h"

#define BLO 21

static JPEGDEC djpeg;

SPIClass spi(1);
Arduino_DataBus *bus = nullptr;
Arduino_ST7789 *gfx = nullptr;

int JPEGDraw(JPEGDRAW *pDraw)
{
    gfx->draw16bitBeRGBBitmap(pDraw->x + 20, pDraw->y + 60,  pDraw->pPixels, pDraw->iWidth, pDraw->iHeight);
    return 1;
}

void jpeg_decode(camera_fb_t *fb)
{
    if (!fb || !gfx)
    {
        return;
    }

    if (djpeg.openRAM(fb->buf, fb->len, JPEGDraw))
    {
        djpeg.setPixelType(RGB565_BIG_ENDIAN);
        djpeg.decode(0, 0, JPEG_SCALE_QUARTER);
        djpeg.close();
    }
    else
    {
        Serial.println("JPEG decode failed");
    }
}

void setup()
{
    Serial.begin(9600);
    if (esp_camera_init(&cam_config) != ESP_OK)
    {
        Serial.println("Camera init failed");
        return;
    }
    Serial.println("Camera init success!");

    // 修改寄存器CLKRC值为0x80,开启倍频
    sensor_t *s = esp_camera_sensor_get();
    s->set_reg(s, 0x0111, 0xFF, 0x80);

    // 背光亮起来
    pinMode(BLO, OUTPUT);
    digitalWrite(BLO, HIGH);

    // 初始化屏幕
    bus = new Arduino_HWSPI(39, 40, 48, 47, -1, &spi);
    gfx = new Arduino_ST7789(bus, 38, 0, true, 240, 240);

    gfx->begin();
    gfx->fillScreen(BLACK);
    
    gfx->setTextSize(2);
    gfx->setTextColor(WHITE);
    gfx->setCursor(72, 20);
    gfx->printf("ESP32 S3");

    Serial.println("Setup done");
}

time_t prev = 0;
int fps = 0;

void loop()
{
    camera_fb_t *fb = esp_camera_fb_get();
    if (!fb)
    {
        Serial.println("Camera capture failed");
        return;
    }

    if (fb->timestamp.tv_sec != prev)
    {
        prev = fb->timestamp.tv_sec;
        Serial.printf("FPS: %d\n", fps);
        fps = 0;
    }
    else
    {
        fps++;
    }

    jpeg_decode(fb);

    esp_camera_fb_return(fb);
}