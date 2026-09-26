#ifndef BOARD_CONFIG_H_
#define BOARD_CONFIG_H_

#include <driver/gpio.h>

// Audio rate
#define AUDIO_INPUT_REFERENCE true
#define AUDIO_INPUT_SAMPLE_RATE 16000
#define AUDIO_OUTPUT_SAMPLE_RATE 24000

// Enable Simplex (two bus systems)
#define AUDIO_I2S_METHOD_SIMPLEX

// Speaker (TX, MASTER) - Bus #0
#define AUDIO_I2S_SPK_GPIO_BCLK GPIO_NUM_7
#define AUDIO_I2S_SPK_GPIO_LRCK GPIO_NUM_4
#define AUDIO_I2S_SPK_GPIO_DOUT GPIO_NUM_2

// Microphone (RX, MASTER) - Bus #1
#define AUDIO_I2S_MIC_GPIO_SCK GPIO_NUM_44
#define AUDIO_I2S_MIC_GPIO_WS GPIO_NUM_9
#define AUDIO_I2S_MIC_GPIO_DIN GPIO_NUM_1

// TFT SPI Display (ST7735 / 120x160)
#define DISPLAY_SPI_HOST SPI2_HOST
#define DISPLAY_WIDTH 120
#define DISPLAY_HEIGHT 160

// Safe GPIO pins for TFT SPI (Avoiding audio pins 1, 2, 4, 7, 9, 44)
#define DISPLAY_SPI_MOSI_PIN GPIO_NUM_11
#define DISPLAY_SPI_SCLK_PIN GPIO_NUM_12
#define DISPLAY_TFT_CS_PIN   GPIO_NUM_10
#define DISPLAY_TFT_DC_PIN   GPIO_NUM_13
#define DISPLAY_TFT_RST_PIN  GPIO_NUM_14

#define DISPLAY_MIRROR_X false
#define DISPLAY_MIRROR_Y false
#define DISPLAY_SWAP_XY  false

// Onboard LED
#define BUILTIN_LED_GPIO GPIO_NUM_21

// Buttons
#define BOOT_BUTTON_GPIO GPIO_NUM_0         // Board Boot
#define VOLUME_UP_BUTTON_GPIO GPIO_NUM_3    // D2
#define VOLUME_DOWN_BUTTON_GPIO GPIO_NUM_8  // D9

// The touch keys are not needed for now, but we've provided a placeholder for them to ensure
// compatibility with the code's constructor.
#define TOUCH_BUTTON_GPIO GPIO_NUM_3

#endif  // BOARD_CONFIG
