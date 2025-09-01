#pragma once
#include "core.h"

#define SPI_MODE0       0x00
#define SPI_MODE1       0x01
#define SPI_MODE2       0x02
#define SPI_MODE3       0x03

#define SPI_TX_THR      0x07
#define SPI_DELAY_DEF   0x00
#define SPI_INT_DISABLE 0x3F

#define SPIx            ((SPI_TypeDef *)(N == 1 ? SPI1_BASE : SPI2_BASE))

template <const int N>
class SPI {
protected:
  uint32_t config;
  uint32_t delay_clk;
  uint32_t FPCLK;

public:
  SPI() {
    // Настройка порта ввода/вывода
    if (N == 1) {
      SPI1_MISO(IN);
      SPI1_MOSI(OUTA);
      SPI1_SCK(OUTA);
      SPI1_NSS(IN);
      FPCLK = (RCC->CFGR0 & RCC_PPRE2) >> 11;
    }
    if (N == 2) {
      SPI2_MISO(IN);
      SPI2_MOSI(OUTA);
      SPI2_SCK(OUTA);
      SPI2_NSS(IN);
      FPCLK = (RCC->CFGR0 & RCC_PPRE1) >> 8;
    }
    FPCLK = F_CPU >> FPCLK;

    config =
      SPI_MODE0 |
      SPI_CTLR1_MSTR |
      SPI_CTLR1_SPE |
      //  SPI_CTLR1_LSBFIRST |
      SPI_CTLR1_SSM |       // Software NSS
      SPI_CTLR1_DFF |       // 8/16 bit format
      SPI_CTLR1_BIDIOE |    // Только передача
      SPI_CTLR1_BIDIMODE |  // Полудуплесный режим
      0;

    fq(0x4000);  // 16 MHz
    SPIx->CTLR1 = 0;
  }

  // Частота в килогерцах
  void fq(uint32_t f) {
    uint8_t baud_rate_div = 0;
    uint16_t max_fq = FPCLK / 2000;
    while (f < max_fq && baud_rate_div < 0x07) {
      max_fq >>= 1;
      baud_rate_div++;
    }
    config = (config & ~SPI_CTLR1_BR) | (baud_rate_div << 3);  // Делитель частоты
  }

  void thr(uint8_t t) {}
  void mode(uint8_t m = SPI_MODE0) { config = (config & ~SPI_MODE3) | m; }
  // void master() { config = (config & ~SPI_CONFIG_MODE_SEL_M) | SPI_CONFIG_MASTER_M; }
  // void slave() { config &= ~SPI_CONFIG_MODE_SEL_M; }
  // void select_cs(uint8_t ss_n) {
  //   config =
  //     (config & ~(SPI_CONFIG_CS_NONE_M | SPI_CONFIG_MANUAL_CS_M)) | (SPI_CONFIG_CS_NONE_M ^ (1 << (SPI_CONFIG_CS_S + ss_n)));
  // }
  // void delay(uint32_t btwn = 0, uint32_t after = 0, uint32_t ini = 0) {
  //   delay_clk =
  //     (ini << SPI_DELAY_INIT_S) | (after << SPI_DELAY_AFTER_S) | (btwn << SPI_DELAY_BTWN_S);
  // }

// Ждать TX
  static void wait() {while (!(SPIx->STATR & SPI_STATR_TXE));}
  ATTR_INLINE static void wait_thr() {  while (!(SPIx->STATR & SPI_STATR_TXE)); }
  ATTR_INLINE static void wait_full() { while (!(SPIx->STATR & SPI_STATR_TXE)); }
  // Ждать завершение пердачи
  ATTR_INLINE static void wait_idle() { while (SPIx->STATR & SPI_STATR_BSY); }

  ATTR_INLINE static void send(uint8_t data) { SPIx->DATAR = data; }
  ATTR_INLINE static void send16(uint16_t data) {
    // SPIx->TXDATA = data >> 8;
    // wait_thr();
    SPIx->TXDATA = data;
  }

  // ATTR_INLINE void begin() {
  //   clear_fifo();
  //   SPIx->CONFIG = config;
  //   // SPIx->TX_THR = tx_thr;
  //   // SPIx->DELAY = delay_clk;
  //   SPIx->ENABLE = SPI_ENABLE_M;  // Включение модуля
  // }

  // ATTR_INLINE static void end() {
  //   wait_idle();
  //   SPIx->ENABLE = 0;
  // }

  // static uint8_t transfer(uint8_t data) {
  //   SPIx->TXDATA = data;
  //   wait_idle();
  //   return SPIx->RXDATA;
  // }

  // static uint16_t transfer16(uint16_t data) {
  //   uint16_t rx_dbyte;
  //   SPIx->TXDATA = data >> 8;
  //   SPIx->TXDATA = data;
  //   wait_idle();
  //   rx_dbyte = SPIx->RXDATA << 8;
  //   rx_dbyte |= SPIx->RXDATA;
  //   return rx_dbyte;
  // }
};
