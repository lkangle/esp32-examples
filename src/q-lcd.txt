#include <Arduino.h>
#include <camera.h>
#include <lcd.h>

xQueueHandle xqh = NULL;
TaskHandle_t videoTask = NULL;

// 显示任务
void display(void *p)
{
    LGFX lcd;
    lcd.init();
    lcd.setRotation(0);
    lcd.setBrightness(200);
    lcd.setColorDepth(16);
    lcd.fillScreen(TFT_BLACK);

    for (;;)
    {
        camera_fb_t fb;
        if (xQueueReceive(xqh, &fb, 5) == pdTRUE)
        {
            lcd.pushImage(0, 0, fb.width, fb.height, (uint16_t *)fb.buf);
            heap_caps_free(fb.buf);
        }
    }

    vTaskDelete(NULL);
}

void video(void *p)
{
    if (esp_camera_init(&camera_config) != ESP_OK)
    {
        Serial.println("Camera Init Failed");
        return;
    }
    Serial.println("Camera Init Succeed");

    for (;;)
    {
        camera_fb_t *fb = esp_camera_fb_get();
        if (!fb)
        {
            Serial.println("Camera Capture Failed");
            return;
        }

        uint8_t *buf = (uint8_t *)heap_caps_malloc(fb->len * sizeof(uint8_t), MALLOC_CAP_DEFAULT);
        if (buf != NULL)
        {
            memcpy(buf, fb->buf, fb->len);
            camera_fb_t cfb = {
                .buf = buf,
                .len = fb->len,
                .width = fb->width,
                .height = fb->height,
            };

            if (xQueueSend(xqh, &cfb, 5) != pdPASS)
            {
                Serial.println("Queue Send Failed");
            }
        }

        esp_camera_fb_return(fb);
    }

    vTaskDelete(NULL);
}

void setup()
{
    Serial.begin(9600);

    xqh = xQueueCreate(10, sizeof(camera_fb_t));
    if (xqh == NULL)
    {
        Serial.println("queue create fail...");
        return;
    }

    if (xTaskCreatePinnedToCore(display, "display", 8192, NULL, 1, NULL, 0) != pdTRUE)
    {
        Serial.println("Task(display) Create Failed");
    }
    if (xTaskCreatePinnedToCore(video, "video", 8192, NULL, 1, &videoTask, 0) != pdTRUE)
    {
        Serial.println("Task(video) Create Failed");
    }
}

void loop()
{
}