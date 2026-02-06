#include "core.h"

template <const int SMP>
class OSC {
private:
  int16_t buffer[SMP];
  int16_t begin;
  int16_t length;
  int16_t median;
  int32_t scale;

  int32_t offset_y;

  uint8_t trigger;

public:
  constexpr static bool BUFFER = false;
  constexpr static bool POINTS = true;

  constexpr static uint8_t RISING  = 0;
  constexpr static uint8_t FALLING = 1;
  constexpr static uint8_t MAXIMUM = 2;
  constexpr static uint8_t MINIMUM = 3;

  constexpr int16_t *get_buffer() { return (int16_t *)buffer; }

  int16_t *get_points() { return (int16_t *)(buffer + begin); }
  void set_points(uint16_t start = 0) { begin = start; }

public:
  OSC() {}

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

    switch (trigger) {
      case RISING:
        if (offset_max > offset_min) offset = offset_min;  // Сканирование начать с минимума
        else
          while (offset++ < end)
            if (buffer[offset] < median) break;

        while (offset++ < end)
          if (buffer[offset] > median) break;
        begin = offset - (length >> 1);
        break;

      case FALLING:
        if (offset_max < offset_min) offset = offset_max;  // Сканирование начать с максимума
        else
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
    if (offset_max < begin || offset_max > begin + length || offset_min < begin || offset_min > begin + length) {
      value_min = INT16_MAX;
      value_max = INT16_MIN;
      for (int16_t i = begin; i < begin + length; i++) {
        const int16_t value = buffer[i];
        if (value_min > value) value_min = value;
        if (value_max < value) value_max = value;
      }
      median = (value_min + value_max) >> 1;
    }

    int16_t count = length;
    int16_t *in   = buffer + begin + length;
    int16_t *out  = buffer + SMP;
    offset_y += ((median * scale) >> 12);  // Q32.12 -> Q32
    while (count--) {
      int32_t result = offset_y - (((*--in) * scale) >> 12);
      *--out         = result;// > INT16_MAX ? INT16_MAX : result < INT16_MIN ? INT16_MIN
                                                                           : result;
    }
    begin = SMP - length;
  }
};


// if (offset_min > offset_max) half_period = offset_min - offset_max;
// else half_period = offset_max - offset_min;