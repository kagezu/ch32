#include "main.h"
#include "lcd.h"
#include "adc.h"
#include "timer.h"
#include "dma.h"
#include "lagrange.h"
#include "encoder.h"
#include "fft.h"
#include "osc.h"

constexpr int ADC_CH   = 3;    // Номер канала ADC
constexpr int INT_FQ   = 200;  // Hz опрос энкодера
constexpr int INFO_FQ  = 5;    // Hz обновление текста
constexpr int L_POINT  = 8;    // Узловых точек для интерполяции Лагранжа (чётная)
constexpr int MAX_STEP = 30;   // Max шаг узлов интерполяции

// Дисплей
LCD lcd;
constexpr int BORDER_TOP    = 66;                                                             // 32;                                                             // Отступ от верха экрана
constexpr int BORDER_BOTTOM = 0;                                                              // Отступ от низа экрана
constexpr int POINTES       = ((lcd.max_x() + 2));                                            // Количество точек для отображения
constexpr int SAMPLES       = POINTES * 3;                                                    // Количество измерений для анализа
constexpr int HEIGHT        = lcd.max_y() - BORDER_BOTTOM - BORDER_TOP;                       // Высота области для граф. данных
constexpr int MIDLE_AXIS    = (lcd.max_y() - BORDER_BOTTOM - (HEIGHT >> 1));                  // Расположение оси X
Rect view                   = Rect(0, BORDER_TOP, lcd.max_x(), lcd.max_y() - BORDER_BOTTOM);  // Графическая область

s16 buffer[SAMPLES];

ADC<1> adc(ADC_CH);
Lagrange<L_POINT, MAX_STEP, adc.DEPTH> L;
FFT<SAMPLES, adc.DEPTH> fft;
Encoder enc;
DMA<DMA_ADC1, DMA_VH> dma;
OSC data(buffer);

volatile int index = 0;
int fps            = 20;
int count          = 0;
int timer          = 0;

uint32_t pix_grid;
uint32_t tick_grid;
uint32_t tick_smp;
uint32_t tick_pix;

///////////////////////////////////////////////////////////////////////////////

extern "C" {
void SysTick_Handler(void) __attribute__((interrupt("WCH-Interrupt-fast")));
void DMA1_Channel1_IRQHandler(void) __attribute__((interrupt("WCH-Interrupt-fast")));
}

// Вывод на экран текстовой информации
void print_info();

// Преобразование к координатам дисплея
void transform_to_display(short in[], short out[], int32_t scale, int32_t offset);

// Поиск окна
void osc_window(int &offset, int &median);

// Заполнение буфера данными из АЦП
void record(uint32_t time, uint32_t len);

// Обрамление графики
void border_draw();
void axis_draw();

// Обновление графика
void data_draw();

void init();

///////////////////////////////////////////////////////////////////////////////

void osc(bool update) {

  // Инициализация режима работы
  if (update) {
    lcd.scroll(0);
    lcd.clear();
    print_info();
    border_draw();
    axis_draw();
  }

  // Обновление фона и информации
  if (!(count = count < INT_FQ / INFO_FQ ? count : 0)) {
    print_info();
    axis_draw();
  }

  pix_grid       = Segment.value;                                     // Шаг сетки
  tick_grid      = (u32)FqScale.get_item<int>() * (F_CPU / 1000000);  // Умножаем микросекунды на X MHz. [такты на сегмент]
  uint32_t scale = ((adc.AREF * pix_grid) / VScale.get_item<int>());  // Q32.12

  data.set_scale(scale);
  data.set_trigger(TType.value);
  data.set_offset(MIDLE_AXIS);

  tick_pix = tick_grid / pix_grid;  // [тактов на пиксель]
  tick_smp = adc.preset(tick_pix);  // Готовим допустимое значение для АЦП. [тактов на выборку]

  if (tick_smp > tick_pix) {        // Использовать интерполяцию
    record(tick_smp, SAMPLES);      // * tick_pix / tick_smp);
    int a = tick_smp;
    int b = tick_pix;
    while (a > MAX_STEP && b > 1) {  // Сокращаем дробь, если большой числитель
      a >>= 1;
      b >>= 1;
    }

    data.set_length(POINTES * tick_pix / tick_smp + L_POINT);
    data.release();

    L.interpolate2(data.get_points(), data.get_buffer(), POINTES, a, b);
    data.set_points();

  } else {
    record(tick_pix, SAMPLES);  // TS = TP  PS = 1 Один семпл на пиксель
    data.set_length(POINTES);
    data.release();
  }

  data_draw();
}


