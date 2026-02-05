#include "core.h"

template <const int SMP, const int PIX>
class OSC {
private:
  s16 buffer[SMP];
  s16 points[PIX];
  s16 begin;
  s32 scale;
  s32 offset;

public:
  constexpr static bool BUFFER = false;
  constexpr static bool POINTS = true;

public:
  OSC() {}

  s16 *get_buffer() { return (s16 *)buffer; }
  s16 *get_points() { return (s16 *)points; }

  /**
   * @brief Преобразование к координатам дисплея
   *
   * @param scale Масштаб Q32.16
   * @param offset Вертикальное смещение
   */
  void to_points(s32 scale, s32 offset, boot src = BUFFER) {
    s16 count = PIX;
    s16 *in   = src ? points : buffer + begin;
    s16 *out  = points;
    while (count--) *out++ = offset - (((*in++) * scale) >> 16);
  }
};