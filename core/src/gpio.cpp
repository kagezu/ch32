#include "ch32v20x.h"

void GPIO_Init_Pin(GPIO_TypeDef *GPIOx, GPIOMode_TypeDef GPIO_Mode, uint8_t pin) {
  uint32_t currentmode = 0x00, pos = 0x00;
  uint32_t tmpreg = 0x00, pinmask = 0x00;

  currentmode = GPIO_Mode & 0x0F;
  if (GPIO_Mode & 0x10) currentmode |= GPIO_Speed_50MHz;
  if (GPIOx == GPIOC) pin -= 13;

  if (pin < 8) {
    tmpreg = GPIOx->CFGLR;
    pos = pin << 2;
    pinmask = ((uint32_t)0x0F) << pos;
    tmpreg &= ~pinmask;
    tmpreg |= (currentmode << pos);
    if (GPIO_Mode == GPIO_Mode_IPD) GPIOx->BCR = (((uint32_t)0x01) << pin);
    if (GPIO_Mode == GPIO_Mode_IPU) GPIOx->BSHR = (((uint32_t)0x01) << pin);
    GPIOx->CFGLR = tmpreg;
  } else {
    tmpreg = GPIOx->CFGHR;
    pos = (pin - 8) << 2;
    pinmask = ((uint32_t)0x0F) << pos;
    tmpreg &= ~pinmask;
    tmpreg |= (currentmode << pos);
    if (GPIO_Mode == GPIO_Mode_IPD) GPIOx->BCR = 0x01 << pin;
    if (GPIO_Mode == GPIO_Mode_IPU) GPIOx->BSHR = 0x01 << pin;
    GPIOx->CFGHR = tmpreg;
  }
}