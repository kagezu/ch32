#pragma once

#include "ch32v20x.h"


void GPIO_Init_Pin(GPIO_TypeDef *GPIOx, GPIOMode_TypeDef GPIO_Mode, uint8_t pin);

#define IN(port, pin)    GPIO_Init_Pin(GPIO##port, GPIO_Mode_IN_FLOATING, pin)
#define OUT(port, pin)   GPIO_Init_Pin(GPIO##port, GPIO_Mode_Out_PP, pin)
#define P_NC(port, pin)  GPIO_Init_Pin(GPIO##port, GPIO_Mode_IN_FLOATING, pin)
#define P_VCC(port, pin) GPIO_Init_Pin(GPIO##port, GPIO_Mode_IPU, pin)
#define P_GND(port, pin) GPIO_Init_Pin(GPIO##port, GPIO_Mode_IPD, pin)


#define PORT(port, pin)  GPIO##port

#define GPIO(port, pin)
#define TIMER(port, pin)  GPIO_Init_Pin(GPIO##port, GPIO_Mode_AF_PP, pin)


// #define SERIAL(port, pin)   PAD_CONFIG->PORT_ ## port ## _CFG |= (0b01 << ( pin << 1))
// #define TIMER(port, pin)    PAD_CONFIG->PORT_ ## port ## _CFG |= (0b10 << ( pin << 1))
// #define ANALOG(port, pin)   PAD_CONFIG->PORT_ ## port ## _CFG |= (0b11 << ( pin << 1))

#define MASK(port, pin)   (1 << pin)
#define OUTPUT(port, pin) (GPIO##port->OUTDR)
#define STATE(port, pin)  (GPIO_##port->INDR)


#define SET(port, pin)    GPIO##port->BSHR = (1 << pin)
#define CLR(port, pin)    GPIO##port->BCR = (1 << pin)
#define INV(port, pin)    GPIO##port->OUTDR ^= (1 << pin)
#define GET(port, pin)    (GPIO##port->INDR & (1 << pin))