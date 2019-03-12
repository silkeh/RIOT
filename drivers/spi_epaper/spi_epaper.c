/*
 * Copyright (C) 2018 Silke Hofstra
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
 * @brief       Device driver implementation for the spi_epaper display controller
 *
 * @author      Silke Hofstra <silke@slxh.eu>
 */
#include <string.h>
#include "byteorder.h"
#include "spi_display.h"

#include "spi_epaper.h"
#include "spi_epaper_params.h"
#include "spi_epaper_internal.h"

#define ENABLE_DEBUG (1)
#include "debug.h"

int spi_epaper_init(spi_epaper_t *dev,
                    const spi_display_params_t *params,
                    const spi_epaper_controller_t *controller,
                    uint8_t size_x, uint16_t size_y, spi_epaper_entry_mode_t entry_mode)
{
    memcpy(&dev->params, params, sizeof(spi_display_params_t));
    memcpy(&dev->controller, controller, sizeof(spi_display_params_t));
    dev->size_x = size_x;
    dev->size_y = size_y;
    dev->entry_mode = entry_mode;
    dev->params.dummy = false;

    return spi_display_init(&dev->params);
}

void spi_epaper_display_init(spi_epaper_t *dev)
{
    le_uint16_t y_data[2] = {0};
    uint8_t y_size;

    if (dev->controller.size_y <= 255) {
        y_data[0].u8[0] = dev->size_y - 1;
        y_size = 2;
    } else {
        y_data[0] = byteorder_btols(byteorder_htons((dev->size_y - 1) & 0x01FF));
        y_size = 3;
    };
    spi_display_write_cmd(&dev->params, SPI_EPAPER_CMD_DRIVER_OUTPUT_CONTROL, (uint8_t *)y_data, y_size);

    uint8_t data[] = {
        0xD7, /* Phase 1: 30 ms phase, sel 3, 6.58 us off */
        0xD6, /* Phase 2: 30 ms phase, sel 3, 3.34 us off */
        0x9D, /* Phase 3: 10 ms phase, sel 4, 1.54 us off */
    };
    spi_display_write_cmd(&dev->params, SPI_EPAPER_CMD_BOOSTER_SOFT_START_CONTROL, data, 3);

    data[0] = dev->controller.vcom;
    spi_display_write_cmd(&dev->params, SPI_EPAPER_CMD_WRITE_VCOM_REGISTER, data, 1);

    data[0] = 0x1A; /* 4 dummy line per gate */
    spi_display_write_cmd(&dev->params, SPI_EPAPER_CMD_SET_DUMMY_LINE_PERIOD, data, 1);

    data[0] = 0x08; /* 2 µs per line */
    spi_display_write_cmd(&dev->params, SPI_EPAPER_CMD_SET_GATE_LINE_WIDTH, data, 1);

    data[0] = (uint8_t)dev->entry_mode;
    spi_display_write_cmd(&dev->params, SPI_EPAPER_CMD_DATA_ENTRY_MODE_SETTING, data, 1);
}

void spi_epaper_activate(spi_epaper_t *dev)
{
    uint8_t data[] = { 0xC0 }; /* Enable Clock Signal, Enable CP */

    spi_display_write_cmd(&dev->params, SPI_EPAPER_CMD_DISPLAY_UPDATE_CONTROL_2, data, sizeof data);
    spi_display_write_cmd(&dev->params, SPI_EPAPER_CMD_MASTER_ACTIVATION, NULL, 0);
    spi_display_wait(&dev->params, SPI_EPAPER_WAIT_ACTIVATION);
}

void spi_epaper_deactivate(spi_epaper_t *dev)
{
    uint8_t data[] = { 0x03 }; /* Disable CP, Disable Clock Signal */

    spi_display_write_cmd(&dev->params, SPI_EPAPER_CMD_DISPLAY_UPDATE_CONTROL_2, data, sizeof data);
    spi_display_write_cmd(&dev->params, SPI_EPAPER_CMD_MASTER_ACTIVATION, NULL, 0);
    spi_display_wait(&dev->params, SPI_EPAPER_WAIT_ACTIVATION);
}

void spi_epaper_init_full(spi_epaper_t *dev)
{
    spi_epaper_display_init(dev);
    spi_epaper_set_area(dev, 0, 200, 0, 200);
    spi_display_write_cmd(&dev->params, SPI_EPAPER_CMD_WRITE_LUT_REGISTER,
                          dev->controller.lut_full, dev->controller.lut_size);
}

void spi_epaper_update_full(spi_epaper_t *dev)
{
    uint8_t data[] = { 0xC4 }; /* Enable Clock Signal, Enable CP, Display Pattern */

    spi_display_write_cmd(&dev->params, SPI_EPAPER_CMD_DISPLAY_UPDATE_CONTROL_2, data, sizeof data);
    spi_display_write_cmd(&dev->params, SPI_EPAPER_CMD_MASTER_ACTIVATION, NULL, 0);
    spi_display_wait(&dev->params, SPI_EPAPER_WAIT_UPDATE_FULL);
    spi_display_write_cmd(&dev->params, SPI_EPAPER_CMD_NOP, NULL, 0);
}

