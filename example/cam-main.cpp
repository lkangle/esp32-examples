#include <Arduino.h>
#include "camera.h"
#include "screen/lcd.h"
#include <WiFi.h>

#define BLO 21
#define UX_QUEUE_LENGTH 5

xQueueHandle xqh = NULL;

void pushJpegBuf(camera_fb_t *fb) {
    if (xqh == NULL) {
        return;
    }
    
    uint8_t *buf = (uint8_t*)heap_caps_malloc(fb->len * sizeof(uint8_t), MALLOC_CAP_SPIRAM);
    if (buf == NULL) {
        Serial.println("Buffer Malloc Fail.");
        return;
    }
    memcpy(buf, fb->buf, fb->len);

    JpegData jd = {
        .buf = buf,
        .len = fb->len
    };

    if (xQueueSend(xqh, &jd, 10) != pdPASS) {
        heap_caps_free(buf);
    }
}

void setup()
{
    Serial.begin(9600);
    WiFi.mode(WIFI_OFF);

    if (esp_camera_init(&cam_config) != ESP_OK)
    {
        Serial.println("Camera init failed");
        return;
    }
    Serial.println("Camera init success!");

    xqh = xQueueCreate(UX_QUEUE_LENGTH, sizeof(JpegData));

    // 修改寄存器CLKRC值为0x80,开启倍频
    sensor_t *s = esp_camera_sensor_get();
    s->set_reg(s, 0x0111, 0xFF, 0x80);

    // 背光亮起来
    pinMode(BLO, OUTPUT);
    digitalWrite(BLO, HIGH);

    // 在核心0上启动显示任务
    xTaskCreatePinnedToCore(displayTask, "lcd_display", 8192*15, &xqh, 1, NULL, 0);
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

    pushJpegBuf(fb);

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