#include "core.h"

void GPIO_Toggle_INIT(void);

#define MAX_STEP_MS 5000
#define ONE_STEP_MS 1
#define ONE_STEP_US 1000

#define LIGHT_IDLE_MS 10

int main(void) {
  GPIO_Toggle_INIT();
  u32 i = 0;

  while (1) {
    if (!(GPIOA->INDR & GPIO_Pin_0)) {
      if (i < LIGHT_IDLE_MS) GPIO_WriteBit(GPIOB, GPIO_Pin_2, Bit_SET);
      else GPIO_WriteBit(GPIOB, GPIO_Pin_2, Bit_RESET);
      delay_ms(ONE_STEP_MS);
    } else {
      u32 k = i > MAX_STEP_MS >> 1 ? MAX_STEP_MS - i : i;
      k = (k << 1) * ONE_STEP_US / MAX_STEP_MS;
      k = k * k / ONE_STEP_US;

      GPIO_WriteBit(GPIOB, GPIO_Pin_2, Bit_SET);
      delay_us(k);

      GPIO_WriteBit(GPIOB, GPIO_Pin_2, Bit_RESET);
      delay_us(ONE_STEP_US - k);
    }

    i++;
    if (i > MAX_STEP_MS) i = 0;
  }
}

void GPIO_Toggle_INIT(void) {
  GPIO_InitTypeDef GPIO_InitStructure = {0};
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
  GPIO_Init(GPIOB, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  // GPIOA->CFGLR = 0b00;
}
