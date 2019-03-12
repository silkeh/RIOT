/*
 * Copyright (C) 2019 Silke Hofstra
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

/**
 * @ingroup tests
 * @{
 *
 * @file
 * @brief       Test application for the SPI e-paper display driver
 *
 * @author      Silke Hofstra <silke@slxh.eu>
 * @}
 */


#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "xtimer.h"

#include "spi_epaper.h"
#include "spi_epaper_pictures.h"
#include "spi_epaper_params.h"

const uint16_t riot_32_height = sizeof(riot_logo_32)/sizeof(riot_logo_32[0]);

/* Draw the large RIOT logo with a full refresh */
void draw_riot(spi_epaper_t *dev) {
    spi_epaper_wake(dev);
    spi_epaper_init_full(dev);
    spi_epaper_activate(dev);
    spi_epaper_write_buffer(dev, (uint8_t*)riot_icon_200, sizeof riot_icon_200);
    spi_epaper_update_full(dev);
    spi_epaper_deactivate(dev);
    if (dev->params.rst_pin != GPIO_UNDEF) {
	spi_epaper_sleep(dev);
    }
}

/* Draw a small RIOT logo with a partial refresh */
void draw_small_riot(spi_epaper_t *dev, uint8_t x, uint16_t y) {
    spi_epaper_wake(dev);
    spi_epaper_init_part(dev);
    spi_epaper_activate(dev);
    spi_epaper_set_area(dev, x, x + 32, y, y + riot_32_height);
    spi_epaper_write_buffer(dev, (uint8_t*)riot_logo_32, sizeof riot_logo_32);
    spi_epaper_update_part(dev);
    spi_epaper_deactivate(dev);
    if (dev->params.rst_pin != GPIO_UNDEF) {
	spi_epaper_sleep(dev);
    }
}

int main(void)
{
    spi_epaper_t dev;
    int init = spi_epaper_init(&dev, spi_epaper_params, spi_epaper_controller,
                           DISPLAY_X, DISPLAY_Y, SPI_EPAPER_Y_INC_X_INC);
    if (init != 0) {
        printf("SPI_EPAPER INIT FAILED: %i\n", init);
        return 1;
    }

    xtimer_ticks32_t last_wakeup = xtimer_now();
    while (1) {
        /* Set both RAM buffers to the RIOT logo */
        draw_riot(&dev);
        draw_riot(&dev);
        xtimer_periodic_wakeup(&last_wakeup, 5 * US_PER_SEC);

        /* Draw small RIOT logos on the display */
        for(int y = 0; y < dev.size_y; y+=riot_32_height) {
            for(int x = 0; x < dev.size_x; x+=32) {
                draw_small_riot(&dev, x, y);
                xtimer_periodic_wakeup(&last_wakeup, 2500 * US_PER_MS);
            }
        }
    }

    return 0;
}
