#include "core.h"
#include "lcd.h"
#include "adc.h"

Pin<PA, 0> EN_A; 
Pin<PA, 1> EN_B; 
Pin<PA, 2> USER_SW; 

LCD lcd;

INLINE void test1(uint32_t arg) {
  delay_us(arg);
}

INLINE void test2(uint32_t arg) {
  delay_ms(arg);
}

void print1(uint32_t arg) {
  STK_C;
  test1(arg);
  uint32_t tick = STK_CNT;
  lcd.printf("Arg: %lu \t Time: %lu tick \n", arg, tick);
}

void print2(uint32_t arg) {
  STK_C;
  test2(arg);
  uint32_t tick = STK_CNT;
  lcd.printf("Arg: %lu \t Time: %lu tick \n", arg, tick);
}

void init_encoder() {
  USER_SW.in_vcc();
  EN_A.in_vcc();
  EN_B.in_vcc();

  RCC->APB1PCENR |= RCC_TIM2EN;

  TIM2->SMCFGR = 0b11;  // Енкодер
  TIM2->ATRLR = 0xffff;
  TIM2->CNT = -2;
  TIM2->CCER = TIM_CC1E | TIM_CC2E;  // Включить канал 1,2
  TIM2->CTLR1 = TIM_DIR | TIM_CEN;
}

RCC_ClocksTypeDef RCC_Clocks;

int main(void) {
  STK_E;

  RCC_GetClocksFreq(&RCC_Clocks);
  init_encoder();
  lcd.init();
  if (lcd.max_y() > 300) lcd.font(sans_24, 0, 0);
  else lcd.font(serif_18i, 0, 0);
  lcd.color(White);
  lcd.background(MidnightBlue);
  lcd.clear();

  lcd.printf("\f");
  lcd.printf("\n\t\t\t\t\tCPU:\t    %4u\tMHz ", RCC_Clocks.SYSCLK_Frequency / 1000000);
  lcd.printf("\n\t\t\t\t\tHCLK:\t    %4u\tMHz ", RCC_Clocks.HCLK_Frequency / 1000000);
  lcd.printf("\n\t\t\t\t\tPCLK1:\t    %4u\tMHz ", RCC_Clocks.PCLK1_Frequency / 1000000);
  lcd.printf("\n\t\t\t\t\tPCLK2:\t    %4u\tMHz ", RCC_Clocks.PCLK2_Frequency / 1000000);
  lcd.printf("\n\t\t\t\t\tADC:\t    %4u\tMHz ", RCC_Clocks.ADCCLK_Frequency / 1000000);
  lcd.printf("\f");

  // lcd.printf("\n   Test 1: \n\n");
  // for (uint32_t arg = 0; arg < 5; arg++) print1(arg);

  // lcd.printf("\n   Test 2: \n\n");
  // for (uint32_t arg = 0; arg < 5; arg++) print2(arg);

  Rect rect(50, 100, 450, 400);

  while (1) {
    uint16_t count = -TIM2->CNT;
    lcd.printf("\f\nCount: %.2.2q      \n", count);
    lcd.printf("Count: %u         \n", count);
    lcd.printf("  %u      ", USER_SW.get());
    lcd.demo4(&rect, count);
  }
}
