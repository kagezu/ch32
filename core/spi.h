#pragma once
#include "core.h"

#define SPI_MODE0 0x00
#define SPI_MODE1 0x01
#define SPI_MODE2 0x02
#define SPI_MODE3 0x03

#define SPIx            ((SPI_TypeDef *)(N == 1 ? SPI1_BASE : SPI2_BASE))

template <const int N>
class SPI {
protected:
  uint32_t config;
  uint32_t FPCLK;

public:
  SPI() {
    // Настройка порта ввода/вывода
    if (N == 1) {
      RCC->APB2PCENR |= RCC_SPI1EN;
      SPI1_NSS(P_NC);
      SPI1_SCK(OUTA);
      SPI1_MISO(IN);
      SPI1_MOSI(OUTA);
      FPCLK = (RCC->CFGR0 & RCC_PPRE2) >> 11;
    }
    if (N == 2) {
      RCC->APB1PCENR |= ((uint32_t)1 << 14);
      SPI2_NSS(P_NC);
      SPI2_SCK(OUTA);
      SPI2_MISO(IN);
      SPI2_MOSI(OUTA);
      FPCLK = (RCC->CFGR0 & RCC_PPRE1) >> 8;
    }
    FPCLK = (FPCLK & 0b11) + (FPCLK >> 2);
    FPCLK = F_CPU >> FPCLK;

    config =
      SPI_MODE0 |
      SPI_CTLR1_MSTR |
      // SPI_CTLR1_SPE |
      //  SPI_CTLR1_LSBFIRST |
      SPI_CTLR1_SSM |  // Software NSS
      SPI_CTLR1_SSI |  // CS pin control
      // SPI_CTLR1_DFF |       // 8/16 bit format
      SPI_CTLR1_BIDIOE |    // Только передача
      SPI_CTLR1_BIDIMODE |  // Полудуплесный режим
      0;

    SPIx->CTLR1 = 0;
    SPIx->CTLR1 = config;
  }

  // Частота в килогерцах
  void fq(uint32_t f) {
    uint8_t baud_rate_div = 0;
    uint32_t max_fq = FPCLK / 2000;
    while (f < max_fq && baud_rate_div < 0x07) {
      max_fq >>= 1;
      baud_rate_div++;
    }
    config = (config & ~SPI_CTLR1_BR) | (baud_rate_div << 3);  // Делитель частоты
  }

  void thr(uint8_t t) {
    config &= ~SPI_CTLR1_DFF;
    if (t == 2) config |= SPI_CTLR1_DFF;
  }
  void mode(uint8_t m = SPI_MODE0) { config = (config & ~SPI_MODE3) | m; }
  void master() { config |= SPI_CTLR1_MSTR; }
  void slave() { config &= ~SPI_CTLR1_MSTR; }

  // Ждать TX
  INLINE static void wait() { while (!(SPIx->STATR & SPI_STATR_TXE)); }
  INLINE static void wait_thr() { while (!(SPIx->STATR & SPI_STATR_TXE)); }
  INLINE static void wait_full() { while (!(SPIx->STATR & SPI_STATR_TXE)); }

  // Ждать завершение пердачи
  INLINE static void wait_idle() { while (SPIx->STATR & SPI_STATR_BSY); }

  INLINE static void send(uint8_t data) {
    wait_thr();
    SPIx->DATAR = data;
  }

  INLINE static void send16(uint16_t data) {
    wait_thr();
    SPIx->DATAR = data;
  }

  INLINE void begin() {
    SPIx->CTLR1 &= ~SPI_CTLR1_SPE;
    SPIx->CTLR1 = config | SPI_CTLR1_SPE;
    SPIx->CTLR1 |= SPI_CTLR1_SPE | SPI_CTLR1_MSTR;
  }

  INLINE static void end() {
    wait_idle();
    SPIx->CTLR1 &= ~SPI_CTLR1_SPE;
  }

  static uint8_t transfer(uint8_t data) {
    SPIx->DATAR = data;
    while (!(SPIx->STATR & SPI_STATR_RXNE));
    return SPIx->DATAR;
  }
};
