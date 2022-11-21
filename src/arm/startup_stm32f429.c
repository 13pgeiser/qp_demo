/*
 * Copyright 2017 Pascal Geiser, all rights reserved.
 */

#define DECLARE(name) void name(void) __attribute__ ((interrupt, weak)); \
  void name(void) { for(;;); }

DECLARE(NMI_Handler)
DECLARE(HardFault_Handler)
DECLARE(MemManage_Handler)
DECLARE(BusFault_Handler)
DECLARE(UsageFault_Handler)
DECLARE(SVC_Handler)
DECLARE(DebugMon_Handler)
DECLARE(PendSV_Handler)
// DECLARE(SysTick_Handler)
DECLARE(WWDG_IRQHandler)
DECLARE(PVD_IRQHandler)
DECLARE(TAMP_STAMP_IRQHandler)
DECLARE(RTC_WKUP_IRQHandler)
DECLARE(FLASH_IRQHandler)
DECLARE(RCC_IRQHandler)
DECLARE(EXTI0_IRQHandler)
DECLARE(EXTI1_IRQHandler)
DECLARE(EXTI2_IRQHandler)
DECLARE(EXTI3_IRQHandler)
DECLARE(EXTI4_IRQHandler)
DECLARE(DMA1_Stream0_IRQHandler)
DECLARE(DMA1_Stream1_IRQHandler)
DECLARE(DMA1_Stream2_IRQHandler)
DECLARE(DMA1_Stream3_IRQHandler)
DECLARE(DMA1_Stream4_IRQHandler)
DECLARE(DMA1_Stream5_IRQHandler)
DECLARE(DMA1_Stream6_IRQHandler)
DECLARE(ADC_IRQHandler)
DECLARE(CAN1_TX_IRQHandler)
DECLARE(CAN1_RX0_IRQHandler)
DECLARE(CAN1_RX1_IRQHandler)
DECLARE(CAN1_SCE_IRQHandler)
DECLARE(EXTI9_5_IRQHandler)
DECLARE(TIM1_BRK_TIM9_IRQHandler)
DECLARE(TIM1_UP_TIM10_IRQHandler)
DECLARE(TIM1_TRG_COM_TIM11_IRQHandler)
DECLARE(TIM1_CC_IRQHandler)
DECLARE(TIM2_IRQHandler)
DECLARE(TIM3_IRQHandler)
DECLARE(TIM4_IRQHandler)
DECLARE(I2C1_EV_IRQHandler)
DECLARE(I2C1_ER_IRQHandler)
DECLARE(I2C2_EV_IRQHandler)
DECLARE(I2C2_ER_IRQHandler)
DECLARE(SPI1_IRQHandler)
DECLARE(SPI2_IRQHandler)
DECLARE(USART1_IRQHandler)
DECLARE(USART2_IRQHandler)
DECLARE(USART3_IRQHandler)
DECLARE(EXTI15_10_IRQHandler)
DECLARE(RTC_Alarm_IRQHandler)
DECLARE(OTG_FS_WKUP_IRQHandler)
DECLARE(TIM8_BRK_TIM12_IRQHandler)
DECLARE(TIM8_UP_TIM13_IRQHandler)
DECLARE(TIM8_TRG_COM_TIM14_IRQHandler)
DECLARE(TIM8_CC_IRQHandler)
DECLARE(DMA1_Stream7_IRQHandler)
DECLARE(FMC_IRQHandler)
DECLARE(SDIO_IRQHandler)
DECLARE(TIM5_IRQHandler)
DECLARE(SPI3_IRQHandler)
DECLARE(UART4_IRQHandler)
DECLARE(UART5_IRQHandler)
DECLARE(TIM6_DAC_IRQHandler)
DECLARE(TIM7_IRQHandler)
DECLARE(DMA2_Stream0_IRQHandler)
DECLARE(DMA2_Stream1_IRQHandler)
DECLARE(DMA2_Stream2_IRQHandler)
DECLARE(DMA2_Stream3_IRQHandler)
DECLARE(DMA2_Stream4_IRQHandler)
DECLARE(ETH_IRQHandler)
DECLARE(ETH_WKUP_IRQHandler)
DECLARE(CAN2_TX_IRQHandler)
DECLARE(CAN2_RX0_IRQHandler)
DECLARE(CAN2_RX1_IRQHandler)
DECLARE(CAN2_SCE_IRQHandler)
DECLARE(OTG_FS_IRQHandler)
DECLARE(DMA2_Stream5_IRQHandler)
DECLARE(DMA2_Stream6_IRQHandler)
DECLARE(DMA2_Stream7_IRQHandler)
DECLARE(USART6_IRQHandler)
DECLARE(I2C3_EV_IRQHandler)
DECLARE(I2C3_ER_IRQHandler)
DECLARE(OTG_HS_EP1_OUT_IRQHandler)
DECLARE(OTG_HS_EP1_IN_IRQHandler)
DECLARE(OTG_HS_WKUP_IRQHandler)
DECLARE(OTG_HS_IRQHandler)
DECLARE(DCMI_IRQHandler)
DECLARE(HASH_RNG_IRQHandler)
DECLARE(FPU_IRQHandler)
DECLARE(UART7_IRQHandler)
DECLARE(UART8_IRQHandler)
DECLARE(SPI4_IRQHandler)
DECLARE(SPI5_IRQHandler)
DECLARE(SPI6_IRQHandler)
DECLARE(SAI1_IRQHandler)
DECLARE(LTDC_IRQHandler)
DECLARE(LTDC_ER_IRQHandler)
DECLARE(DMA2D_IRQHandler)

