#include <Arduino.h>
#include <camera.h>
#include <lcd.h>

static LGFX lcd;

void setup()
{
    Serial.begin(9600);
    if (esp_camera_init(&camera_config) != ESP_OK)
    {
        Serial.println("Camera Init Failed");
        return;
    }
    Serial.println("Camera Init Succeed");

    lcd.init();
    lcd.setRotation(0);
    lcd.setBrightness(200);
    lcd.fillScreen(TFT_BLACK);
    lcd.setColorDepth(16);
}

void loop()
{
    camera_fb_t *fb = esp_camera_fb_get();
    if (!fb)
    {
        Serial.println("Camera Capture Failed");
        return;
    }

    lcd.pushImage(0, 0, fb->width, fb->height, (uint16_t *)(fb->buf));

    esp_camera_fb_return(fb);
}