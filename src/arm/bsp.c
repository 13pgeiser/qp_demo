/*
 * Copyright 2019 Pascal Geiser, all rights reserved.
 */
#include <string.h>
#include <stdio.h>
#include <qpc.h>
#include <stm32f429i_discovery.h>
#include <stm32f429i_discovery_lcd.h>
#include <bsp.h>
#include <model.h>

Q_DEFINE_THIS_FILE

enum KernelAwareISRs {
  SYSTICK_PRIO = QF_AWARE_ISR_CMSIS_PRI, /* see NOTE00 */
  MAX_KERNEL_AWARE_CMSIS_PRI /* keep always last */
};

void Q_onAssert(char const * const module, int loc) {
  BSP_LCD_SetFont(&Font12);
  BSP_LCD_DisplayStringAt(0, 0, (uint8_t*)"Q_onAssert", LEFT_MODE);
  BSP_LCD_DisplayStringAt(0, 16, (uint8_t*)module, LEFT_MODE);
  char line_number[16];
  sniprintf(line_number, sizeof(line_number), "line: %d", loc);
  BSP_LCD_DisplayStringAt(0, 32, (uint8_t*)line_number, LEFT_MODE);
  for(;;) {}
}

void QF_onStartup(void) {
  NVIC_SetPriorityGrouping(0U);
  NVIC_SetPriority(SysTick_IRQn, SYSTICK_PRIO);
}

void QF_onCleanup(void) {
}

void QK_onIdle(void) {
}

static void SystemClock_Config(void) {
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_OscInitTypeDef RCC_OscInitStruct;
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 336;
  RCC_OscInitStruct.PLL.PLLP = 2;
  RCC_OscInitStruct.PLL.PLLQ = 7;
  HAL_RCC_OscConfig(&RCC_OscInitStruct);
  RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;
  HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5);
}

void BSP_init(void) {
  HAL_SetTickFreq(BSP_TICKS_PER_SEC);
  HAL_Init();
  SystemClock_Config();
  SystemCoreClockUpdate();
#if (ART_ACCLERATOR_ENABLE != 0)
  __HAL_FLASH_ART_ENABLE();
#endif /* ART_ACCLERATOR_ENABLE */
  BSP_LED_Init(LED3);
  BSP_LED_Init(LED4);
  BSP_PB_Init(BUTTON_KEY, BUTTON_MODE_EXTI);
  BSP_LCD_Init();
  BSP_LCD_LayerDefaultInit(0, LCD_FRAME_BUFFER);
  BSP_LCD_SelectLayer(0);
  BSP_LCD_DisplayOn();
  BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
  BSP_LCD_SetBackColor(LCD_COLOR_BLACK);
  BSP_LCD_SetFont(&Font12);
  BSP_LCD_Clear(LCD_COLOR_WHITE);
  BSP_LCD_DisplayStringAt(0, 16, (uint8_t*)"Ready", CENTER_MODE);
}

void BSP_setColor(BspColor_t color) {
  uint32_t lcd_color = 0;
  switch (color) {
    case BSP_COLOR_BLUE:
      lcd_color = LCD_COLOR_BLUE;
      break;
    case BSP_COLOR_RED:
      lcd_color = LCD_COLOR_RED;
      break;
    case BSP_COLOR_GREEN:
      lcd_color = LCD_COLOR_GREEN;
      break;
    default:
      Q_ERROR();
  }
  BSP_LCD_Clear(lcd_color);
}

void BSP_led(BspLed_t led, bool enable) {
  Led_TypeDef current_led;
  switch (led) {
    case BSP_LED_1:
      current_led = LED3;
      break;
    case BSP_LED_2:
      current_led = LED4;
      break;
    default:
      Q_ERROR();
  }
  if (enable) {
    BSP_LED_On(current_led);
  } else {
    BSP_LED_Off(current_led);
  }
}
void EXTI0_IRQHandler(void) {
  HAL_GPIO_EXTI_IRQHandler(KEY_BUTTON_PIN);
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
  if (GPIO_Pin == KEY_BUTTON_PIN) {
    MainApplication_ButtonPress();
  }
}

void SysTick_Handler(void) {
  QK_ISR_ENTRY();
  HAL_IncTick();
  QF_TICK_X(0U, (void *)0);
  QK_ISR_EXIT();
}

caddr_t _sbrk(int incr) {
  extern char end asm("end");
  static char* heap_end;
  register char* stack_ptr asm("sp");
  char* prev_heap_end;
  if (heap_end == 0) {
    heap_end = &end;
  }
  prev_heap_end = heap_end;
  if (heap_end + incr > stack_ptr) {
    return (caddr_t) - 1;
  }
  heap_end += incr;
  return (caddr_t) prev_heap_end;
}
