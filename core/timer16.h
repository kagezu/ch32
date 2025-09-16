#pragma once
#include "timer.h"

constexpr static TIM_TypeDef *T_16(u32 N) {
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
protected:
  u32 f_in;
  u32 f_clk;
  // u32 f_cycle;

public:
  T16() {
    if (N == 1) RCC->APB2PCENR |= RCC_TIM1EN;
    else RCC->APB1PCENR |= (RCC_TIM2EN << (N - 2));
    T_16(N)->CTLR1 = 0;       // стоп
    T_16(N)->PSC = 0;         // Prescaler
    T_16(N)->ATRLR = 0xffff;  // Top
  }
  ~T16() {
    if (N == 1) RCC->APB2PCENR &= ~RCC_TIM1EN;
    else RCC->APB1PCENR &= ~(RCC_TIM2EN << (N - 2));
  }

  // Регистры

  INLINE u16 CNT() { return T_16(N)->CNT; }
  INLINE void CNT(u16 val) { T_16(N)->CNT = val; }
  INLINE void PSC(u16 val) { T_16(N)->PSC = val; }
  INLINE void TOP(u16 val) { T_16(N)->ATRLR = val; }
  INLINE void REP(u8 val) { T_16(N)->RPTCR = val; }
  INLINE void OCR(u16 val, uc32 ch) { *(&(T_16(N)->RPTCR) + (ch << 1)) = val; }
  INLINE u16 ICR(uc32 ch) { return *(&(T_16(N)->RPTCR) + (ch << 1)); }  // CH1CVR - CH4CVR

  // Общие

  INLINE void cont() { T_16(N)->CTLR1 = (T_16(N)->CTLR1 & ~TIM_OPM) | TIM_CEN; }
  INLINE void single() { T_16(N)->CTLR1 |= TIM_CEN | TIM_OPM; }
  INLINE void enable() { T_16(N)->CTLR1 |= TIM_CEN; }
  INLINE void disable() { T_16(N)->CTLR1 &= ~TIM_CEN; }
  INLINE void clear() { T_16(N)->SWEVGR = TIM_UG; }  // ???

  // Функции таймера

  // Установить in_clk TOP OCR, out_enable, enable
  INLINE void compare(uc16 config, uc32 ch) {
    using namespace TimerConfig;
    T_16(N)->DMAINTENR =  // Прерывания и DMA запросы
      (T_16(N)->DMAINTENR & ~(config & (INT | DMA))) |
      ((config & (INT | DMA)) << ch);

    if (ch < 3)  // Установка режима вывода
      T_16(N)->CHCTLR1 =
        (T_16(N)->CHCTLR1 & ~((TIM_OC1M | TIM_OC1PE | TIM_CC1S) << ((ch - 1) << 3))) |
        ((OVR & config) << ((ch - 1) << 3));
    else
      T_16(N)->CHCTLR1 =
        (T_16(N)->CHCTLR1 & ~((TIM_OC1M | TIM_OC1PE | TIM_CC1S) << ((ch - 3) << 3))) |
        ((OVR & config) << ((ch - 3) << 3));

    T_16(N)->CCER |= (config & INV) << ((ch - 1) << 2);  // Инверсия
    T_16(N)->CTLR1 &= ~TIM_ARPE;                          // Отключить прелоад регистр
    if (N == 1) T_16(N)->BDTR |= TIM_MOE;                // для TIM1
  }

  // Предварительно установить in_clk TOP OCR
  INLINE void pwm(uc16 config, uc32 ch) {
    using namespace TimerConfig;
    T_16(N)->= TIM_UG;    // Инициализация теневых регистров
    T_16(N)->DMAINTENR =  // Прерывания и DMA запросы
      (T_16(N)->DMAINTENR & ~(config & (INT | DMA))) |
      ((config & (INT | DMA)) << ch);

    if (ch < 3)  // Установка режима вывода
      T_16(N)->CHCTLR1 =
        (T_16(N)->CHCTLR1 & ~((TIM_OC1M | TIM_CC1S) << ((ch - 1) << 3))) |
        ((PWM | TIM_OC1PE) << ((ch - 1) << 3));
    else
      T_16(N)->CHCTLR1 =
        (T_16(N)->CHCTLR1 & ~((TIM_OC1M | TIM_CC1S) << ((ch - 3) << 3))) |
        ((PWM | TIM_OC1PE) << ((ch - 3) << 3));

    T_16(N)->CCER |= (config & INV) << ((ch - 1) << 2);  // Инверсия
    T_16(N)->CTLR1 |= TIM_ARPE;                          // Использовать прелоад регистр
    if (N == 1) T_16(N)->BDTR |= TIM_MOE;                // для TIM1
  }


  INLINE void init(uc32 mod) {}

  INLINE void direct(uc16 mod) {
    using namespace TimerConfig;
    u16 ctlr1 = T_16(N)->CTLR1 & ~(TIM_DIR | TIM_CMS);
    switch (mod) {
      case DIR: break;
      case REV: ctlr1 |= TIM_DIR; break;
      case BI: ctlr1 |= TIM_CMS; break;
    }
    T_16(N)->CTLR1 = ctlr1;
  }

  // Каналы

  INLINE void out_enable(uc16 ch) { T_16(N)->CCER |= (u16)1 << ((ch - 1) << 2); }
  INLINE void out_disable(uc16 ch) { T_16(N)->CCER &= ~((u16)1 << ((ch - 1) << 2)); }

  INLINE void inv(const bool mod = true, uc32 ch = 1) {
    if (mod) T_16(N)->CCER |= (u16)1 << (((ch - 1) << 2) + 1);
    else T_16(N)->CCER &= ~((u16)1 << (((ch - 1) << 2) + 1));
  }
  INLINE void mode(u32 mod, uc32 ch = 1) {
    using namespace TimerConfig;
    u16 chctlr, sl = ch - 1;
    if (ch < 3) chctlr = T_16(N)->CHCTLR1 & ~((TIM_OC1M | TIM_CC1S) << (sl << 3));
    else {
      sl -= 2;
      chctlr = T_16(N)->CHCTLR2 & ~((TIM_OC3M | TIM_CC3S) << (sl << 3));
    }
    switch (mod) {
      case CMP: chctlr |= (TIM_OC1M_0 | TIM_OC1M_1) << sl; break;  // Инверсия
      case PWM: chctlr |= (TIM_OC1M_1 | TIM_OC1M_2) << sl; break;  // ШИМ 1
      case CAP: chctlr |= TIM_CC1S_0 << sl; break;                 // Захват IC1 => TI1
    }
    if (ch < 3) T_16(N)->CHCTLR1 = chctlr;
    else T_16(N)->CHCTLR2 = chctlr;
    if (ch == 1) T_16(N)->BDTR = TIM_MOE;
  }
  // ts = ITR[0, 1, 2, 3] + [4(in)]  sms = 0(CK_INT), 1,2,3(encoder), 5(Gating mode)
  INLINE void slave(u16 ts, u16 sms) { T_16(N)->SMCFGR = (ts << 4) | sms; }
  // mms = 1(CNT_EN), 4-7(OC1-4)
  INLINE void master(u16 mms) { T_16(N)->CTLR2 = mms << 4; }

  // Прерывания
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
  */

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

  /*
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