int main(void) {
  init();

  volatile int mode = -1;
  int sl            = 0;

  while (true) {
    fps           = fps - (fps >> 2) + (INT_FQ / timer);  // Расчёт FPS
    timer         = 0;
    pix_grid      = Segment.value;
    tick_grid     = (u32)FqScale.get_item<int>() * (F_CPU / 1000000);  // Умножаем микросекунды на X MHz. [такты на сегмент]
                                                                       // uint32_t scale = (adc.AREF * pix_grid) / VScale.get_item<int>();    // Масштабирование по напряжению [пикселей на весь диапазон]
    int32_t scale = ((adc.AREF * pix_grid) / VScale.get_item<int>());  // Q32.12

    data.set_scale(scale);
    data.set_trigger(TType.value);
    data.set_offset(MIDLE_AXIS);

    // Инициализация режимов работы при переключении
    if (mode != menu.value) {
      mode = menu.value;
      sl   = 0;
      lcd.scroll(0);
      lcd.clear();

      switch (mode) {
        case MODE_OSC:
          print_info();
          border_draw();
          axis_draw();
          // greed_draw();
          break;

        case MODE_FFT:
          print_info();
          border_draw();
          // greed_draw();
          break;

        case MODE_SPEC:
          lcd.viewport();
          break;
      }
    }

    // Обновление фона и информации
    if (!(count = count < INT_FQ / INFO_FQ ? count : 0))
      switch (mode) {
        case MODE_OSC:
          print_info();
          axis_draw();
          // greed_draw();
          break;

        case MODE_FFT:
          print_info();
          // greed_draw();
          break;
      }

    switch (mode) {
      case MODE_OSC: {

        tick_pix = tick_grid / pix_grid;  // [тактов на пиксель]
        tick_smp = adc.preset(tick_pix);  // Готовим допустимое значение для АЦП. [тактов на выборку]

        if (tick_smp > tick_pix) {        // Использовать интерполяцию
          record(tick_smp, SAMPLES);      // * tick_pix / tick_smp);
          int a = tick_smp;
          int b = tick_pix;
          while (a > MAX_STEP && b > 1) {  // Сокращаем дробь, если большой числитель
            a >>= 1;
            b >>= 1;
          }

          data.set_length(POINTES * tick_pix / tick_smp + L_POINT);
          data.release();

          L.interpolate2(data.get_points(), data.get_buffer(), POINTES, a, b);
          data.set_points();

        } else {
          record(tick_pix, SAMPLES);  // TS = TP  PS = 1 Один семпл на пиксель
          data.set_length(POINTES);
          data.release();
        }

        data_draw();
        break;
      }

      case MODE_FFT: {
        record(tick_grid, SAMPLES);
        fft.run(data.get_buffer());                    // Быстрое преобразование Фурье
        if (FType.value) fft.sum();                    // Применяем суммирующий фильтр
        fft.sqrt(data.get_buffer(), lcd.max_x() + 1);  // Находим модуля амплитуд
        transform_to_display(data.get_buffer(), data.get_points(), scale, lcd.max_y() - BORDER_BOTTOM);
        data_draw();
        break;
      }

      case MODE_SPEC: {
        record(tick_grid, SAMPLES);
        fft.run(data.get_buffer());
        fft.sqrt(data.get_buffer(), lcd.max_y() + 2);

        for (int i = 1; i < lcd.max_y() + 2; i++) {
          int val = *(data.get_buffer() + i);
          int r   = val >> 4;
          int g   = val > 1023 ? 255 : val >> 2;
          int b   = val > 255 ? 255 : val;

          lcd.pixel(lcd.max_x() - sl, lcd.max_y() - i, RGB(r, g, b));
        }
        sl = sl < lcd.max_x() ? sl + 1 : 0;
        lcd.scroll(sl);
        break;
      }
    }

    if (enc.is_push()) {
      menu.select();
      if (menu.value != MODE_SPEC) print_info();
      // menu.save((int *)RTC->REG);
      while (enc.is_push());
    }
  }
}

///////////////////////////////////////////////////////////////////////////////

// Преобразование к координатам дисплея
void transform_to_display(short in[], short out[], int32_t scale, int32_t offset) {
  for (int16_t i = 0; i <= POINTES; i++) out[i] = offset - (((in[i]) * scale) >> adc.DEPTH);
}

///////////////////////////////////////////////////////////////////////////////

// Вывод на экран текстовой информации
void print_info() {
  lcd.viewport();
  lcd.color(Aqua);
  lcd.color2(Yellow);
  lcd.printf(
    "\f\1%u\1us \1%u\1mV %S  ",
    FqScale.get_item<int>(),
    VScale.get_item<int>(),
    VType.get_item<char *>());
  lcd.at(lcd.max_x(), 0);
  lcd.printf("\b\b\b\b\b\b\b\b\bFPS %.1.2q ", fps);
  lcd.printf("\f\n");
  menu.print(&lcd);
  lcd.prints("             \n");
  lcd.printf("TG: %lu PG: %u TP: %u TS: %u      \n", tick_grid, pix_grid, tick_pix, tick_smp);
  lcd.printf("adc_prescale: %u   ", adc_prescale);
}

