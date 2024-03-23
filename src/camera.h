#include <Wire.h>
#include <esp_camera.h>

// WROVER-KIT PIN Map
#define CAM_PIN_PWDN 15  // power down is not used
#define CAM_PIN_RESET 42 // software reset will be performed
#define CAM_PIN_XCLK 0
#define CAM_PIN_SDA 10 // 数据
#define CAM_PIN_SCL 9  // 时钟

// 像素相关配置
#define CAM_PIN_D7 8
#define CAM_PIN_D6 7
#define CAM_PIN_D5 6
#define CAM_PIN_D4 5
#define CAM_PIN_D3 4
#define CAM_PIN_D2 3
#define CAM_PIN_D1 2
#define CAM_PIN_D0 1
#define CAM_PIN_VSYNC 46
#define CAM_PIN_HREF 45
#define CAM_PIN_PCLK 41

static camera_config_t camera_config = {
    .pin_pwdn = CAM_PIN_PWDN,
    .pin_reset = CAM_PIN_RESET,
    .pin_xclk = CAM_PIN_XCLK,
    .pin_sccb_sda = CAM_PIN_SDA,
    .pin_sccb_scl = CAM_PIN_SCL,

    .pin_d7 = CAM_PIN_D7,
    .pin_d6 = CAM_PIN_D6,
    .pin_d5 = CAM_PIN_D5,
    .pin_d4 = CAM_PIN_D4,
    .pin_d3 = CAM_PIN_D3,
    .pin_d2 = CAM_PIN_D2,
    .pin_d1 = CAM_PIN_D1,
    .pin_d0 = CAM_PIN_D0,
    .pin_vsync = CAM_PIN_VSYNC,
    .pin_href = CAM_PIN_HREF,
    .pin_pclk = CAM_PIN_PCLK,

    .xclk_freq_hz = 20000000,
    .ledc_timer = LEDC_TIMER_0,
    .ledc_channel = LEDC_CHANNEL_0,

    .pixel_format = PIXFORMAT_RGB565, // YUV422,GRAYSCALE,RGB565,JPEG
    .frame_size = FRAMESIZE_240X240,

    // .jpeg_quality = 5,                 // 0-63 lower number means higher quality
    .fb_count = 4,                     // if fb_count more than one, the driver will work in continuous mode.
    .fb_location = CAMERA_FB_IN_PSRAM, // esp32没有psram，所以只能使用DRAM
    .grab_mode = CAMERA_GRAB_LATEST    // CAMERA_GRAB_LATEST. Sets when buffers should be filled
};
