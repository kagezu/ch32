#pragma once
#include "core.h"

constexpr u16

  // DMA/Interrupt Enable Register

  TIM_INT = 0x0001,  // CCxIE  Прерывание
  TIM_DMA = 0x0100,  // CCxDE  Запрос DMA

  // Compare/Capture Enable Register

  // CCxP   Инвертировать выход
  TIM_INV = 0x0002,
  TIM_EN  = 0x0001,  // Включить вывод

  /* Compare/Capture Control Register */

  TIM_IDLE    = 0x0000,  // OCxM   Вывод не активен
  TIM_OVR     = 0x0030,  // OCxM   Инверсия при сравнении
  TIM_H_CMP   = 0x0010,  // OCxM   При сравнении 1
  TIM_L_CMP   = 0x0020,  // OCxM   При сравнении 0
  TIM_H       = 0x0040,  // OCxM   Принудительно 1
  TIM_L       = 0x0050,  // OCxM   Принудительно 0
  TIM_PWM     = 0x0060,  // OCxM   ШИM
  TIM_PWM_INV = 0x0070;  // OCxM   Инвертированный ШИM

constexpr uc32
  SINGLE = (1 << 3);


constexpr u32
  CMP = 1,
  CAP = 3,
  DIR = 4,
  REV = 5,
  BI  = 6,
  ENC = 7;
