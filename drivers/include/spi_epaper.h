/*
 * Copyright (C) 2019 Silke Hofstra
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

/**
 * @defgroup    drivers_spi_epaper Generic e-paper/e-ink display driver.
 * @ingroup     drivers_actuators
 * @brief       Device driver for black/white e-ink/e-paper displays.
 *
 * TODO
 *
 * @{
 * @file
 * @brief       Generic black/white e-paper/e-ink display driver.
 *
 * @author      Silke Hofstra <silke@slxh.eu>
 */

#ifndef SPI_EPAPER_H
#define SPI_EPAPER_H

#include "periph/spi.h"
#include "periph/gpio.h"
#include "spi_display.h"

#ifdef __cplusplus
extern "C" {
#endif

#define SPI_EPAPER_COLOR_WHITE (0xFF) /**< White (8x1 pixels) */
#define SPI_EPAPER_COLOR_BLACK (0x00) /**< Black (8x1 pixels) */

/**
 * @brief   Data entry mode settings.
 *
 * This setting affect the automatic increment/decrement of the address counters.
 */
typedef enum {
    SPI_EPAPER_Y_DEC_X_DEC = 0b00, /**< Y decrement, X decrement */
    SPI_EPAPER_Y_DEC_X_INC = 0b01, /**< Y decrement, X increment */
    SPI_EPAPER_Y_INC_X_DEC = 0b10, /**< Y increment, X decrement */
    SPI_EPAPER_Y_INC_X_INC = 0b11, /**< Y increment, X increment */
} spi_epaper_entry_mode_t;

/**
 * @brief   Display controller parameters.
 */
typedef struct {
    uint8_t vcom;       /**< VCOM voltage level */
    const uint16_t size_x;    /**< supported number of horizontal pixels */
    const uint16_t size_y;    /**< supported number of vertical pixels */
    const uint8_t lut_size;   /**< size of the waveform lookup table */
    const uint8_t* lut_full;  /**< lookup table for a full display refresh */
    const uint8_t* lut_part;  /**< lookup table for a partial display refresh */
} spi_epaper_controller_t;

/**
 * @brief   Device initialisation parameters.
 */
typedef struct {
    spi_display_params_t params;        /**< SPI display parameters */
    spi_epaper_controller_t controller; /**< E-paper controller configuration */
    uint8_t size_x;                     /**< number of horizontal pixels in the display */
    uint16_t size_y;                    /**< number of vertical pixels in the display */
    spi_epaper_entry_mode_t entry_mode; /**< data entry mode */
} spi_epaper_t;

/**
 * @brief   Initialise the display.
 *
 * @param[out] dev          Display to initialise.
 * @param[in]  params       SPI Display parameters to use for initialisation.
 * @param[in]  controller   Display controller of the e-Paper display.
 * @param[in]  size_x       Number of horizontal pixels in the display.
 * @param[in]  size_y       Number of vertical pixels in the display.
 * @param[in]  entry_mode   Data entry mode.
 */
int spi_epaper_init(spi_epaper_t *dev,
                    const spi_display_params_t *params,
                    const spi_epaper_controller_t *controller,
                    uint8_t size_x, uint16_t size_y, spi_epaper_entry_mode_t entry_mode);

/**
 * @brief   Activate the display.
 *
 * @param[in] dev   Device descriptor.
 */
void spi_epaper_activate(spi_epaper_t *dev);

/**
 * @brief   Deactivate the display.
 *
 * @param[in] dev   Device descriptor.
 */
void spi_epaper_deactivate(spi_epaper_t *dev);

/**
 * @brief   Initialise the display for a full refresh.
 *
 * @param[in] dev   Device descriptor.
 */
void spi_epaper_init_full(spi_epaper_t *dev);

/**
 * @brief   Update the display with a full refresh.
 *
 * @param[in] dev   Device descriptor.
 */
void spi_epaper_update_full(spi_epaper_t *dev);

/**
 * @brief   Initialise the display for a partial refresh.
 *
 * @param[in] dev   Device descriptor.
 */
void spi_epaper_init_part(spi_epaper_t *dev);

/**
 * @brief   Update the display with a partial refresh.
 *
 * @param[in] dev   Device descriptor.
 */
void spi_epaper_update_part(spi_epaper_t *dev);

/**
 * @brief   Clear the entire display.
 *
 * @param[in] dev   Device descriptor.
 */
void spi_epaper_clear(spi_epaper_t *dev);

/**
 * @brief   Fill an area with a single color.
 *
 * @param[in] dev   Device descriptor
 * @param[in] x1    X coordinate of the first corner (multiple of 8).
 * @param[in] x2    X coordinate of the opposite corner (multiple of 8).
 * @param[in] y1    Y coordinate of the first corner.
 * @param[in] y2    Y coordinate of the opposite corner.
 * @param[in] color Color to use (`spi_epaper_COLOR_BLACK` or `spi_epaper_COLOR_WHITE`)
 */
void spi_epaper_fill(spi_epaper_t *dev, uint8_t x1, uint8_t x2, uint16_t y1, uint16_t y2,
                     uint8_t color);

/**
 * @brief   Fill an area with an array of pixels.
 *
 * Note that the length of the array should be the same as the number of pixels
 * in the given area.
 *
 * @param[in] dev   Device descriptor.
 * @param[in] x1    X coordinate of the first corner (multiple of 8).
 * @param[in] x2    X coordinate of the opposite corner (multiple of 8).
 * @param[in] y1    Y coordinate of the first corner.
 * @param[in] y2    Y coordinate of the opposite corner.
 * @param[in] px    Array of pixels to use.
 */
void spi_epaper_fill_pixels(spi_epaper_t *dev, uint8_t x1, uint8_t x2, uint16_t y1, uint16_t y2,
                            uint8_t *px);

/**
 * @brief   Set the area in which can be drawn.
 *
 * @param[in] dev   Device descriptor.
 * @param[in] x1    X coordinate of the first corner (multiple of 8).
 * @param[in] x2    X coordinate of the opposite corner (multiple of 8).
 * @param[in] y1    Y coordinate of the first corner.
 * @param[in] y2    Y coordinate of the opposite corner.
 */
void spi_epaper_set_area(spi_epaper_t *dev, uint8_t x1, uint8_t x2, uint16_t y1, uint16_t y2);

/**
 * @brief   Write to the RAM of the spi_epaper controller.
 *
 * Together with `spi_epaper_set_area()`, this allows one to draw a pregenerated
 * image on the screen.
 *
 * @param[in] dev   Device descriptor.
 * @param[in] buf   Buffer to write to the display.
 * @param[in] len   Size of the buffer to write to the display.
 */
void spi_epaper_write_buffer(spi_epaper_t *dev, const uint8_t *buf, size_t len);

/**
 * @brief   Set the display to deep sleep mode.
 *
 * After the display has gone to sleep, a wake can be triggered with the reset pin.
 * Do not use this if no reset pin has been defined.
 *
 * @param[in] dev   Device descriptor.
 */
void spi_epaper_sleep(spi_epaper_t *dev);

/**
 * @brief   Wake the device.
 *
 * This doesn't do anything without using the reset pin.
 *
 * @param[in] dev   Device descriptor.
 */
void spi_epaper_wake(spi_epaper_t *dev);

/**
 * @brief   Perform a soft reset of the device.
 *
 * This resets all commands and parameters to their default values,
 * except for sleep mode and the RAM.
 *
 * @param[in] dev   Device descriptor.
 */
void spi_epaper_swreset(spi_epaper_t *dev);

#ifdef __cplusplus
}
#endif
#endif /* SPI_EPAPER_H */
/** @} */
