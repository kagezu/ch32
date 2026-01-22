#include "main.h"
#include "lcd.h"
#include "adc.h"
#include "timer.h"
#include "dma.h"
#include "lagrange.h"
#include "encoder.h"
#include "fft.h"

constexpr int ADC_CH  = 0;    // Номер канала ADC
constexpr int INT_FQ  = 200;  // Hz опрос энкодера
constexpr int INFO_FQ = 3;    // Hz обновление текста
constexpr int Lp      = 8;    // Узловых точек для интерполяции Лагранжа (чётная)

// Дисплей
LCD lcd;
constexpr int BORDER_TOP    = 32;                                                             // Отступ от верха экрана
constexpr int BORDER_BOTTOM = 1;                                                              // Отступ от низа экрана
constexpr int SEG           = 16;                                                             // Шаг сетки
constexpr int POINTES       = ((lcd.max_x() + 2));                                            // Количество точек для отображения
constexpr int SAMPLES       = POINTES * 3;                                                    // Количество измерений для анализа
constexpr int END_LEN       = Lp + 2;                                                         // Резерв для интерполятора
constexpr int END_POINT     = SAMPLES - POINTES - END_LEN;                                    // Последняя точка, с которой может начаться отображение
constexpr int HEIGHT        = lcd.max_y() - BORDER_BOTTOM - BORDER_TOP;                       // Высота области для граф. данных
constexpr int MIDLE_AXIS    = (lcd.max_y() - BORDER_BOTTOM - (HEIGHT >> 1));                  // Расположение оси X
Rect view                   = Rect(0, BORDER_TOP, lcd.max_x(), lcd.max_y() - BORDER_BOTTOM);  // Графическая область

Lagrange<Lp, SEG, ADC::DEPTH> L;
FFT<SAMPLES, ADC::DEPTH> fft;
Encoder enc;
DMA<DMA_ADC1, DMA_VH> dma;

short buffer[SAMPLES];
short points[POINTES];
short points2[POINTES] = {};

int fps   = 20;
int count = 0;
int timer = 0;

// Преобразование к координатам дисплея
void transform_to_display(short in[], short out[], int32_t scale, int32_t offset) {
  for (int16_t i = 0; i <= POINTES; i++) out[i] = offset - (((in[i]) * scale) >> ADC::DEPTH);
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
  lcd.prints("             ");
}

///////////////////////////////////////////////////////////////////////////////

// Поиск окна
void osc_window(int &offset, int &median) {
  int min = 1 << ADC::DEPTH;
  int max = 0;
  offset  = 0;
  for (int i = (POINTES >> 1); i < END_POINT + (POINTES >> 1); i++) {
    const int val = buffer[i];
    if (min > val) min = val;
    else if (max < val) {
      max    = val;
      offset = i;
    }
  }
  offset -= POINTES >> 1;
  median = (min + max) >> 1;

  if (TType.value == Front) {
    offset = 0;
    while (offset++ < END_POINT)
      if (buffer[offset + (POINTES >> 1)] < median) break;
    while (offset++ < END_POINT)
      if (buffer[offset + (POINTES >> 1)] > median) break;
  }

  if (TType.value == Cutoff) {
    offset = 0;
    while (offset++ < END_POINT)
      if (buffer[offset + (POINTES >> 1)] > median) break;
    while (offset++ < END_POINT)
      if (buffer[offset + (POINTES >> 1)] < median) break;
  }
}

////////////////////////////////// Axis ///////////////////////////////////////

// Обрамление графической области
void border_draw() {
  lcd.viewport();
  lcd.color(Blue);
  lcd.w_line(0, view.min_y - 1, lcd.max_x());
  lcd.w_line(0, view.max_y + 1, lcd.max_x());
}

// Сетка
void axis_draw() {
  lcd.viewport(&view);
  lcd.color(Teal);

  lcd.h_line((lcd.max_x() >> 1), BORDER_TOP, lcd.max_y() - BORDER_BOTTOM);
  for (int i = 1; i <= lcd.max_x() / (SEG * 2); i++) {
    lcd.h_line((lcd.max_x() >> 1) + i * SEG, BORDER_TOP, lcd.max_y() - BORDER_BOTTOM);
    lcd.h_line((lcd.max_x() >> 1) - i * SEG, BORDER_TOP, lcd.max_y() - BORDER_BOTTOM);
  }

  lcd.w_line(0, MIDLE_AXIS, lcd.max_x() - 0);
  for (int i = 0; i <= (HEIGHT / SEG); i++) {
    lcd.w_line(0, MIDLE_AXIS + i * SEG, lcd.max_x());
    lcd.w_line(0, MIDLE_AXIS - i * SEG, lcd.max_x());
  }
}

///////////////////////////////////////////////////////////////////////////////

