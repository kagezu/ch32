#include "core.h"
#include "lcd.h"

Pin<PA, 0> EN_A; 
Pin<PA, 1> EN_B; 
Pin<PA, 2> USER_SW; 
// Pin<PA, 0> USER_B;

LCD lcd;
int mode = 1;


void init_encoder() {
  USER_SW.init(GP_VCC);
  EN_A.init(GP_VCC);
  EN_B.init(GP_VCC);

  tim2.TOP(0xFF);
  tim2.enable(1);
  tim2.enable(2);
  tim2.encoder();
  tim2.enable();

  RCC->APB1PCENR |= RCC_TIM2EN;

  TIM2->SMCFGR = 0b11;  // Енкодер
  TIM2->ATRLR = 0xffff;
  TIM2->CNT = -2;
  TIM2->CCER = TIM_CC1E | TIM_CC2E;  // Включить канал 1,2
  TIM2->CTLR1 = TIM_DIR | TIM_CEN;
}

int main(void) {
  init_encoder();
  lcd.init();
  if (lcd.max_y() > 300) lcd.font(sans_24, 0, 0);
  else lcd.font(serif_18i, 0, 0);
  lcd.color(White);
  lcd.background(MidnightBlue);

  STK_E;

  // int x = 0;
  while (true) {
    if (!USER_SW.get()) {
      while (!USER_SW.get());
      mode++;
    }
    STK_C;

    switch (mode) {
      case 0:
        lcd.background(color[tim2.CNT() & 0x7F]);
        lcd.clear();
        break;
      case 1: lcd.demo(tim2.CNT()); break;
      case 2: lcd.demo2(tim2.CNT()); break;
      case 3: lcd.demo3(tim2.CNT()); break;

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