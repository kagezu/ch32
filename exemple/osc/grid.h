#include "core.h"

#define MAX_PIX_GRID  60
#define MIN_PIX_GRID  24
#define STEP_PIX_GRID 4
#define MAX_X_GRID    300

extern const int _fq[];

u16 convert_x[MAX_X_GRID];

void init_convert_x() {
  u16 count      = 0;
  u8 range       = 0;
  u8 pix         = MAX_PIX_GRID;
  sc16 max_range = sizeof(_fq) >> 1;

  while (count < MAX_X_GRID && range < max_range) {
    convert_x[count++] = pix)| (range << 8);
    // pix -= STEP_PIX_GRID;
    pix--;
    if (pix < MIN_PIX_GRID) {
      pix = MIN_PIX_GRID * (s32)_fq[range + 1] / _fq[range];
      if (pix > MAX_PIX_GRID) pix = MAX_PIX_GRID;
      range++;
    }
  }
}