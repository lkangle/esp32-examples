#include <WiFi.h>
#include <HTTPClient.h>
#include <vector>

#define WIFI_SSID "CU_Y8w2"
#define WIFI_PASSWORD "sw6pvxmz"

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

void readVideoStream()
{
    if (WiFi.status() != WL_CONNECTED)
    {
        return;
    }

    HTTPClient http;
    http.begin("http://192.168.1.6:10088/ss.mjpeg");

    int code = http.GET();
    if (code == 200)
    {
        int index = 0;
        bool started = false;
        std::vector<uint8_t> bytes;

        WiFiClient c = http.getStream();
        while (c.connected() && c.available() > 0)
        {
            int b = c.read();
            bytes.push_back(b);

            // start
            if (index > 0 && b == 0xD8 && bytes.at(index - 1) == 0xFF)
            {
                started = true;
            }

            // end
            if (started && b == 0xD9 && bytes.at(index - 1) == 0xFF)
            {
                uint8_t *data = bytes.data();
                size_t len = bytes.size();

                Serial.printf("read one frame, size %d\n", len);

                // handleJpeg(data, len);

                bytes.clear();
                index = 0;
                started = false;
                continue;
            }

            index++;
            delay(1);
        }
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