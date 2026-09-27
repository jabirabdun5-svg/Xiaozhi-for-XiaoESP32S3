#include "wifi_board.h"
#include "codecs/no_audio_codec.h"
#include "display/spi_lcd_display.h"
#include "system_reset.h"
#include "application.h"
#include "button.h"
#include "assets/lang_config.h"
#include "led/single_led.h"

#include "board_config.h"  // <- Dari direktori saat ini
#include <wifi_station.h>

#include <esp_log.h>
#include <driver/spi_master.h>
#include <esp_lcd_panel_io.h>
#include <esp_lcd_panel_vendor.h>
#include <esp_lcd_panel_ops.h>
#include <esp_lcd_st7735.h>

#define TAG "SeeedStudioS3WifiBoard"

class SeeedStudioS3WifiBoard : public WifiBoard {
private:
    esp_lcd_panel_io_handle_t panel_io_ = nullptr;
    esp_lcd_panel_handle_t panel_ = nullptr;
    Display* display_ = nullptr;

    Button boot_button_;
    Button touch_button_;
    Button volume_up_button_;
    Button volume_down_button_;

    void InitializeTftDisplay() {
        // Inisialisasi Bus SPI untuk TFT
        spi_bus_config_t buscfg = {};
        buscfg.mosi_io_num = DISPLAY_PIN_MOSI;
        buscfg.miso_io_num = -1;
        buscfg.sclk_io_num = DISPLAY_PIN_SCLK;
        buscfg.quadwp_io_num = -1;
        buscfg.quadhd_io_num = -1;
        buscfg.max_transfer_sz = DISPLAY_WIDTH * DISPLAY_HEIGHT * sizeof(uint16_t);
        ESP_ERROR_CHECK(spi_bus_initialize(DISPLAY_SPI_HOST, &buscfg, SPI_DMA_CH_AUTO));

        // Panel IO handle
        esp_lcd_panel_io_spi_config_t io_config = {};
        io_config.cs_gpio_num = DISPLAY_PIN_CS;
        io_config.dc_gpio_num = DISPLAY_PIN_DC;
        io_config.spi_mode = 0;
        io_config.pclk_hz = 40 * 1000 * 1000;
        io_config.trans_queue_depth = 10;
        io_config.lcd_cmd_bits = 8;
        io_config.lcd_param_bits = 8;
        
        ESP_ERROR_CHECK(esp_lcd_new_panel_io_spi((esp_lcd_spi_bus_handle_t)DISPLAY_SPI_HOST, &io_config, &panel_io_));

        // Konfigurasi Driver Panel ST7735 untuk TFT 128x160
        esp_lcd_panel_dev_config_t panel_config = {};
        panel_config.reset_gpio_num = DISPLAY_PIN_RST;
        panel_config.rgb_endian = LCD_RGB_ENDIAN_BGR;
        panel_config.bits_per_pixel = 16;

        ESP_ERROR_CHECK(esp_lcd_new_panel_st7735(panel_io_, &panel_config, &panel_));

        ESP_ERROR_CHECK(esp_lcd_panel_reset(panel_));
        ESP_ERROR_CHECK(esp_lcd_panel_init(panel_));
        ESP_ERROR_CHECK(esp_lcd_panel_disp_on_off(panel_, true));

        // Buat objek Display TFT
        display_ = new SpiLcdDisplay(
            panel_io_,
            panel_,
            DISPLAY_PIN_BCKL,
            DISPLAY_WIDTH,
            DISPLAY_HEIGHT,
            DISPLAY_MIRROR_X,
            DISPLAY_MIRROR_Y,
            DISPLAY_SWAP_XY
        );
    }

    void InitButtons() {
        touch_button_.OnPressDown([this]() {
            Application::GetInstance().StartListening();
        });
        touch_button_.OnPressUp([this]() {
            Application::GetInstance().StopListening();
        });
    }

public:
    SeeedStudioS3WifiBoard() :
        boot_button_(BOOT_BUTTON_GPIO),
        touch_button_(TOUCH_BUTTON_GPIO),
        volume_up_button_(VOLUME_UP_BUTTON_GPIO),
        volume_down_button_(VOLUME_DOWN_BUTTON_GPIO) {

        ESP_LOGI(TAG, "Init SeeedStudioS3WifiBoard with TFT Display");
        InitializeTftDisplay();
        InitButtons();
    }

    virtual Led* GetLed() override {
        static SingleLed led(BUILTIN_LED_GPIO);
        return &led;
    }

    virtual AudioCodec* GetAudioCodec() override {
#ifndef AUDIO_I2S_METHOD_SIMPLEX
        static NoAudioCodecDuplex audio_codec(
            AUDIO_INPUT_SAMPLE_RATE,
            AUDIO_OUTPUT_SAMPLE_RATE,
            AUDIO_I2S_GPIO_BCLK,
            AUDIO_I2S_GPIO_WS,
            AUDIO_I2S_GPIO_DOUT,
            AUDIO_I2S_GPIO_DIN
        );
#else
        static NoAudioCodecSimplex audio_codec(
            AUDIO_INPUT_SAMPLE_RATE,
            AUDIO_OUTPUT_SAMPLE_RATE,
            AUDIO_I2S_SPK_GPIO_BCLK,
            AUDIO_I2S_SPK_GPIO_LRCK,
            AUDIO_I2S_SPK_GPIO_DOUT,
            AUDIO_I2S_MIC_GPIO_SCK,
            AUDIO_I2S_MIC_GPIO_WS,
            AUDIO_I2S_MIC_GPIO_DIN
        );
#endif
        return &audio_codec;
    }

    virtual Display* GetDisplay() override {
        return display_;
    }
};

// Register this board for system use.
DECLARE_BOARD(SeeedStudioS3WifiBoard);
