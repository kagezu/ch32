#pragma once

#include "ch32v20x.h"
#include "gpio.h"
#include "attribute.h"
#include "systick.h"
#include "system_ch32v20x.h"


#define P(x)  x
#define PROGMEM
#define pgm_read_byte(addr) (*(const unsigned char *)(addr))
#define pgm_read_word(addr) (*(const unsigned short *)(addr))
#define pgm_read_dword(addr) (*(const unsigned long *)(addr))


#ifdef __cplusplus
 extern "C" {
#endif 

void delay_init(void);
void delay_us(uint32_t n);
void delay_ms(uint32_t n);

#ifdef __cplusplus
}
#endif