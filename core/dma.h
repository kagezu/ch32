#pragma once
#include "mik32.h"

// Каналы DMA

constexpr int DMA_MEM = 0;

// ADC
constexpr int DMA_ADC1 = 1;

// SPI
constexpr int DMA_SPI1_RX = 2;
constexpr int DMA_SPI1_TX = 3;
constexpr int DMA_SPI2_RX = 4;
constexpr int DMA_SPI2_TX = 5;

// USART
constexpr int DMA_USART1_TX = 4;
constexpr int DMA_USART1_RX = 5;
constexpr int DMA_USART2_RX = 6;
constexpr int DMA_USART2_TX = 7;
constexpr int DMA_USART3_TX = 2;
constexpr int DMA_USART3_RX = 3;
constexpr int DMA_USART4_TX = 1;
constexpr int DMA_USART4_RX = 8;

// I2C
constexpr int DMA_I2C1_TX = 6;
constexpr int DMA_I2C1_RX = 7;
constexpr int DMA_I2C2_TX = 4;
constexpr int DMA_I2C2_RX = 5;

// TIM
constexpr int DMA_TIM1_CH1  = 2;
constexpr int DMA_TIM1_CH2  = 3;
constexpr int DMA_TIM1_CH4  = 4;
constexpr int DMA_TIM1_COM  = 4;
constexpr int DMA_TIM1_TRIG = 4;
constexpr int DMA_TIM1_UP   = 5;
constexpr int DMA_TIM1_CH3  = 6;

constexpr int DMA_TIM2_CH3 = 1;
constexpr int DMA_TIM2_UP  = 2;
constexpr int DMA_TIM2_CH1 = 5;
constexpr int DMA_TIM2_CH2 = 7;
constexpr int DMA_TIM2_CH4 = 7;

constexpr int DMA_TIM3_CH3  = 2;
constexpr int DMA_TIM3_CH4  = 3;
constexpr int DMA_TIM3_UP   = 3;
constexpr int DMA_TIM3_CH1  = 6;
constexpr int DMA_TIM3_TRIG = 6;

constexpr int DMA_TIM4_CH1 = 1;
constexpr int DMA_TIM4_CH2 = 4;
constexpr int DMA_TIM4_CH3 = 5;
constexpr int DMA_TIM4_UP  = 7;

constexpr int DMA_L = 0;
constexpr int DMA_M = 1;
constexpr int DMA_H = 2;
constexpr int DMA_VH = 3;

template <const int N>
class DMA {
protected:
  uint32_t config;

  enum : int { BYTE,
               HALF,
               WORD };
  enum : int { IMM,
               INC };
  enum : int { NONE,
               ACK };

public:
  constexpr DMA_Channel_TypeDef *CH() { return (DMA_Channel_TypeDef *)(AHBPERIPH_BASE + 0x0008 + (N - 1) * 20); }
  constexpr u32 DMA_READY() { return (u32)1 << (N * 4 - 3); }

  constexpr static uint32_t read(int perf, int brust = 0, int even = BYTE, int inc = IMM, int ack = NONE) {
    return (perf == DMA_MEM
              ? DMA_CH_CFG_READ_MODE_MEMORY_M         // Установить режим памяти
              : (perf << DMA_CH_CFG_READ_REQUEST_S))  // Установить периферийную линию
           | (even << DMA_CH_CFG_READ_SIZE_S)         // Разрядность данных
           | (brust << DMA_CH_CFG_READ_BURST_SIZE_S)  // Установить размер пакета
           | (inc << DMA_CH_CFG_READ_INCREMENT_S)     // Установить инкремент
           | (ack << DMA_CH_CFG_READ_ACK_EN_S);       // Установить подтверждение
  }

  constexpr static uint32_t write(int perf, int brust = 0, int even = BYTE, int inc = IMM, int ack = NONE) {
    return (perf == DMA_MEM
              ? DMA_CH_CFG_WRITE_MODE_MEMORY_M         // Установить режим памяти
              : (perf << DMA_CH_CFG_WRITE_REQUEST_S))  // Установить периферийную линию
           | (even << DMA_CH_CFG_WRITE_SIZE_S)         // Разрядность данных
           | (brust << DMA_CH_CFG_WRITE_BURST_SIZE_S)  // Установить размер пакета
           | (inc << DMA_CH_CFG_WRITE_INCREMENT_S)     // Установить инкремент
           | (ack << DMA_CH_CFG_WRITE_ACK_EN_S);       // Установить подтверждение
  }

  // Начать работу
  INLINE void start() { CH()->CFG = config; }
  // Сбросить канал
  INLINE void reset() { CH()->CFG = 0; }

  // Ожидание завершения работы канала
  INLINE void wait() {
    while (!(DMA1->INTFR & DMA_READY())) delay_us(10);
    DMA1->INTFCR = DMA_READY();
  }
  INLINE bool is_active() { return !(DMA1->INTFR & DMA_READY()); }

  // Установка данных
  template <typename D, typename S>
  INLINE void setup(D dst, S src, uint32_t len) {
    CH()->DST = (uint32_t)dst;
    CH()->SRC = (uint32_t)src;
    CH()->LEN = len - 1;
  }

  // mem -> gpio
  // void memout(uint32_t dst, uint32_t src, uint32_t len)
  // {
  // setup(dst, src, len);
  // read(DMA::MEM, WORD, HALF, INC, NONE);
  // write(DMA::MEM, HALF, HALF, IMM, NONE);
  // DMA_CH->CFG = config;
  // }

  // adc -> mem
  void adc(const int line, void *dst, uint32_t len) {
    setup(dst, (uint32_t)&ANALOG_REG->ADC_VALUE, (len >> 2) << 2);
    config = DMA_CH_CFG_ENABLE_M | (P << DMA_CH_CFG_PRIOR_S) | read(line, HALF, HALF, IMM, ACK) | write(DMA_MEM, WORD, WORD, INC);
  }
};
