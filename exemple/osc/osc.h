// #pragma once
#include "main.h"


class OSC {
private:
  s16 buffer[SAMPLES];
  s16 begin;
  s16 length;
  s16 median;
  int32_t scale;

  int32_t offset_y;

  uint8_t trigger;

public:
  constexpr static bool BUFFER = false;
  constexpr static bool POINTS = true;

  constexpr static u8 RISING  = 0;
  constexpr static u8 FALLING = 1;
  constexpr static u8 MAXIMUM = 2;
  constexpr static u8 MINIMUM = 3;

  constexpr s16 *get_buffer() { return buffer; }

  s16 *get_points() { return (s16 *)(buffer + begin); }
  void set_points(uint16_t start = 0) { begin = start; }

public:
  // Q32.12
  void set_scale(int32_t s) { scale = s; }
  void set_offset(uint32_t offset) { offset_y = offset; }
  void set_trigger(uint8_t trig) { trigger = trig; }
  void set_length(uint16_t len) { length = len; }


  // Применить переобразования
  void release() {
    const int16_t start = length >> 1;
    const int16_t end   = SMP - start;
    int16_t value_min   = INT16_MAX;
    int16_t value_max   = INT16_MIN;
    int16_t offset_min  = start;
    int16_t offset_max  = start;
    int16_t offset      = start;

    for (int16_t i = start; i < end; i++) {
      const int16_t value = buffer[i];
      if (value_min > value) {
        value_min  = value;
        offset_min = i;
      }
      if (value_max < value) {
        value_max  = value;
        offset_max = i;
      }
    }

    median = (value_min + value_max) >> 1;
    //  median = 0x800;

    switch (trigger) {
      case RISING:
        // if (offset_max > offset_min) offset = offset_min;  // Сканирование начать с минимума
        // else
        while (offset++ < end)
          if (buffer[offset] < median) break;

        while (offset++ < end)
          if (buffer[offset] > median) break;
        begin = offset - (length >> 1);
        break;

      case FALLING:
        // if (offset_max < offset_min) offset = offset_max;  // Сканирование начать с максимума
        // else
        while (offset++ < end)
          if (buffer[offset] > median) break;

        while (offset++ < end)
          if (buffer[offset] < median) break;
        begin = offset - (length >> 1);
        break;

      case MAXIMUM:
        begin = offset_max - (length >> 1);
        break;

      case MINIMUM:
        begin = offset_min - (length >> 1);
        break;
    }

    // Уточнение средней линии
    // if (offset_max < begin || offset_max > begin + length || offset_min < begin || offset_min > begin + length) {
    //   value_min = INT16_MAX;
    //   value_max = INT16_MIN;
    //   for (int16_t i = begin; i < begin + length; i++) {
    //     const int16_t value = buffer[i];
    //     if (value_min > value) value_min = value;
    //     if (value_max < value) value_max = value;
    //   }
    //   base = (value_min + value_max) >> 1;
    // }

    // median += base - (median >> 5);

    int16_t count = length;
    s16 *in   = buffer + begin + length;
    s16 *out  = buffer + SMP;
    int32_t y     = offset_y + ((median * scale) >> 12);  // Q32.12 -> Q32
    while (count--) {
      int32_t result = y - (((*--in) * scale) >> 12);
      if (result & 0xFFFFF000) result = 0;
      *--out = result;
    }
    begin = SMP - length;
  }





};


// if (offset_min > offset_max) half_period = offset_min - offset_max;
// else half_period = offset_max - offset_min;