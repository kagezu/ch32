#include "adc.h"

#define CFGR0_PLLMull_Mask ((uint32_t)0x003C0000)

const uint16_t AdcSmpclk[] = {14, 20, 26, 41, 54, 68, 84, 252};
// const uint16_t PllPrescale[] = {104, 112, 120, 128, 144};
uint8_t adc_prescale       = 0;

void ADCCLK(u8 div) {
  switch (div) {
    case 2: RCC_ADCCLKConfig(RCC_PCLK2_Div2); break;
    case 4: RCC_ADCCLKConfig(RCC_PCLK2_Div4); break;
    case 6: RCC_ADCCLKConfig(RCC_PCLK2_Div6); break;
    case 8: RCC_ADCCLKConfig(RCC_PCLK2_Div8); break;
    default: return;
  }
  adc_prescale = div;
}

void RCC_PLL(uint32_t pllmull) {
  uint32_t tmpreg = RCC->CFGR0 & CFGR0_PLLMull_Mask;

  if (pllmull > 17) pllmull = 17;
  if (pllmull < 2) pllmull = 2;
  pllmull = (pllmull - 2) << 18;

  RCC->CFGR0 = tmpreg | pllmull;
}