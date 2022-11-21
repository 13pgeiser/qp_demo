/*
 * Copyright 2019 Pascal Geiser, all rights reserved.
 */
#ifndef _BSP_H
#define _BSP_H

#include <stdbool.h>

#define BSP_TICKS_PER_SEC 100U

typedef enum BspColor_e {
  BSP_COLOR_BLUE,
  BSP_COLOR_RED,
  BSP_COLOR_GREEN,
} BspColor_t;

typedef enum BspLed_e {
  BSP_LED_1,
  BSP_LED_2,
} BspLed_t;

void BSP_init(void);
void BSP_setColor(BspColor_t color);
void BSP_led(BspLed_t led, bool enable);

#endif // _BSP_H

