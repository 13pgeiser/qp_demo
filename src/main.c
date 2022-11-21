/*
 * Copyright 2019 Pascal Geiser, all rights reserved.
 */
#include <qpc.h>
#include <model.h>

#define POOL_ELEMENT_SIZE  (64)
#define POOL_ELEMENT_COUNT (16)

int main(void) {
  static uint8_t poolSto[POOL_ELEMENT_SIZE * POOL_ELEMENT_COUNT] __attribute__((aligned(16)));
  QF_init();
  QF_poolInit(poolSto, sizeof(poolSto), POOL_ELEMENT_SIZE);
  BlinkLeds_Ctor();
  MainApplication_Ctor();
  MainApplication_Start();
  BlinkLeds_Start();
  return QF_run();
}
