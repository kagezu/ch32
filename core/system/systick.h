#pragma once

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
#define STK_CNT   SysTick->CNT

#define STK_E     SysTick->CTLR = STK_STE | STK_STCLK; SysTick->SR = 0
#define STK_D     SysTick->CTLR = 0; SysTick->SR = 0
#define STK_C     SysTick->CTLR = STK_INIT | STK_STCLK | STK_STE



#ifdef __cplusplus
}
#endif