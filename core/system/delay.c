#include "systick.h"


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