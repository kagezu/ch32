#pragma once
#include "core.h"

// Каналы DMA

constexpr int DMA_PERIF = 0;
constexpr int DMA_MEM   = 1;

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

constexpr int DMA_L  = 0;
constexpr int DMA_M  = 1;
constexpr int DMA_H  = 2;
constexpr int DMA_VH = 3;

// Channelx Configuration Register
constexpr int DMA_MEM2MEM = 14;
constexpr int DMA_PL      = 12;
constexpr int DMA_MSIZE   = 10;
constexpr int DMA_PSIZE   = 8;
constexpr int DMA_MINC    = 7;
constexpr int DMA_PINC    = 6;

constexpr int DMA_CIRC = 32;
constexpr int DMA_DIR  = 16;
constexpr int DMA_TEIE = 8;
constexpr int DMA_HTIE = 4;
constexpr int DMA_TCIE = 2;
constexpr int DMA_EN   = 1;

template <const int N, const int P>
class DMA {
protected:
  uint32_t config;

  enum : int {
    BYTE  = 0,
    HALF  = 1,
    WORD  = 2,
    INC   = 1,
    GIF   = 1,  // Появление любого флага
    TCIF  = 2,  // Передача завершина
    HTIF  = 4,  // Половина передачи
    TEIF  = 8,  // Ошибка канала
    ALLIF = 15  // Все / любой
  };

public:
  constexpr DMA_Channel_TypeDef *CH() { return (DMA_Channel_TypeDef *)(AHBPERIPH_BASE + 0x0008 + (N - 1) * 20); }
  constexpr u32 INTF(uc32 flag = ALLIF) { return flag << ((N - 1) << 2); }
  constexpr static uint32_t perif(int size, int inc = 0) { return (size << DMA_PSIZE) | (inc << DMA_PINC); }
  constexpr static uint32_t mem(int size, int inc = 0) { return (size << DMA_MSIZE) | (inc << DMA_MINC); }

  DMA() { RCC->AHBPCENR |= RCC_AHBPeriph_DMA1; }

  // Начать работу
  INLINE void start() {
    DMA1->INTFCR = INTF(ALLIF);
    CH()->CFGR   = config | DMA_EN;
  }

  // Сбросить канал
  INLINE void reset() {
    CH()->CFGR   = 0;
    DMA1->INTFCR = INTF(ALLIF);
  }

  // Ожидание завершения работы канала
  INLINE void wait() { while (!(DMA1->INTFR & INTF(TCIF))); }
  INLINE bool is_active() { return CH()->CFGR & DMA_EN; }
  INLINE void int_compl() {config |= DMA_TCIE; }

  // Установка данных
  template <typename PADD, typename MADD>
  INLINE void setup(PADD padd, MADD madd, uint32_t len) {
    CH()->PADDR = (uint32_t)padd;
    CH()->MADDR = (uint32_t)madd;
    CH()->CNTR  = len;
  }

  // adc -> mem
  void adc(void *dst, uint32_t len) {
    setup(&ADC1->RDATAR, dst, len);
    config = perif(WORD) | mem(HALF, INC) | P << DMA_PL | DMA_CIRC;
  }
};
