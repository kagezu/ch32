#include "core.h"
#include "lcd.h"

#define USER_B(f) f(A, 0)

LCD lcd;

int mode = 1;

int main(void) {
  USER_B(P_GND);
  lcd.init();
  if (lcd.max_y() > 300) lcd.font(sans_24, 0, 0);
  else lcd.font(serif_18i, 0, 0);
  lcd.color(White);
  lcd.background(MidnightBlue);

  STK_E;

  int x = 0;
  while (true) {
    if (USER_B(GET)) {
      while (USER_B(GET));
      mode++;
    }
    STK_C;

    switch (mode) {
      case 0:
        lcd.background(color[x++ & 0x7F]);
        lcd.clear();
        break;
      case 1: lcd.demo(x++); break;
      case 2: lcd.demo2(x++); break;
      case 3: lcd.demo3(x++); break;

     default: mode = 0;
    }

    uint32_t fps = (F_CPU << 4) / STK_CNT;
    lcd.at(0, lcd.max_y() - 2 * lcd.get_height() + 1);
    lcd.color(White);
    lcd.background(MidnightBlue);
    lcd.printf(
      "FPS: %.2.4q\n%u X %u X %u", fps,
      lcd.max_x() + 1, lcd.max_y() + 1, RGB::len());
  }
}