#include "core.h"
#include "lcd.h"

#define USER_B(f) f(A, 0)

LCD lcd;

int main(void) {
  STK_E;
  USER_B(P_GND);

  lcd.init();
  if (lcd.max_y() > 300) lcd.font(sans_24, 0, 0);
  else lcd.font(serif_18i, 0, 0);
  lcd.color(White);
  lcd.background(MidnightBlue);
  lcd.clear();

  lcd.printf("\f \n");
  lcd.printf("F_CPU: %u MHz \n", SystemCoreClock / 1000000);

  STK_C;
  uint32_t tick = STK_CNT;
  lcd.printf("init: %u tick \n", tick);

  STK_C;
  delay_us(0);
  tick = STK_CNT;
  lcd.printf("delay_us(0): %lu tick \n", tick);

  STK_C;
  delay_us(1);
  tick = STK_CNT;
  lcd.printf("delay_us(1): %lu tick \n", tick);

  STK_C;
  delay_us(2);
  tick = STK_CNT;
  lcd.printf("delay_us(2): %lu tick \n", tick);

  STK_C;
  delay_us(1000);
  tick = STK_CNT;
  lcd.printf("delay_us(1000): %lu tick \n", tick);

  STK_C;
  delay_us(1000000);
  tick = STK_CNT;
  lcd.printf("delay_ms(1000000): %lu tick \n", tick);

  while (1);
}