////////////////////////////////// Axis ///////////////////////////////////////

// Обрамление графической области
void border_draw() {
  lcd.viewport();
  lcd.color(LightGreen);
  lcd.w_line(0, view.min_y, lcd.max_x());
  lcd.w_line(0, view.max_y, lcd.max_x());
}

// Сетка
void axis_draw_raw(int segment) {
  lcd.h_line((lcd.max_x() >> 1), BORDER_TOP, lcd.max_y() - BORDER_BOTTOM);
  for (int i = 1; i <= lcd.max_x() / (segment * 2); i++) {
    lcd.h_line((lcd.max_x() >> 1) + i * segment, BORDER_TOP, lcd.max_y() - BORDER_BOTTOM);
    lcd.h_line((lcd.max_x() >> 1) - i * segment, BORDER_TOP, lcd.max_y() - BORDER_BOTTOM);
  }

  lcd.w_line(0, MIDLE_AXIS, lcd.max_x() - 0);
  for (int i = 0; i <= (HEIGHT / segment); i++) {
    lcd.w_line(0, MIDLE_AXIS + i * segment, lcd.max_x());
    lcd.w_line(0, MIDLE_AXIS - i * segment, lcd.max_x());
  }
}

void axis_draw() {
  static int old_seg = 0;
  lcd.viewport(&view);
  if (Segment.value != old_seg) {
    lcd.color(Black);
    axis_draw_raw(old_seg);
    old_seg = Segment.value;
  }
  lcd.color(Teal);
  axis_draw_raw(old_seg);
  border_draw();
}

///////////////////////////////////////////////////////////////////////////////

// Обновление графика
void data_draw() {
  static int16_t points2[POINTES] = {};
  int16_t *points                 = data.get_points();
  int last                        = points2[0];
  int last2                       = points[0];
  points2[0]                      = last2;
  lcd.viewport(&view);

  for (int i = 1; i < POINTES; i++) {
    lcd.color(Black);
    lcd.h_line(i + view.min_x - 1, last, points2[i]);
    lcd.color(Yellow);
    lcd.h_line(i + view.min_x - 1, last2, points[i]);
    last  = points2[i];
    last2 = points2[i] = points[i];
  }
}

///////////////////////////////////////////////////////////////////////////////

void init() {
  // Восстановление настроек из батарейного домена
  // if (RTC->REG[15] == RTC_REG_SAVED)
  //   menu.load((int *)RTC->REG);
  // RTC->REG[15] = RTC_REG_SAVED;

  // Таймер для работы с АЦП
  tim4.PSC(0);
  tim4.cont();
  tim4.OCR(1, 4);
  tim4.pwm(TIM_EN, 4);
  tim4.enable(4);

  // sysTick Сканирование энкодера по таймеру
  STK_CMP  = F_CPU / INT_FQ - 1;
  STK_CTRL = STK_STE | STK_STCLK | STK_STRE | STK_STIE;

  // ADC DMA
  adc.dma_enable();
  adc.trigger(ADC_ExternalTrigConv_T4_CC4);

  dma.adc(data.get_buffer(), SAMPLES);
  dma.int_compl();

  // LCD
  lcd.init();
  if (lcd.max_x() < 320) lcd.font(system_5x7, 1, 3);
  else lcd.font(arial_14, 1, 3);

  //
  fft.init();
  enc.init();

  NVIC_EnableIRQ(SysTick_IRQn);
  NVIC_EnableIRQ(DMA1_Channel1_IRQn);
}

//////////////////////////////////  АЦП  //////////////////////////////////////

// Один канал
void record_adc(uint32_t time, uint32_t len) {
  // tim4.disable();
  // adc.delay(time);          // Устанавливаем время выборки АЦП
  tim4.TOP(time - 1);  // Количество тактов между семплами
  tim4.enable();

  // dma.adc(data.get_buffer(), len);
  // dma.int_compl();

  dma.CNT(len);
  dma.start();              // Запускаем передачу данных из АЦП в буфер
  while (dma.is_active());  // Ожидаем завершения работы DMA
  tim4.disable();
}

// Два канала
void record_dual(uint32_t time) {
  // tim4.disable();
  adc.delay(time);          // Устанавливаем время выборки АЦП
  tim4.TOP(time - 1);       // Количество тактов между семплами
  tim4.enable();
  dma.start();              // Запускаем передачу данных из АЦП в буфер
  while (dma.is_active());  // Ожидаем завершения работы DMA
  tim4.disable();
}

// Заполнение буфера данными из АЦП
void record(uint32_t time, uint32_t len) {
  record_adc(time, len);
}

///////////////////////////////////////////////////////////////////////////////

void DMA1_Channel1_IRQHandler(void) {
  dma.reset();
}

void SysTick_Handler(void) {
  int inc = enc.scan();
  if (inc) menu.next(inc);
  count++;
  timer++;
  STK_SR = 0;
}
