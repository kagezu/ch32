#include "core.h"

template <const int SMP, const int PIX>
class OSC {
private:
  int16_t buffer[SMP];
  int16_t points[PIX];
  int16_t begin;
  int16_t offset;
  int16_t median;
  int16_t half_period;
  int32_t scale;

public:
  constexpr static bool BUFFER = false;
  constexpr static bool POINTS = true;

  constexpr static uint8_t RISING  = 0;
  constexpr static uint8_t FALLING = 1;
  constexpr static uint8_t MAXIMUM = 2;
  constexpr static uint8_t MINIMUM = 3;

  constexpr int16_t *get_buffer() { return (int16_t *)buffer; }
  constexpr int16_t *get_points() { return (int16_t *)points; }

public:
  OSC() {}

  /**
   * @brief Преобразование к координатам дисплея
   *
   * @param scale Масштаб Q32.16
   * @param offset Вертикальное смещение
   */
  void scaling(int32_t scale, int32_t offset, bool src = BUFFER) {
    int16_t count = PIX;
    int16_t *in   = src ? points : buffer + begin;
    int16_t *out  = points;
    while (count--) *out++ = offset - (((*in++) * scale) >> 16);
  }

  // Поиск окна по тригеру
  void trigger(uint8_t trig = RISING, uint16_t length = PIX) {
    const uint16_t start = length >> 1;
    const uint16_t end   = length == PIX ? SMP - (PIX >> 1) : SMP - (length >> 1);
    int16_t value_min    = INT16_MAX;
    int16_t value_max    = INT16_MIN;
    int16_t offset_min   = start;
    int16_t offset_max   = start;

    for (uint16_t i = start; i < end; i++) {
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

    switch (trig) {
      case RISING:
        // offset = start;
        // while (offset++ < end)
        //   if (buffer[offset] < median) break;

        // Сканирование начать с минимума
        offset = offset_min;
        while (offset++ < end)
          if (buffer[offset] > median) break;
        break;

      case FALLING:
        // offset = start;
        // while (offset++ < end)
        //   if (buffer[offset] > median) break;

        // Сканирование начать с максимума
        offset = offset_max;
        while (offset++ < end)
          if (buffer[offset] < median) break;
        break;

      case MAXIMUM:
        offset = offset_max;
        break;

      case MINIMUM:
        offset = offset_min;
        break;
    }

    offset -= length >> 1;
    if (offset_min > offset_max) half_period = offset_min - offset_max;
    else half_period = offset_max - offset_min;
  }
};