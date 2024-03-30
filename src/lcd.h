#define LGFX_USE_V1
#include <LovyanGFX.hpp>

// 背光
#define PIN_BLO 9
#define TFT_DC 11
#define TFT_CS 12
#define PIN_WR 14
#define PIN_RD 13
#define PIN_RST 10

#define PIN_D0 18
#define PIN_D1 8
#define PIN_D2 17
#define PIN_D3 7
#define PIN_D4 16
#define PIN_D5 6
#define PIN_D6 15
#define PIN_D7 5
#define PIN_D8 41
#define PIN_D9 4
#define PIN_D10 42
#define PIN_D11 3
#define PIN_D12 45
#define PIN_D13 2
#define PIN_D14 46
#define PIN_D15 1

class LGFX : public lgfx::LGFX_Device
{

    lgfx::Panel_ST7796 _panel_instance;
    lgfx::Bus_Parallel16 _bus_instance;
    lgfx::Light_PWM _light_instance;

public:
    LGFX(void)
    {
        {                                      // 配置总线控制设置。
            auto cfg = _bus_instance.config(); // 获得总线配置的结构。

            cfg.freq_write = 40000000;
            cfg.pin_rs = TFT_DC;
            cfg.pin_rd = PIN_RD;
            cfg.pin_wr = PIN_WR;

            cfg.pin_d0 = PIN_D0;
            cfg.pin_d1 = PIN_D1;
            cfg.pin_d2 = PIN_D2;
            cfg.pin_d3 = PIN_D3;
            cfg.pin_d4 = PIN_D4;
            cfg.pin_d5 = PIN_D5;
            cfg.pin_d6 = PIN_D6;
            cfg.pin_d7 = PIN_D7;
            cfg.pin_d8 = PIN_D8;
            cfg.pin_d9 = PIN_D9;
            cfg.pin_d10 = PIN_D10;
            cfg.pin_d11 = PIN_D11;
            cfg.pin_d12 = PIN_D12;
            cfg.pin_d13 = PIN_D13;
            cfg.pin_d14 = PIN_D14;
            cfg.pin_d15 = PIN_D15;

            _bus_instance.config(cfg);              // //反映总线上的配置值。
            _panel_instance.setBus(&_bus_instance); /// 设置屏幕总线。
        }

        {
            auto cfg = _light_instance.config();

            cfg.pin_bl = PIN_BLO;
            cfg.invert = false;
            cfg.freq = 44100;
            cfg.pwm_channel = 7;

            _light_instance.config(cfg);
            _panel_instance.setLight(&_light_instance); // バックライトをパネルにセットします。
        }

        {                                        // 配置显示面板控制设置。
            auto cfg = _panel_instance.config(); // 获取屏幕配置的结构。。

            cfg.pin_cs = TFT_CS;   // 连接CS的引脚编号（-1 = 禁用）。
            cfg.pin_rst = PIN_RST; // 连接RST的引脚编号 (-1 = 禁用)
            cfg.pin_busy = -1;     // 连接BUSY的引脚编号 (-1 = 禁用)

            // * 下面的设置对每个面板都有一般的默认值，如果你对某个项目不确定，可以把它注释出来并试一试。

            cfg.panel_width = 320;    // 实际可显示的宽度
            cfg.panel_height = 480;   // 实际可显示的高度
            cfg.offset_x = 0;         // 在屏幕的X方向上的偏移量
            cfg.offset_y = 0;         // 在屏幕的Y方向上的偏移量
            cfg.offset_rotation = 0;  // 旋转方向的偏移量为0~7（4~7为倒置）。
            cfg.dummy_read_pixel = 8; // 读取像素前的假读位数量
            cfg.dummy_read_bits = 1;  // 读取非像素数据前的虚拟读取位数
            cfg.readable = true;      // 如果可以读取数据，则设置为true。
            cfg.invert = true;        // 设定 是否反色，有些屏幕需要设置这个值才能获取正确的颜色
            cfg.rgb_order = false;    // true 为 RGB false 为 BGR
            cfg.dlen_16bit = true;    // 如果面板在16位并行或SPI中以16位单位传输数据长度，则设置为true。
            cfg.bus_shared = false;   // SDカー如果与SD卡共享总线，则设置为true（总线控制由drawJpgFile等执行）。

            _panel_instance.config(cfg);
        }

        setPanel(&_panel_instance);
    }
};