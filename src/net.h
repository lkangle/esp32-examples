#include <WiFi.h>
#include <HTTPClient.h>
#include <vector>

#define WIFI_SSID "KL_1603"
#define WIFI_PASSWORD "cbm961122"

void connectWiFi();
void uploadImage(size_t width, size_t height, size_t size, uint8_t *imgData);
void pushData(size_t width, size_t height, size_t size, uint8_t *data);
void readVideoStream();

void connectWiFi()
{
    int count = 0;
    Serial.print("Connecting to WiFi.");
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
        count++;
        if (count > 10)
        {
            Serial.println("\nWiFi Connect Fail.");
            return;
        }
    }

    Serial.printf("\nWiFi Connected. IP address: %s\n", WiFi.localIP().toString().c_str());
}

int findEndFlag(uint8_t *buffer, size_t size)
{
    for (size_t i = 0; i < size - 1; i++)
    {
        if (buffer[i] == 0xFF && buffer[i + 1] == 0xD9)
        {
            return i + 1;
        }
    }
    return -1;
}

void readVideoStream()
{
    if (WiFi.status() != WL_CONNECTED)
    {
        Serial.println("Wifi Not Connected.");
        return;
    }

    HTTPClient http;
    http.begin("http://192.168.1.6:10088/ss.mjpeg");

    int code = http.GET();
    if (code == 200)
    {
        int buffer_size = 8192;
        uint8_t *buffer = (uint8_t *)heap_caps_malloc(buffer_size * sizeof(uint8_t), MALLOC_CAP_DEFAULT);
        std::vector<uint8_t> chunk;

        WiFiClient *sp = http.getStreamPtr();

        long start = millis();
        int count = 0;

        while (http.connected())
        {
            int size = sp->available();
            if (size <= 0)
            {
                continue;
            }

            int c = sp->readBytes(buffer, min(buffer_size, size));

            int index = findEndFlag(buffer, c);
            // 找到Jpeg结束标识
            if (index != -1)
            {
                chunk.insert(chunk.end(), buffer, buffer + index + 1);

                {
                    count++;
                    long end = millis();
                    if (end - start >= 1e3)
                    {
                        Serial.printf("frame rate: %d\n", count);
                        start = end;
                        count = 0;
                    }
                }

                chunk.clear();

                // buffer 剩余部分存起来
                if (index < c - 1)
                {
                    chunk.insert(chunk.end(), buffer + index + 1, buffer + c);
                }
            }
            else
            {
                chunk.insert(chunk.end(), buffer, buffer + c);
            }
        }

        heap_caps_free(buffer);
    }
    else
    {
        Serial.printf("Fail. Code: %d\n", code);
    }

    http.end();
}

void uploadImage(size_t width, size_t height, size_t size, uint8_t *imgData)
{
    if (WiFi.status() != WL_CONNECTED)
    {
        Serial.println("WiFi not connected, can't upload image");
        return;
    }

    HTTPClient client;

    client.begin("http://192.168.31.122:13000/jpg/upload");
    client.addHeader("Content-Type", "image/jpeg");
    client.addHeader("Content-Length", String(size));
    client.addHeader("x-width", String(width));
    client.addHeader("x-height", String(height));

    int httpCode = client.POST(imgData, size);

    if (httpCode > 0)
    {
        Serial.printf("Image uploaded, server response code: %d\n", httpCode);
    }
    else
    {
        Serial.printf("Error uploading image, HTTP error code: %d\n", httpCode);
    }
    client.end();
}

void pushData(size_t width, size_t height, size_t size, uint8_t *data)
{
    if (WiFi.status() != WL_CONNECTED)
    {
        Serial.println("WiFi not connected, can't upload image");
        return;
    }

    HTTPClient client;

    client.begin("http://192.168.31.122:13000/push/data");
    client.addHeader("Content-Type", "image/jpeg");
    client.addHeader("Content-Length", String(size));
    client.addHeader("x-width", String(width));
    client.addHeader("x-height", String(height));

    int httpCode = client.POST(data, size);

    if (httpCode != 200)
    {
        Serial.printf("Error uploading image, HTTP error code: %d\n", httpCode);
    }
    client.end();
}