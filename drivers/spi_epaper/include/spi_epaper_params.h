/*
 * Copyright (C) 2019 Silke Hofstra
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

/**
 * @ingroup     drivers_spi_epaper
 *
 * @{
 * @file
 * @brief       Default configuration for spi_epaper
 *
 * @author      Silke Hofstra <silke@slxh.eu>
 */

#ifndef SPI_EPAPER_PARAMS_H
#define SPI_EPAPER_PARAMS_H

#include "board.h"
#include "spi_epaper.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @name    Set default configuration parameters for a SPI e-Paper display
 * @{
 */
#ifndef SPI_EPAPER_PARAM_SPI
#define SPI_EPAPER_PARAM_SPI        (SPI_DEV(0))
#endif
#ifndef SPI_EPAPER_PARAM_SPI_CLK
#define SPI_EPAPER_PARAM_SPI_CLK    (SPI_CLK_5MHZ)
#endif
#ifndef SPI_EPAPER_PARAM_CS
#define SPI_EPAPER_PARAM_CS         (GPIO_UNDEF)
#endif
#ifndef SPI_EPAPER_PARAM_DC
#define SPI_EPAPER_PARAM_DC         (GPIO_UNDEF)
#endif
#ifndef SPI_EPAPER_PARAM_RST
#define SPI_EPAPER_PARAM_RST        (GPIO_UNDEF)
#endif
#ifndef SPI_EPAPER_PARAM_BUSY
#define SPI_EPAPER_PARAM_BUSY       (GPIO_UNDEF)
#endif

#ifndef SPI_EPAPER_PARAMS
#define SPI_EPAPER_PARAMS           { .spi = SPI_EPAPER_PARAM_SPI, \
                                      .spi_clk = SPI_EPAPER_PARAM_SPI_CLK, \
                                      .cs_pin = SPI_EPAPER_PARAM_CS, \
                                      .dc_pin = SPI_EPAPER_PARAM_DC, \
                                      .rst_pin = SPI_EPAPER_PARAM_RST, \
                                      .busy_pin = SPI_EPAPER_PARAM_BUSY, \
                                    }
#endif
/**@}*/

/**
 * @brief   SPI configuration of display
 */
static const spi_display_params_t spi_epaper_params[] =
{
    SPI_EPAPER_PARAMS,
};

/**
 * @name    Waveform lookup table for a full display refresh for IL3829.
 */
static const uint8_t spi_epaper_il3829_lut_default_full[] = {
    0x50, 0xAA, 0x55, 0xAA, 0x11, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0xFF, 0xFF, 0x1F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

/**
 * @name    Waveform lookup table for a partial display refresh for IL3829.
 */
static const uint8_t spi_epaper_il3829_lut_default_part[] = {
    0x10, 0x18, 0x18, 0x08, 0x18, 0x18, 0x08, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x13, 0x14, 0x44, 0x12, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

/**
 * @name    Configuration for IL3829 e-paper display controller
 */
#define SPI_EPAPER_CONTROLLER_IL3829 { \
    .vcom = 0xA8, \
    .size_x = 200, \
    .size_y = 300, \
    .lut_size = sizeof spi_epaper_il3829_lut_default_full, \
    .lut_full = spi_epaper_il3829_lut_default_full, \
    .lut_part = spi_epaper_il3829_lut_default_part, \
}

/**
 * @name    Configuration for SSD1673 e-paper display controller
 */
#define SPI_EPAPER_CONTROLLER_SSD1673 { \
    .vcom = 0xA8, \
    .size_x = 150, \
    .size_y = 250, \
    .lut_size = sizeof spi_epaper_il3829_lut_default_full, \
    .lut_full = spi_epaper_il3829_lut_default_full, \
    .lut_part = spi_epaper_il3829_lut_default_part, \
}

/**
 * @name    Default controller definition
 */
#ifndef SPI_EPAPER_CONTROLLER
#define SPI_EPAPER_CONTROLLER SPI_EPAPER_CONTROLLER_IL3829
#endif

/**
 * @brief   SPI configuration of display
 */
static const spi_epaper_controller_t spi_epaper_controller[] =
{
    SPI_EPAPER_CONTROLLER,
};

#ifdef __cplusplus
}
#endif
#endif /* SPI_EPAPER_PARAMS_H */