void spi_epaper_init_part(spi_epaper_t *dev)
{
    spi_epaper_display_init(dev);
    spi_epaper_set_area(dev, 0, 200, 0, 200);
    spi_display_write_cmd(&dev->params, SPI_EPAPER_CMD_WRITE_LUT_REGISTER,
                          dev->controller.lut_part, dev->controller.lut_size);
}

void spi_epaper_update_part(spi_epaper_t *dev)
{
    uint8_t data[] = { 0x04 }; /* Display Pattern */

    spi_display_write_cmd(&dev->params, SPI_EPAPER_CMD_DISPLAY_UPDATE_CONTROL_2, data, sizeof data);
    spi_display_write_cmd(&dev->params, SPI_EPAPER_CMD_MASTER_ACTIVATION, NULL, 0);
    spi_display_wait(&dev->params, SPI_EPAPER_WAIT_UPDATE_PART);
    spi_display_write_cmd(&dev->params, SPI_EPAPER_CMD_NOP, NULL, 0);
}

void spi_epaper_write_ram(spi_epaper_t *dev)
{
    spi_display_write_cmd(&dev->params, SPI_EPAPER_CMD_WRITE_RAM, NULL, 0);
}

void spi_epaper_clear(spi_epaper_t *dev)
{
    spi_epaper_fill(dev, 0, dev->size_x, 0, dev->size_y, SPI_EPAPER_COLOR_WHITE);
}

void spi_epaper_fill(spi_epaper_t *dev, uint8_t x1, uint8_t x2, uint16_t y1, uint16_t y2, uint8_t color)
{
    spi_epaper_set_area(dev, x1, x2, y1, y2);

    spi_acquire(dev->params.spi, dev->params.cs_pin, SPI_MODE_0, dev->params.spi_clk);
    spi_display_cmd_start(&dev->params, SPI_EPAPER_CMD_WRITE_RAM, true);

    uint16_t size = ((x2 - x1) >> 3) * (y2 - y1);
    for (uint16_t i = 0; i < size - 1; i++) {
        spi_transfer_byte(dev->params.spi, dev->params.cs_pin, true, color);
    }
    spi_transfer_byte(dev->params.spi, dev->params.cs_pin, false, color);

    spi_release(dev->params.spi);
}

void spi_epaper_fill_pixels(spi_epaper_t *dev, uint8_t x1, uint8_t x2, uint16_t y1, uint16_t y2,
                        uint8_t *px)
{
    spi_epaper_set_area(dev, x1, x2, y1, y2);
    spi_epaper_write_buffer(dev, px, (x2 - x1) * (y2 - y1));
}

void spi_epaper_set_area(spi_epaper_t *dev, uint8_t x1, uint8_t x2, uint16_t y1, uint16_t y2)
{
    /* Set X bounds */
    uint8_t x_data[] = {
        (x1 >> 3) & 0x1F,
        ((x2 - 1) >> 3) & 0x1F,
    };
    spi_display_write_cmd(&dev->params, SPI_EPAPER_CMD_SET_RAM_X, x_data, sizeof x_data);

    /* Set Y bounds */
    le_uint16_t y_data[2] = {0};
    uint8_t y_size;
    if (dev->controller.size_y <= 255) {
        y_data[0].u8[0] = y1;
        y_data[0].u8[1] = y2 - 1;
        y_size = 2;
    } else {
        y_data[0] = byteorder_btols(byteorder_htons(y1 & 0x01FF));
        y_data[1] = byteorder_btols(byteorder_htons((y2 - 1) & 0x01FF));
        y_size = 4;
    };
    spi_display_write_cmd(&dev->params, SPI_EPAPER_CMD_SET_RAM_Y, (uint8_t *)y_data, y_size);

    /* Set counters to start positions */
    spi_display_write_cmd(&dev->params, SPI_EPAPER_CMD_SET_RAM_X_ADDR_COUNTER, x_data, 1);
    spi_display_write_cmd(&dev->params, SPI_EPAPER_CMD_SET_RAM_Y_ADDR_COUNTER, (uint8_t *)y_data, 2);
}

void spi_epaper_write_buffer(spi_epaper_t *dev, const uint8_t *buf, size_t len)
{
    spi_display_write_cmd(&dev->params, SPI_EPAPER_CMD_WRITE_RAM, buf, len);
}

void spi_epaper_sleep(spi_epaper_t *dev)
{
    uint8_t data[] = { 0x01 };

    spi_display_write_cmd(&dev->params, SPI_EPAPER_CMD_DEEP_SLEEP_MODE, data, 1);
}

void spi_epaper_wake(spi_epaper_t *dev)
{
    /* Give a low pulse on the reset pin */
    if (dev->params.rst_pin != GPIO_UNDEF) {
        gpio_clear(dev->params.rst_pin);
        xtimer_usleep(SPI_EPAPER_WAIT_RESET);
        gpio_set(dev->params.rst_pin);
    }

    /* Turn off sleep mode */
    uint8_t data[] = { 0x00 };
    spi_display_write_cmd(&dev->params, SPI_EPAPER_CMD_DEEP_SLEEP_MODE, data, 1);
    spi_display_wait(&dev->params, SPI_EPAPER_WAIT_RESET);
}

void spi_epaper_swreset(spi_epaper_t *dev)
{
    spi_display_write_cmd(&dev->params, SPI_EPAPER_CMD_SWRESET, NULL, 0);
}
