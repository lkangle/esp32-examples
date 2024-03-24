#include <Arduino.h>
#include <SPI.h>
#include <font/u8g2_font_unifont_t_chinese4.h>
#include <Arduino_GFX_Library.h>
#include <camera.h>
#include <scale.h>

// 背光引脚
#define BLO 21

SPIClass spi(1);
Arduino_DataBus *bus = nullptr;
Arduino_ST7789 *gfx = nullptr;

void setup()
{
    Serial.begin(9600);
    bus = new Arduino_HWSPI(39, 40, 48, 47, -1, &spi);
    gfx = new Arduino_ST7789(bus, 38, 0, true, 240, 240);

    // 背光亮起来
    pinMode(BLO, OUTPUT);
    digitalWrite(BLO, HIGH);

    if (!gfx->begin())
    {
        Serial.println("ST7789 failed!");
    }
    Serial.println("ST7789 initialization done!");

    if (esp_camera_init(&camera_config2) != ESP_OK)
    {
        Serial.println("Camera init failed");
        return;
    }
    Serial.println("Camera init success!");

    gfx->setRotation(0);
    gfx->fillScreen(BLACK);
    gfx->setFont(u8g2_font_unifont_t_chinese4);
    gfx->setUTF8Print(true);
    gfx->setTextColor(WHITE);

    gfx->setCursor(0, 20);
    gfx->print("Hello 世界!");

    gfx->setCursor(0, 0);
}

void loop()
{
    long start_ms = millis();
    camera_fb_t *fb = esp_camera_fb_get();
    if (!fb)
    {
        Serial.println("Camera capture failed");
        return;
    }

    Serial.printf("size: %u, time: %lums\n", fb->len, millis() - start_ms);

    // int dst_width = 240;
    // int dst_height = 175;

    // uint16_t *dst = (uint16_t *)malloc(dst_width * dst_height * sizeof(uint16_t));
    // if (dst == nullptr)
    // {
    //     Serial.println("Memory allocation failed");
    //     return;
    // }
    // scaleImage((uint16_t *)fb->buf, dst, fb->width, fb->height, dst_width, dst_height);

    // gfx->draw16bitBeRGBBitmap(0, (240 - dst_height) / 2, dst, dst_width, dst_height);

    // free(dst);
    esp_camera_fb_return(fb);
}