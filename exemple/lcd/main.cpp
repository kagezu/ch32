#include "core.h"
#include "lcd.h"

LCD lcd;


int main(void) {
  lcd.init();
  if (lcd.max_y() > 300) lcd.font(sans_24, 0, 0);
  else lcd.font(serif_18i, 0, 0);
  lcd.color(White);
  lcd.background(MidnightBlue);

  STK_E;

  int x = 0;
  while (true) {
    STK_C;
    lcd.demo(x++);

    u32 fps = (SystemCoreClock << 4) / STK_CNT;
    lcd.at(0, lcd.max_y() - 2 * lcd.get_height() + 1);
    lcd.printf(
      "FPS: %.2.4q\n%u X %u X %u", fps,
      lcd.max_x() + 1, lcd.max_y() + 1, RGB::len());
  }
}