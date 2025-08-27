#pragma once
#include "ch32v20x.h"


#ifdef __cplusplus
 extern "C" {
#endif 

#define STK_SWIE  (1 << 31)  //
#define STK_INIT  (1 << 5)   // обновлять счётчик при старте
#define STK_MODE  (1 << 4)   // 0 = UP  1 = DOWN
#define STK_STRE  (1 << 3)   // 0 = игнорировать CMP 1 = сбрасывать счёт
#define STK_STCLK (1 << 2)   // 0 = div 8
#define STK_STIE  (1 << 1)   // прерывания
#define STK_STE   (1 << 0)   // запуск

#define STK_CTRL  SysTick->CTLR
#define STK_CMP   SysTick->CMP
#define STK_SR    SysTick->SR

void delay_init(void);
void delay_us(uint32_t n);
void delay_ms(uint32_t n);

#ifdef __cplusplus
}
#endif