void __attribute__ ((noreturn)) Reset_Handler(void);
extern void HAL_IncTick(void);

/*
void SysTick_Handler(void) __attribute__ ((interrupt, weak));
void SysTick_Handler(void) {
  HAL_IncTick();
}
*/
void __attribute__ ((noreturn)) SysTick_Handler(void);

typedef void (* const pHandler)(void);
extern unsigned int _estack;
__attribute__ ((section(".isr_vector"), used)) pHandler isr_vector[] = {
  (pHandler) &_estack,
  Reset_Handler,
  NMI_Handler,
  HardFault_Handler,
  MemManage_Handler,
  BusFault_Handler,
  UsageFault_Handler,
  0,
  0,
  0,
  0,
  SVC_Handler,
  DebugMon_Handler,
  0,
  PendSV_Handler,
  SysTick_Handler,
  WWDG_IRQHandler,
  PVD_IRQHandler,
  TAMP_STAMP_IRQHandler,
  RTC_WKUP_IRQHandler,
  FLASH_IRQHandler,
  RCC_IRQHandler,
  EXTI0_IRQHandler,
  EXTI1_IRQHandler,
  EXTI2_IRQHandler,
  EXTI3_IRQHandler,
  EXTI4_IRQHandler,
  DMA1_Stream0_IRQHandler,
  DMA1_Stream1_IRQHandler,
  DMA1_Stream2_IRQHandler,
  DMA1_Stream3_IRQHandler,
  DMA1_Stream4_IRQHandler,
  DMA1_Stream5_IRQHandler,
  DMA1_Stream6_IRQHandler,
  ADC_IRQHandler,
  CAN1_TX_IRQHandler,
  CAN1_RX0_IRQHandler,
  CAN1_RX1_IRQHandler,
  CAN1_SCE_IRQHandler,
  EXTI9_5_IRQHandler,
  TIM1_BRK_TIM9_IRQHandler,
  TIM1_UP_TIM10_IRQHandler,
  TIM1_TRG_COM_TIM11_IRQHandler,
  TIM1_CC_IRQHandler,
  TIM2_IRQHandler,
  TIM3_IRQHandler,
  TIM4_IRQHandler,
  I2C1_EV_IRQHandler,
  I2C1_ER_IRQHandler,
  I2C2_EV_IRQHandler,
  I2C2_ER_IRQHandler,
  SPI1_IRQHandler,
  SPI2_IRQHandler,
  USART1_IRQHandler,
  USART2_IRQHandler,
  USART3_IRQHandler,
  EXTI15_10_IRQHandler,
  RTC_Alarm_IRQHandler,
  OTG_FS_WKUP_IRQHandler,
  TIM8_BRK_TIM12_IRQHandler,
  TIM8_UP_TIM13_IRQHandler,
  TIM8_TRG_COM_TIM14_IRQHandler,
  TIM8_CC_IRQHandler,
  DMA1_Stream7_IRQHandler,
  FMC_IRQHandler,
  SDIO_IRQHandler,
  TIM5_IRQHandler,
  SPI3_IRQHandler,
  UART4_IRQHandler,
  UART5_IRQHandler,
  TIM6_DAC_IRQHandler,
  TIM7_IRQHandler,
  DMA2_Stream0_IRQHandler,
  DMA2_Stream1_IRQHandler,
  DMA2_Stream2_IRQHandler,
  DMA2_Stream3_IRQHandler,
  DMA2_Stream4_IRQHandler,
  ETH_IRQHandler,
  ETH_WKUP_IRQHandler,
  CAN2_TX_IRQHandler,
  CAN2_RX0_IRQHandler,
  CAN2_RX1_IRQHandler,
  CAN2_SCE_IRQHandler,
  OTG_FS_IRQHandler,
  DMA2_Stream5_IRQHandler,
  DMA2_Stream6_IRQHandler,
  DMA2_Stream7_IRQHandler,
  USART6_IRQHandler,
  I2C3_EV_IRQHandler,
  I2C3_ER_IRQHandler,
  OTG_HS_EP1_OUT_IRQHandler,
  OTG_HS_EP1_IN_IRQHandler,
  OTG_HS_WKUP_IRQHandler,
  OTG_HS_IRQHandler,
  DCMI_IRQHandler,
  HASH_RNG_IRQHandler,
  FPU_IRQHandler,
  UART7_IRQHandler,
  UART8_IRQHandler,
  SPI4_IRQHandler,
  SPI5_IRQHandler,
  SPI6_IRQHandler,
  SAI1_IRQHandler,
  LTDC_IRQHandler,
  LTDC_ER_IRQHandler,
  DMA2D_IRQHandler,
};

void __attribute__ ((noreturn)) Default_Handler(void) {
  while (1)
    ;
}

extern void SystemInit(void);
extern int main(void);

void __attribute__ ((noreturn)) Reset_Handler(void) {
  extern void* _sidata, *_sdata, *_edata, *_sbss, *_ebss;
  // Initialize Data section
  // cppcheck-suppress comparePointers
  for (void** src = &_sidata, **dst_beg = &_sdata; dst_beg <= &_edata;
       src++, dst_beg++) {
    *dst_beg = *src;
  }
  // Initialize BSS section
  // cppcheck-suppress comparePointers
  for (void**dst_beg = &_sbss; dst_beg <= &_ebss; dst_beg++) {

    *dst_beg = 0;
  }

  SystemInit();

  main();

  while (1)
    ;
}

