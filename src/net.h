#include <WiFi.h>
#include <HTTPClient.h>

#define WIFI_SSID "Redmi_A2204"
#define WIFI_PASSWORD "18196623393"

void connectWiFi();
void uploadImage(size_t width, size_t height, size_t size, uint8_t *imgData);
void pushData(size_t width, size_t height, size_t size, uint8_t *data);

void connectWiFi()
{
    Serial.println("Connecting to WiFi...");
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.println("Connecting...");
    }

    Serial.printf("Connected to WiFi. IP address: %s\n", WiFi.localIP().toString().c_str());
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