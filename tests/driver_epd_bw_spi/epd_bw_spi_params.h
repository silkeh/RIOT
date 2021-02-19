/**
 * Nucleo F411RE connected to a 1.54 inch Waveshare display,
 * connected as follows:
 *   SCK/D13  -> CLK
 *   MOSI/D11 -> DIN/SDI
 *   CS/D10   -> CS
 *   D9       -> DC
 *   D7       -> RST
 *   D6       -> BUSY
 */
#ifdef BOARD_NUCLEO_F411RE
#define EPD_BW_SPI_DISPLAY_X  (200)
#define EPD_BW_SPI_DISPLAY_Y  (200)
#define EPD_BW_SPI_CONTROLLER EPD_BW_SPI_CONTROLLER_IL3829
#define EPD_BW_SPI_PARAM_CS   GPIO_PIN(PORT_B, 6)
#define EPD_BW_SPI_PARAM_DC   GPIO_PIN(PORT_C, 7)
#define EPD_BW_SPI_PARAM_RST  GPIO_PIN(PORT_A, 8)
#define EPD_BW_SPI_PARAM_BUSY GPIO_PIN(PORT_B, 10)
#endif

#include_next "epd_bw_spi_params.h"
