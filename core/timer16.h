#pragma once
#include "timer.h"

constexpr static TIM_TypeDef *T_16(auto N)
{
  switch (N) {
    case 1: return TIM1;
    case 2: return TIM2;
    case 3: return TIM3;
    case 4: return TIM4;
    case 5: return TIM5;
    default: return 0;
  }
}

/*
  ### 16х битный таймер
  #### Регистры:
  + cnt() top() div() ocr([ch]) icr([ch])
  + счётчик предел делитель сравнение захват
  #### Управление:
  + start() stop() single() clear()
  + en(ch) dis(ch) включить/выключить канал
  + inv(on/off, ch) инвертировать канал
  #### Настройка:
  + direct(u32::[DIR, REV, BI]) направление счёта
  + mode[(u32::[CMP, PVM, CAP], ch)] выбрать режим работы канала
  #### Прерывания:
  + int_[ovf, udf, cmp, cap)][(ch, on/off)] установить/снять прерывание
  + [переполнение, обнуление, сравнение, захват]
  + int_[en, dis, clr] () установить / отключить / сброс EPIC
  #### Частоты:
  + osc() fq() частота счётчика/обновления
*/
template <const auto N>
class T16 {
private:
  // Установка режима вывода
  INLINE void OCxREF(uc16 config, uc32 ch)
  {
    if (ch < 3)
      T_16(N)->CHCTLR1 =
      (T_16(N)->CHCTLR1 &
        ~(0xFF << ((ch - 1) << 3))) |
      (config << ((ch - 1) << 3));
    else
      T_16(N)->CHCTLR2 =
      (T_16(N)->CHCTLR2 &
        ~(0xFF << ((ch - 3) << 3))) |
      (config << ((ch - 3) << 3));
  }

  // Прерывания и DMA запросы
  INLINE void INTDMA(uc16 config, uc32 ch)
  {
    T_16(N)->DMAINTENR =
      (T_16(N)->DMAINTENR &
        ~(TIM_INT | TIM_DMA)) |
      ((config & (TIM_INT | TIM_DMA)) << ch);
  }

  // Инверсия вывода
  INLINE void OUTx(uc16 config, uc32 ch)
  {
    T_16(N)->CCER =
      (T_16(N)->CCER &
        ~((TIM_INV | TIM_EN) << ((ch - 1) << 2))) |
      (config << ((ch - 1) << 2));
  }

public:
  T16()
  {
    if (N == 1) RCC->APB2PCENR |= RCC_TIM1EN;
    else RCC->APB1PCENR |= (RCC_TIM2EN << (N - 2));
    T_16(N)->CTLR1 = 0;  // стоп
  }
  // ~T16() {
  //   if (N == 1) RCC->APB2PCENR &= ~RCC_TIM1EN;
  //   else RCC->APB1PCENR &= ~(RCC_TIM2EN << (N - 2));
  // }

  // Регистры

  INLINE u16 CNT() { return T_16(N)->CNT; }
  INLINE void CNT(u16 val) { T_16(N)->CNT = val; }
  INLINE void PSC(u16 val) { T_16(N)->PSC = val; }
  INLINE void TOP(u16 val) { T_16(N)->ATRLR = val; }
  INLINE void REP(u8 val) { T_16(N)->RPTCR = val; }
  INLINE void OCR(u16 val, uc32 ch)
  {
    switch (ch) {
      case 1: T_16(N)->CH1CVR = val; break;
      case 2: T_16(N)->CH2CVR = val; break;
      case 3: T_16(N)->CH3CVR = val; break;
      case 4: T_16(N)->CH4CVR = val; break;
    }
  }
  INLINE u16 ICR(uc32 ch)
  {
    switch (ch) {
      case 1: return T_16(N)->CH1CVR;
      case 2: return T_16(N)->CH2CVR;
      case 3: return T_16(N)->CH3CVR;
      case 4: return T_16(N)->CH4CVR;
    }
  }

  // Общие

  INLINE void cont() { T_16(N)->CTLR1 = (T_16(N)->CTLR1 & ~TIM_OPM) | TIM_CEN; }
  INLINE void single() { T_16(N)->CTLR1 |= TIM_CEN | TIM_OPM; }
  INLINE void enable() { T_16(N)->CTLR1 |= TIM_CEN; }
  INLINE void disable() { T_16(N)->CTLR1 &= ~TIM_CEN; }
  INLINE void clear() { T_16(N)->SWEVGR = TIM_UG; }  // ???

  // Функции таймера

  // Установить TOP OCR, out_enable, enable
  INLINE void compare(uc16 config, uc32 ch)
  {
    INTDMA(config & (TIM_INT | TIM_DMA), ch);  // Прерывания и DMA запросы
    OCxREF(TIM_OVR & config, ch);              // Установка режима вывода
    OUTx((config & TIM_INV) | TIM_EN, ch);     // Инверсия и включение канала
    T_16(N)->CTLR1 &= ~(TIM_ARPE | TIM_UDIS);  // Отключить обновление регистров (ARR, PSC, CHxCVR)
    // T_16(N)->CTLR1 |= TIM_ARPE;
    if (N == 1) T_16(N)->BDTR |= TIM_MOE;  // для TIM1
  }

  // Предварительно установить in_clk TOP OCR
  INLINE void pwm(uc16 config, uc32 ch)
  {
    T_16(N)->SWEVGR = TIM_UG;                                  // Инициализация теневых регистров
    INTDMA(config & (TIM_INT | TIM_DMA), ch);                  // Прерывания и DMA запросы
    OCxREF((TIM_PWM_INV & config) | TIM_PWM | TIM_OC1PE, ch);  // Установка режима вывода
    OUTx((config & TIM_INV) | TIM_EN, ch);                     // Инверсия и включение канала
    T_16(N)->CTLR1 |= TIM_ARPE;                                // Использовать прелоад регистр
    if (N == 1) T_16(N)->BDTR |= TIM_MOE;                      // для TIM1
  }

