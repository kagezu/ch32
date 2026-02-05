#pragma once
#include "core.h"

constexpr int ADC_CH   = 3;    // Номер канала ADC
constexpr int INT_FQ   = 200;  // Hz опрос энкодера
constexpr int INFO_FQ  = 5;    // Hz обновление текста
constexpr int Lp       = 8;    // Узловых точек для интерполяции Лагранжа (чётная)
constexpr int MAX_STEP = 30;   // Max шаг узлов интерполяции

// Дисплей
LCD lcd;
constexpr int BORDER_TOP    = 66;                                                             // 32;                                                             // Отступ от верха экрана
constexpr int BORDER_BOTTOM = 0;                                                              // Отступ от низа экрана
constexpr int POINTES       = ((lcd.max_x() + 2));                                            // Количество точек для отображения
constexpr int SAMPLES       = POINTES * 4;                                                    // Количество измерений для анализа
constexpr int END_LEN       = Lp + 2;                                                         // Резерв для интерполятора
constexpr int END_POINT     = SAMPLES - POINTES - END_LEN;                                    // Последняя точка, с которой может начаться отображение
constexpr int HEIGHT        = lcd.max_y() - BORDER_BOTTOM - BORDER_TOP;                       // Высота области для граф. данных
constexpr int MIDLE_AXIS    = (lcd.max_y() - BORDER_BOTTOM - (HEIGHT >> 1));                  // Расположение оси X