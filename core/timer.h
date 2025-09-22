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


// Slave Mode Configuration Mode
constexpr uc16        //  T1  T2  T3  T4  T5
  TIM_IT1    = 0x00,  //  T5  T1  T1  T1  T2
  TIM_IT2    = 0x10,  //  T2  USB T2  T2  T3
  TIM_IT3    = 0x20,  //  T3  T3  T4  T3  T4
  TIM_IT4    = 0x30,  //  T4  T4  T5  T8  T8
  TIM_TI1    = 0x40,
  TIM_TI2FP1 = 0x50,
  TIM_TI2FP2 = 0x60,
  TIM_ETRF   = 0x70,

  TIM_ENC1 = 0x01,
  TIM_ENC2 = 0x02,
  TIM_ENC3 = 0x03,
  TIM_RST  = 0x04,
  TIM_GAT  = 0x05,
  TIM_TRGI = 0x06,
  TIM_TCLK = 0x07;

// Master mode selection Источник тригира
constexpr u16
  TIM_EGR    = 0x00,  // Ресет / бит UG
  TIM_CNT_EN = 0x10,  // Постоянно бит CNT_EN
  TIM_OVF    = 0x20,  // Переполнение (использовать таймер как прдделитель)
  TIM_CMP    = 0x30,  // Импульс сравнения/захвата
  TIM_OC1REF = 0x40,  // 
  TIM_OC2REF = 0x50,  // 
  TIM_OC3REF = 0x60,  // 
  TIM_OC4REF = 0x70;  // 


  constexpr u16
  TIM_REV   = 1,
  TIM_BI    = 2;
