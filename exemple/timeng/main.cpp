#include "core.h"
#include "lcd.h"

#define USER_B(f) f(A, 0)
LCD lcd;

ATTR_INLINE void test1(uint32_t arg) {
  delay_us(arg);
}

ATTR_INLINE void test2(uint32_t arg) {
  delay_ms(arg);
}

int main(void) {
  STK_E;
  USER_B(P_GND);

  lcd.init();
  if (lcd.max_y() > 300) lcd.font(sans_24, 0, 0);
  else lcd.font(serif_18i, 0, 0);
  lcd.color(White);
  lcd.background(MidnightBlue);
  lcd.clear();

  lcd.printf("\f");
  lcd.printf("F_CPU: %u MHz \n", SystemCoreClock / 1000000);

  lcd.printf("\n   Test 1: \n\n");
  for (uint32_t arg = 0; arg < 5; arg++) {
    STK_C;
    test1(arg);
    uint32_t tick = STK_CNT;
    lcd.printf("Arg: %lu \t Time: %lu tick \n", arg, tick);
  }

  lcd.printf("\n   Test 2: \n\n");
  for (uint32_t arg = 0; arg < 5; arg++) {
    STK_C;
    test2(arg);
    uint32_t tick = STK_CNT;
    lcd.printf("Arg: %lu \t Time: %lu tick \n", arg, tick);
  }

  while (1);
}
