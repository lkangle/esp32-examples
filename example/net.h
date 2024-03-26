#include <WiFi.h>
#include <HTTPClient.h>

#define WIFI_SSID "Redmi_A2204"
#define WIFI_PASSWORD "18196623393"

void connectWiFi();
void uploadImage(size_t width, size_t height, uint8_t *imgData);

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

void uploadImage(size_t width, size_t height, uint8_t *imgData)
{
    if (WiFi.status() != WL_CONNECTED)
    {
        Serial.println("WiFi not connected, can't upload image");
        return;
    }

    HTTPClient client;

    client.begin("http://192.168.31.122:13000/rgb565/upload");
    client.addHeader("Content-Type", "image/rgb565");
    client.addHeader("x-width", String(width));
    client.addHeader("x-height", String(height));

    int size = width * height * 2;
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
