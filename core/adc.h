#pragma once
#include "core.h"

#define ADC_SMP_0   0  // 1.5
#define ADC_SMP_1   1  // 7.5
#define ADC_SMP_2   2  // 13.5
#define ADC_SMP_3   3  // 28.5
#define ADC_SMP_4   4  // 41.5
#define ADC_SMP_5   5  // 55.5
#define ADC_SMP_6   6  // 71.5
#define ADC_SMP_7   7  // 239.5


#define ADC_GAIN_S    27
#define ADC_DUALMOD_S 16

#define ADC_EXTSEL_S  17


static const uint16_t AdcSmpclk[] = {3 + 22, 15 + 22, 27 + 22, 57 + 22, 83 + 22, 111 + 22, 143 + 22, 479 + 22};

class ADC {
private:
  static uint8_t ch1;

public:
  // constexpr static int SAH_MIN = 0;  // Минимальное время выборки
  // constexpr static int SAH_MAX = 63; // Максимальное время выборки
  constexpr static int DEPTH = 12;  // Разрядность АЦП
  // constexpr static int TIME = 14;    // Время преобразования
  constexpr static int AREF = 3300;  // Опорное напряжение в милливольтах

public:
  // template<const int ch>
  INLINE static void init(const int ch) {
    switch (ch) {
      case 0: ADC_IN0(INA); break;
      case 1: ADC_IN1(INA); break;
      case 2: ADC_IN2(INA); break;
      case 3: ADC_IN3(INA); break;
      case 4: ADC_IN4(INA); break;
      case 5: ADC_IN5(INA); break;
      case 6: ADC_IN6(INA); break;
      case 7: ADC_IN7(INA); break;
    }
    chanel(ch);

    ADC1->CTLR1 =
      (0b00 << ADC_GAIN_S) |       // Усиление 1, 4, 16, 64
      (0b0111 << ADC_DUALMOD_S) |  // Fast mode
      // (0b1000 << ADC_DUALMOD_S) |  // Slow mode
      0;
    ADC1->CTLR2 =
      // ADC_EXTTRIG |            // External trigger
      // (0b111 << ADC_EXTSEL_S) |  // Software trigger
      // ADC_ALIGN | // Left
      // ADC_DMA |
      // ADC_CONT |
      ADC_ADON;

    ADC2->CTLR1 =
      (0b00 << ADC_GAIN_S) |       // Усиление 1, 4, 16, 64
      (0b0111 << ADC_DUALMOD_S) |  // Fast mode
      // (0b1000 << ADC_DUALMOD_S) |  // Slow mode
      0;
    ADC2->CTLR2 =
      // ADC_EXTTRIG |            // External trigger
      // (0b111 << ADC_EXTSEL_S) |  // Software trigger
      // ADC_ALIGN | // Left
      // ADC_DMA |
      // ADC_CONT |
      ADC_ADON;
  }

  // Преобразует число тактов в допустимое SMP
  static int smp(int32_t tick) {
    uint8_t smp = 0;
    while (smp < 7 && tick > AdcSmpclk[smp++]);
    smp--;
    return smp;
  }

  static int cycle(int32_t tick) {
    return AdcSmpclk[smp(tick)];
  }

  INLINE static void delay(int time) {
    ADC1->SAMPTR2 = smp(time) << (ch1 * 3);
  }

  INLINE static void chanel(int ch) {
    ch1 = ch;
    ADC1->RSQR3 = ch;
    ADC2->RSQR3 = ch;
  }

  INLINE static void single() { ADC1->CTLR2 = ADC_ADON; }
  INLINE static void start() { ADC1->CTLR2 = ADC_CONT | ADC_ADON; }
  INLINE static void stop() { ADC1->CTLR2 = ADC_ADON; }  // Снять CONT
  INLINE static void wait() { while (!(ADC1->STATR & ADC_EOC)); }
  INLINE static uint32_t value() { return ADC1->RDATAR; }
};
