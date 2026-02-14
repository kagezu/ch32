#pragma once
#include "lcd.h"
#include "lagrange.h"
#include "adc.h"
#include "timer.h"
#include "dma.h"
#include "fft.h"
#include "encoder.h"

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

