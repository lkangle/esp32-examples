#include <Arduino.h>
#include "./camera.h"
#include "./lcd.h"

xQueueHandle queue = NULL;
static LGFX lcd;

typedef struct
{
  size_t width;
  size_t height;
  uint8_t *buf;
} QueueItem;

void printHeap()
{
  size_t fsize = heap_caps_get_free_size(MALLOC_CAP_8BIT);
  Serial.printf("free size: %d\n", fsize);
}

// 不断向队列中发视频数据
void captureTask(void *p)
{
  if (esp_camera_init(&camera_config) != ESP_OK)
  {
    Serial.println("Camera init failed");
    goto end;
  }

  for (;;)
  {
    camera_fb_t *fb = esp_camera_fb_get();

    uint8_t *buf = (uint8_t *)heap_caps_calloc(fb->len, sizeof(uint8_t), MALLOC_CAP_8BIT);
    if (buf != NULL)
    {
      memcpy(buf, fb->buf, fb->len);
      QueueItem item = {.width = fb->width,
                        .height = fb->height,
                        .buf = buf};

      xQueueSend(queue, &item, 5);
    }
    else
    {
      size_t fsize = heap_caps_get_free_size(MALLOC_CAP_8BIT);
      Serial.printf("malloc fail..., free size: %d\n", fsize);
    }

    esp_camera_fb_return(fb);
  }

end:
  vTaskDelete(NULL);
}

void setup()
{
  Serial.begin(9600);

  lcd.init();
  lcd.setRotation(0);
  lcd.setBrightness(200);
  lcd.fillScreen(TFT_BLACK);
  lcd.setColorDepth(16);

  queue = xQueueCreate(5, sizeof(QueueItem));
  if (queue == NULL)
  {
    Serial.println("queue create fail...");
    return;
  }

  if (xTaskCreatePinnedToCore(captureTask, "cap_task", 4096, NULL, 0, NULL, 0) != pdPASS)
  {
    Serial.println("task create fail...");
  }
}

void loop()
{
  QueueItem item;
  if (xQueueReceive(queue, &item, 5) == pdPASS)
  {
    lcd.pushImage(32, 48, item.width, item.height, (uint16_t *)item.buf);
    free(item.buf);
  }
}
