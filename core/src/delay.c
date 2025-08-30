#include "ch32v20x.h"

void delay_us(uint32_t us) {
// #ifdef SYSCLK_FREQ_HSE
//   if (!us) return;
//   us--;
//   us <<= 1;
//   if (!us) return;

//   asm volatile(
//     "1:                 \n\t"
//     "xor x0, x0, x0     \n\t"  // 1 такт
//     "addi %0, %0, -1    \n\t"  // 1 такт
//     "bne x0, %0, 1b     \n\t"  // 2 такта
//     : "=r"(us)
//     : "0"(us));

// #elif defined SYSCLK_FREQ_48MHz_HSE
// #elif defined SYSCLK_FREQ_56MHz_HSE
// #elif defined SYSCLK_FREQ_72MHz_HSE
// #elif defined SYSCLK_FREQ_96MHz_HSE
// #elif defined SYSCLK_FREQ_120MHz_HSE
// #elif defined SYSCLK_FREQ_144MHz_HSE

  if (!us) return;
  int l = 17;
  asm volatile(
    "1:                 \n\t"
    "addi %0, %0, -1    \n\t"  // 1 такт
    "bnez %0, 1b        \n\t"  // 2 такта
    : "=r"(l)
    : "0"(l));
  us--;
  us = (us << 5) + (us << 2);
  if (!us) return;

  asm volatile(
    "1:                 \n\t"
    "xor x0, x0, x0     \n\t"  // 1 такт
    "addi %0, %0, -1    \n\t"  // 1 такт
    "bne x0, %0, 1b     \n\t"  // 2 такта
    : "=r"(us)
    : "0"(us));

// #endif
}

void delay_ms(uint32_t ms) {
  while (ms--) delay_us(1000);
}


/*
static u32 p_us = 0;
static u32 p_ms = 0;

void delay_init(void) {
  p_us = SystemCoreClock / 1000000;
  p_ms = p_us * 1000;
}

void delay_us(uint32_t n) {
  u32 i = n * p_us;

  STK_SR = 0;
  STK_CMP = i;
  STK_CTRL = STK_MODE;
  STK_CTRL = STK_INIT | STK_MODE | STK_STCLK | STK_STE;
  while (!STK_SR);
  STK_CTRL = 0;
}

void delay_ms(uint32_t n) {
  u32 i = n * p_ms;

  STK_SR = 0;
  STK_CMP = i;
  STK_CTRL = STK_MODE;
  STK_CTRL = STK_INIT | STK_MODE | STK_STCLK | STK_STE;
  while (!STK_SR);
  STK_CTRL = 0;
}
*/