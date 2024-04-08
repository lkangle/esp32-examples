#include <Arduino.h>
#include "screen/lcd.h"
#include "net.h"
#include <JPEGDEC.h>

xQueueHandle queue;

void handleFrame(uint8_t *data, size_t len)
{
    void *buf = heap_caps_malloc(sizeof(uint8_t) * len, MALLOC_CAP_DEFAULT);
    memcpy(buf, data, len);

    JpegFrame frame = {
        .length = len,
        .data = (uint8_t *)buf,
    };

    // 发送失败直接丢弃
    if (xQueueSend(queue, &frame, 3) != pdTRUE)
    {
        // Serial.printf("Queue Send Fail. size: %d\n", len);
        heap_caps_free(buf);
    }
}

void run_setup()
{
    Serial.begin(9600);
    WiFi.mode(WIFI_STA);
    connectWiFi();

    queue = xQueueCreate(4, sizeof(JpegFrame));

    xTaskCreatePinnedToCore(readVideoStream, "video_stream", 8192 * 2, (void *)handleFrame, 2, NULL, 1);
    xTaskCreatePinnedToCore(displayTask, "gfx_show", 8192 * 10, &queue, 1, NULL, 0);
}

void run_loop()
{
    // gfx.fillRect(0, 0, 480, 40, TFT_BLUE);
    // gfx.fillRect(0, 40, 480, 40, TFT_RED);
    // gfx.fillRect(0, 80, 480, 40, TFT_GREEN);
    // gfx.fillRect(0, 120, 480, 40, TFT_CYAN);
    // gfx.fillRect(0, 160, 480, 40, TFT_WHITE);
    // gfx.fillRect(0, 200, 480, 40, TFT_DARKGRAY);
    // gfx.fillRect(0, 240, 480, 40, TFT_YELLOW);
    // gfx.fillRect(0, 280, 480, 40, TFT_PINK);
}