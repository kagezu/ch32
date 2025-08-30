#include "ch32v20x.h"

void delay_us(uint32_t us) {
#ifdef SYSCLK_FREQ_HSE
  if (!us) return;
  us--;
  if (!us) return;
  us = (us << 3) / 3 - 2;
  if (!us) return;
  us--;
#else
  if (!us) return;
  int a = F_CPU / 3000000 - 7;
  asm volatile(
    "l1%=:               \n\t"
    "addi %0, %0, -1    \n\t"   // 1 такт
    "bne x0, %0, l1%=    \n\t"  // 2 такта
    : "=r"(a)
    : "0"(a));
  us--;
  us *= F_CPU / 3000000;
  us++;
#endif

  asm volatile(
    "l2%=:               \n\t"
    "addi %0, %0, -1    \n\t"   // 1 такт
    "bne x0, %0, l2%=    \n\t"  // 2 такта
    : "=r"(us)
    : "0"(us));
}

void delay_ms(uint32_t ms) {
  while (ms--) delay_us(1000);
}