  INLINE void forced(bool out, uc32 ch = 1)
  {
    if (out) OCxREF(TIM_H, ch);
    else OCxREF(TIM_L, ch);
  }

  INLINE void encoder() { T_16(N)->SMCFGR = TIM_ENC3; }

  // ts = ITR[0, 1, 2, 3] + [4(in)]  sms = 0(CK_INT), 1,2,3(encoder), 5(Gating mode)
  INLINE void slave(uc16 trig)
  {
    T_16(N)->SMCFGR = 0;
    T_16(N)->SMCFGR = trig;
  }
  INLINE void master(uc16 mms) { T_16(N)->CTLR2 |= mms; }

  INLINE void capture(uc16 config, uc32 ch) {}
  INLINE void capture_pwm(uc16 config, uc32 ch) {}

  INLINE void init(uc32 mod) {}

  INLINE void direct(uc16 mod = 0)
  {
    u16 ctlr1 = T_16(N)->CTLR1 & ~(TIM_DIR | TIM_CMS);
    switch (mod) {
      case 0: break;
      case TIM_REV: ctlr1 |= TIM_DIR; break;
      case TIM_BI: ctlr1 |= TIM_CMS; break;
    }
    T_16(N)->CTLR1 = ctlr1;
  }

  // Каналы

  INLINE void enable(uc16 ch) { T_16(N)->CCER |= (u16)1 << ((ch - 1) << 2); }
  INLINE void disable(uc16 ch) { T_16(N)->CCER &= ~((u16)1 << ((ch - 1) << 2)); }

  // Прерывания

  INLINE void int_cleat() { TIM3->INTFR = 0; }
  INLINE void ovf_wait() { while ((TIM3->INTFR & TIM_UIF) == 0); }


  /*
INLINE void int_ovf(const bool on = true) {
  T_32(N)->INT_MASK = on ? T_32(N)->INT_MASK |= TIMER32_INT_OVERFLOW_M : T_32(N)->INT_MASK &= ~TIMER32_INT_OVERFLOW_M;
}
INLINE void int_udf(const bool on = true) {
  T_32(N)->INT_MASK = on ? T_32(N)->INT_MASK |= TIMER32_INT_UNDERFLOW_M : T_32(N)->INT_MASK &= ~TIMER32_INT_UNDERFLOW_M;
}
INLINE void int_cmp(uc32 ch, const bool on = true) {
  T_32(N)->INT_MASK = on ? T_32(N)->INT_MASK |= TIMER32_INT_OC_M(ch) : T_32(N)->INT_MASK &= ~TIMER32_INT_OC_M(ch);
}
INLINE void int_cap(uc32 ch, const bool on = true) {
  T_32(N)->INT_MASK = on ? T_32(N)->INT_MASK |= TIMER32_INT_IC_M(ch) : T_32(N)->INT_MASK &= ~TIMER32_INT_IC_M(ch);
}

INLINE void int_clr() { T_32(N)->INT_CLEAR = -1; }
INLINE void int_en() {
  switch (N) {
    case 0: EPIC->MASK_LEVEL_SET = EPIC_LINE_M(EPIC_LINE_TIMER32_0_S); break;
    case 1: EPIC->MASK_LEVEL_SET = EPIC_LINE_M(EPIC_LINE_TIMER32_1_S); break;
    case 2: EPIC->MASK_LEVEL_SET = EPIC_LINE_M(EPIC_LINE_TIMER32_2_S); break;
  }
}
INLINE void int_dis() {
  switch (N) {
    case 0: EPIC->MASK_LEVEL_CLEAR = EPIC_LINE_M(EPIC_LINE_TIMER32_0_S); break;
    case 1: EPIC->MASK_LEVEL_CLEAR = EPIC_LINE_M(EPIC_LINE_TIMER32_1_S); break;
    case 2: EPIC->MASK_LEVEL_CLEAR = EPIC_LINE_M(EPIC_LINE_TIMER32_2_S); break;
  }
}


  // Установка частоты

  void osc(u32 val) {
    u32 _div = f_in / val;
    if (_div) _div--;
    PSC(_div);
    // f_get();
  }

  void fq(u32 val) {
    u32 _top = f_clk / val;
    if (_top) _top--;
    TOP(_top);
    // f_get();
  }


    void f_get() {
      switch (T_32(N)->CONTROL & TIMER32_CONTROL_CLOCK_M) {
        case TIMER32_CONTROL_CLOCK_PRESCALER_M:
          if (N) f_in = F_CPU / (PM->DIV_APB_P + 1);
          else f_in = F_CPU / (PM->DIV_APB_M + 1);
          break;

        case TIMER32_CONTROL_CLOCK_TIM1_M:
          if ((PM->TIMER_CFG >> (PM_TIMER_CFG_MUX_TIMER32_1_S * N)) & 0b01) f_in = F_CPU;
          else f_in = F_CPU / (PM->DIV_AHB + 1);
          break;

        case TIMER32_CONTROL_CLOCK_TX_PIN_M:
          f_in = 0;
          break;

        case TIMER32_CONTROL_CLOCK_TIM2_M:
          f_in = F_CPU;
          break;
      }

      f_clk = f_in / (T_32(N)->PRESCALER + 1);
      // f_cycle = f_clk / (T_32(N)->TOP + 1);
    }
    */
};

T16<1> tim1;
T16<2> tim2;
T16<3> tim3;
T16<4> tim4;
T16<5> tim5;
