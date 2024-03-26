
#define LGFX_USE_V1
#include <LovyanGFX.hpp>

class LGFX : public lgfx::LGFX_Device
{

  lgfx::Panel_ST7789 _panel_instance;
  lgfx::Bus_SPI _bus_instance; // SPIバスのインスタンス
  lgfx::Light_PWM _light_instance;

public:
  LGFX(void)
  {
    {                                    // 配置总线控制设置。
      auto cfg = _bus_instance.config(); // 获得总线配置的结构。

      // SPI设定
      cfg.spi_host = VSPI_HOST;  // 选择要使用的SPI ESP32-S2,C3 : SPI2_HOST 或 SPI3_HOST / ESP32 : VSPI_HOST 或 HSPI_HOST
                                 // * 随着ESP-IDF版本的升级，VSPI_HOST , HSPI_HOST的描述被废弃了，所以如果发生错误，请使用SPI2_HOST ,
      cfg.spi_mode = 0;          // 设置SPI通信模式(0 ~ 3)
      cfg.freq_write = 80000000; // 发送时的SPI时钟（最大80MHz，四舍五入为80MHz的整数）。
      cfg.freq_read = 6000000;   // 接收时的SPI时钟
      cfg.spi_3wire = false;     // 如果用MOSI引脚进行接收，则设置为true
      cfg.use_lock = true;       // 如果使用交易锁则设置为true

      cfg.dma_channel = SPI_DMA_CH_AUTO; // 设置要使用的DMA通道（0=不使用DMA/1=1ch/2=ch/SPI_DMA_CH_AUTO=auto设置）
                                         // 随着ESP-IDF版本的升级，现在推荐使用SPI_DMA_CH_AUTO（自动设置）作为DMA通道，1ch和2ch被弃用。

      cfg.pin_sclk = 18; // 设置SPI SCLK引脚编号
      cfg.pin_mosi = 23; // 设置SPI的MOSI引脚编号
      cfg.pin_miso = -1; // 设置SPI的MISO针脚编号（-1 = 禁用）。
      cfg.pin_dc = 13;   // 设置SPI的D/C针脚编号（-1 = 禁用）。
                         // 当与SD卡共同使用SPI总线时，必须无遗漏地设置MISO。

      _bus_instance.config(cfg);              // //反映总线上的配置值。
      _panel_instance.setBus(&_bus_instance); /// 设置屏幕总线。
    }

    {                                      // バックライト制御の設定を行います。（必要なければ削除）
      auto cfg = _light_instance.config(); // バックライト設定用の構造体を取得します。

      cfg.pin_bl = 12;     // バックライトが接続されているピン番号
      cfg.invert = false;  // バックライトの輝度を反転させる場合 true
      cfg.freq = 44100;    // バックライトのPWM周波数
      cfg.pwm_channel = 7; // 使用するPWMのチャンネル番号

      _light_instance.config(cfg);
      _panel_instance.setLight(&_light_instance); // バックライトをパネルにセットします。
    }

    {                                      // 配置显示面板控制设置。
      auto cfg = _panel_instance.config(); // 获取屏幕配置的结构。。

      cfg.pin_cs = 5;    // 连接CS的引脚编号（-1 = 禁用）。
      cfg.pin_rst = 14;  // 连接RST的引脚编号 (-1 = 禁用)
      cfg.pin_busy = -1; // 连接BUSY的引脚编号 (-1 = 禁用)

      // * 下面的设置对每个面板都有一般的默认值，如果你对某个项目不确定，可以把它注释出来并试一试。

      cfg.panel_width = 240;    // 实际可显示的宽度
      cfg.panel_height = 240;   // 实际可显示的高度
      cfg.offset_x = 0;         // 在屏幕的X方向上的偏移量
      cfg.offset_y = 0;         // 在屏幕的Y方向上的偏移量
      cfg.offset_rotation = 0;  // 旋转方向的偏移量为0~7（4~7为倒置）。
      cfg.dummy_read_pixel = 8; // 读取像素前的假读位数量
      cfg.dummy_read_bits = 1;  // 读取非像素数据前的虚拟读取位数
      cfg.readable = true;      // 如果可以读取数据，则设置为true。
      cfg.invert = true;        // 设定 是否反色，有些屏幕需要设置这个值才能获取正确的颜色
      cfg.rgb_order = false;    // true 为 RGB false 为 BGR
      cfg.dlen_16bit = false;   // 如果面板在16位并行或SPI中以16位单位传输数据长度，则设置为true。
      cfg.bus_shared = false;   // SDカー如果与SD卡共享总线，则设置为true（总线控制由drawJpgFile等执行）。

      _panel_instance.config(cfg);
    }

    setPanel(&_panel_instance); // 使用するパネルをセットします。
  }
};