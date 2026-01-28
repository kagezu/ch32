#pragma once
#include "core.h"

#define ADC_DUALMOD_S 16
#define ADC_EXTSEL_S  17
#define ADC_GAIN_S    27

//  (0b00 << ADC_GAIN_S) |  // Усиление 1, 4, 16, 64
//  (0b0111 << ADC_DUALMOD_S) |  // Fast mode
//  (0b1000 << ADC_DUALMOD_S) |  // Slow mode

extern uint8_t adc_prescale;
extern const uint16_t AdcSmpclk[];
// extern const uint16_t PllPrescale[];
void ADCCLK(u8 div);

template <const int N>
class ADC {
private:
  uint8_t chanel;

private:
  constexpr ADC_TypeDef *ADCx() { return (ADC_TypeDef *)(ADC1_BASE + (N - 1) * 0x0400); }

public:
  constexpr static int DEPTH = 12;    // Разрядность АЦП
  constexpr static int AREF  = 3300;  // Опорное напряжение в милливольтах

public:
  ADC(const int ch) {
    chanel = ch;
    if (!adc_prescale) ADCCLK(8);
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
    ADCx()->RSQR3 = ch;
    ADCx()->CTLR1 = ADC_OutputBuffer_Enable;
    ADCx()->CTLR2 =
      ADC_EXTTRIG |                // External trigger
      ADC_ExternalTrigConv_None |  // Software trigger
      ADC_ADON;
  }

  // Преобразует число тактов в допустимое SMP
  static int smp(int32_t tick) {
    uint8_t smp = 7;
    while (smp && tick < (adc_prescale * AdcSmpclk[smp])) smp--;
    return smp;
  }

  static int cycle(int32_t tick) { return adc_prescale * AdcSmpclk[smp(tick)]; }

  INLINE void trigger(uint32_t trig) { ADCx()->CTLR2 = (ADCx()->CTLR2 & ~ADC_ExternalTrigConv_None) | trig; }

  INLINE void int_clear() { ADCx()->STATR = 0; }
  INLINE void int_enable() { ADCx()->CTLR1 |= ADC_EOCIE; }
  INLINE void int_disable() { ADCx()->CTLR1 &= ~ADC_EOCIE; }
  INLINE void dma_enable() { ADCx()->CTLR2 |= ADC_DMA; }

  INLINE void delay(int time) { ADCx()->SAMPTR2 = smp(time) << (chanel * 3); }
  INLINE void gain(u32 gain) { ADCx()->CTLR1 = ADC_OutputBuffer_Enable | (gain << ADC_GAIN_S); }
  INLINE void single() { ADCx()->CTLR2 |= ADC_ADON | ADC_SWSTART; }
  INLINE void start() { ADCx()->CTLR2 |= ADC_CONT | ADC_ADON | ADC_SWSTART; }
  INLINE void stop() { ADCx()->CTLR2 &= ~(ADC_CONT | ADC_SWSTART); }  // Снять CONT
  INLINE void wait() { while (!(ADCx()->STATR & ADC_EOC)); }
  INLINE uint32_t value() { return ADCx()->RDATAR; }
};