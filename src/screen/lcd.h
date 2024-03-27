#include <Arduino.h>
#include <SPI.h>
#include <Arduino_GFX_Library.h>

typedef struct {
    uint8_t * buf;
    size_t len;
} JpegData;

void displayTask(void *p);