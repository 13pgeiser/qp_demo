/*
 * Copyright 2019 Pascal Geiser, all rights reserved.
 */
#include <stdio.h>
#include <qpc.h>
#include <bsp.h>
#include <model.h>

Q_DEFINE_THIS_FILE

void BSP_init(void) {
  fprintf(stderr, "BSP_init\n");
}

void BSP_setColor(BspColor_t color) {
  switch (color) {
    case BSP_COLOR_BLUE:
      fprintf(stderr, "BSP_setColor(BLUE)\n");
      break;
    case BSP_COLOR_RED:
      fprintf(stderr, "BSP_setColor(RED)\n");
      break;
    case BSP_COLOR_GREEN:
      fprintf(stderr, "BSP_setColor(GREEN)\n");
      break;
    default:
      Q_ERROR();
  }
}
void BSP_led(BspLed_t led, bool enable) {
  switch (led) {
    case BSP_LED_1:
      fprintf(stderr, "BSP_led(LED_1, %s)\n", enable ? "true" : "false");
      break;
    case BSP_LED_2:
      fprintf(stderr, "BSP_led(LED_2, %s)\n", enable ? "true" : "false");
      break;
    default:
      Q_ERROR();
  }
}

void Q_onAssert(char const * const module, int loc) {
  fprintf(stderr, "Assert: %s -> %d\n", module, loc);
  for(;;) {}
}

void QF_onStartup(void) {
  fprintf(stderr, "QF_onStartup\n");
  QF_setTickRate(100, 0);
  QF_consoleSetup();
}

void QF_onCleanup(void) {
  fprintf(stderr, "QF_onCleanup\n");
  QF_consoleCleanup();
}

void QF_onClockTick(void) {
  QF_TICK_X(0U, (void *)0);  /* perform the QF clock tick processing */
  int key = QF_consoleGetKey();
  if (key == 0) {
	  return;
  } else if (key == 32) { // space bar
	    MainApplication_ButtonPress();
  } else if ( (key == 27) || (key == 113) ) {
	  QF_stop();
  } else {
	  fprintf(stderr, "Key: %d\n", key);
  }
}
