#include "FS.h"
#include "SD.h"
#include "SPI.h"
#include <SD_MMC.h>
#include <Arduino.h>

#define D2 7
#define D3 8
#define CMD 9
#define CLK 10
#define D0 11
#define D1 12

#define TEST_FILE_SIZE (4 * 1024 * 1024)

void testFileIO(fs::FS &fs, const char *path)
{
    File file = fs.open(path, FILE_WRITE, true);

    char test_chunk[] = "hello";
    int chunk_size = sizeof(test_chunk);
    int loop = TEST_FILE_SIZE / chunk_size;

    uint32_t start = millis();

    if (file)
    {
        while (loop--)
        {
            if (file.print(test_chunk) <= 0)
            {
                Serial.println("Write Fail.");
                break;
            }
        }
        file.close();

        uint32_t time_used = millis() - start;
        Serial.printf("Write file used: %ld ms, %f KB/s\r\n", time_used, (float)TEST_FILE_SIZE / time_used);
    }
    else
    {
        Serial.println("Failed to open file for writing");
    }

    file = fs.open(path);
    if (!file)
    {
        Serial.println("Failed to open file for reading");
        return;
    }

    start = millis();
    loop = TEST_FILE_SIZE / chunk_size;

    while (loop--)
    {
        if (file.readBytes(test_chunk, chunk_size) <= 0)
        {
            break;
        }
    }
    file.close();

    uint32_t time_used = millis() - start;
    Serial.printf("Read file used: %ld ms, %f KB/s\r\n", time_used, (float)TEST_FILE_SIZE / time_used);
}

void testSPISd()
{
    SPIClass spi(0);
    spi.begin(CLK, D0, CMD, D3);

    if (SD.begin(D3, spi, 20000000) != pdPASS)
    {
        Serial.println("sd init fail.");
        return;
    }

    uint8_t cardType = SD.cardType();

    if (cardType == CARD_NONE)
    {
        Serial.println("No SD card attached");
        return;
    }

    Serial.print("SD Card Type: ");
    if (cardType == CARD_MMC)
    {
        Serial.println("MMC");
    }
    else if (cardType == CARD_SD)
    {
        Serial.println("SDSC");
    }
    else if (cardType == CARD_SDHC)
    {
        Serial.println("SDHC");
    }
    else
    {
        Serial.println("UNKNOWN");
    }

    uint64_t cardSize = SD.cardSize() / (1024 * 1024);
    Serial.printf("SD Card Size: %lluMB\n", cardSize);

    testFileIO(SD, "/abc.txt");
}

void testSDMMC()
{
    if (!SD_MMC.setPins(CLK, CMD, D0))
    {
        Serial.println("SDMMC Pins change failed!");
        return;
    }
    if (!SD_MMC.begin("/sdcard", true))
    {
        Serial.println("sdmmc init fail.");
        return;
    }

    uint8_t cardType = SD_MMC.cardType();

    if (cardType == CARD_NONE)
    {
        Serial.println("No SD_MMC card attached");
        return;
    }

    Serial.print("SD_MMC Card Type: ");
    if (cardType == CARD_MMC)
    {
        Serial.println("MMC");
    }
    else if (cardType == CARD_SD)
    {
        Serial.println("SDSC");
    }
    else if (cardType == CARD_SDHC)
    {
        Serial.println("SDHC");
    }
    else
    {
        Serial.println("UNKNOWN");
    }

    uint64_t cardSize = SD_MMC.cardSize() / (1024 * 1024);
    Serial.printf("SD_MMC Card Size: %lluMB\n", cardSize);

    testFileIO(SD_MMC, "/mmc.txt");
}

void setup()
{
    Serial.begin(9600);
    testSDMMC();
}

void loop()
{
}