// Обновление графика
void data_draw() {
  int last   = points2[0];
  int last2  = points[0];
  points2[0] = last2;
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

  // Сканирование энкодера по таймеру
  // T32_0_PS; T32_0_FQ(INT_FQ);
  // T32_0_OVF; T32_0_IS; T32_0_EN;//EPIC_TIMER32_0_INDEX

  // Таймер для работы DMA с АЦП
  // T32_1_PS; T32_1_EN;

  // Таймер для расчёта FPS
  // T32_2_PS; T32_2_EN;

}

//////////////////////////////////  АЦП  //////////////////////////////////////

// Заполнение буфера данными из АЦП
void sample(uint32_t time) {
  uint32_t samp = ADC::cycle(time);  // Допустимое значение для АЦП
  // T32_1_TOP(time - 1);  // Количество тактов между семплами
  // T32_1_C;              // Обнуляем таймер, на случай если он уже выше TOP
  ADC::delay(samp);  // Устанавливаем время выборки АЦП
  ADC::start();      // Запускаем непрерывное преобразование АЦП
  // if (time < 64) cli(); // Если между выборками меньше 2х микросекунд, отключаем прерывания
  // dma.start();  // Запускаем передачу данных из АЦП в буфер
  // dma.wait();   // Ожидаем завершения работы DMA
  for (int i = 0; i < sizeof(buffer) >> 1; i++)
    buffer[i] = (u16)ADC::value();
  ADC::stop();  // Останавливаем преобразование АЦП
  // sei();
}

///////////////////////////////////////////////////////////////////////////////

int main(void) {
  init();
  lcd.init();
  if (lcd.max_x() < 320) lcd.font(system_5x7, 1, 3);
  else lcd.font(arial_14, 1, 3);
  fft.init();
  ADC::init(ADC_CH);
  dma.adc(buffer, sizeof(buffer));

  volatile int mode = -1;
  int sl            = 0;

  while (true) {
    const int32_t t_seg = (int32_t)FqScale.get_item<int>() << 5;       // Умножаем микросекунды на 32 MHz. [такты на сегмент]
    int32_t scale       = (ADC::AREF * SEG) / VScale.get_item<int>();  // Масштабирование по напряжению [пикселей на весь диапазон]
    fps                 = fps - (fps >> 2) + (INT_FQ / timer);         // Расчёт FPS
    timer               = 0;

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
        int t_adc  = ADC::cycle(t_seg / SEG);        // Готовим допустимое значение для АЦП. [тактов на выборку]
        int p_samp = (t_adc * SEG - 1) / t_seg + 1;  // Если больше 1, необходима интерполяция. [точек на выборку]
        int t_samp = t_seg * p_samp / SEG;           // Уточняем время выборки. [тактов на выборку]
        sample(t_samp);

        if (p_samp > 1) {  // Использовать интерполяцию
          L.init(p_samp);  // Инициализация коэффициентов Лагранжа
          // Чтоб немного сэкономить память, пишем в тот же буфер
          L.interpolate(buffer + SAMPLES - SAMPLES / p_samp - END_LEN / 2, buffer, SAMPLES);
        }
        int median, offset;
        osc_window(offset, median);  // Поиск окна по триггеру и среднего напряжения

        // Настраиваем масштабирование
        median = VType.value == 0 ? MIDLE_AXIS + ((median * scale) >> ADC::DEPTH) : lcd.max_y() - BORDER_BOTTOM;
        median -= ZeroLevel.value;
        transform_to_display(buffer + offset, points, scale, median);
        data_draw();
        break;
      }

      case MODE_FFT: {
        sample(t_seg);
        fft.run(buffer);                    // Быстрое преобразование Фурье
        if (FType.value) fft.sum();         // Применяем суммирующий фильтр
        fft.sqrt(buffer, lcd.max_x() + 1);  // Находим модуля амплитуд
        transform_to_display(buffer, points, scale, lcd.max_y() - BORDER_BOTTOM);
        data_draw();
        break;
      }

      case MODE_SPEC: {
        sample(t_seg);
        fft.run(buffer);
        fft.sqrt(buffer, lcd.max_y() + 2);

        for (int i = 1; i < lcd.max_y() + 2; i++) {
          int val = buffer[i];
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

    int inc = enc.scan();
    if (inc) menu.next(inc);
    count++;
    timer++;

    if (enc.is_push()) {
      menu.select();
      if (menu.value != MODE_SPEC) print_info();
      // menu.save((int *)RTC->REG);
      while (enc.is_push());
    }
  }
}

///////////////////////////////////////////////////////////////////////////////

// INT_HANDLER
// {
//   int inc = enc.scan();
//   if (inc) menu.next(inc);
//   count++;
//   timer++;
//   T32_0_IC;
//   EPIC_C;
// }
