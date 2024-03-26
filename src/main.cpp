#include <Arduino.h>
#include "camera.h"

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

    esp_camera_fb_return(fb);
}