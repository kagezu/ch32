#pragma once

#include "ch32v20x.h"


#define PORT(x,y) GPIO ## x




// #define IN(port, pin)       pin < 8 ?                                   \
//                             PORT(port) ->CFGLR &= ~(b11 << (pin << 2))  \
//                             PORT(port) ->CFGHR &= ~(b11 << ((pin - 8) << 2))

// #define OUT(port, pin)      pin < 8 ?                                   \
//                             PORT(port) ->CFGLR |= b11 << (pin << 2)     \
//                             PORT(port) ->CFGHR |= b11 << ((pin - 8) << 2)
// #define OUT_2MH(port, pin) pin < 8 ?                                   \
//                             PORT(port) ->CFGLR |= b10 << (pin << 2)     \
//                             PORT(port) ->CFGHR |= b10 << ((pin - 8) << 2)
// #define OUT_10MH(port, pin) pin < 8 ?                                   \
//                             PORT(port) ->CFGLR |= b01 << (pin << 2)     \
//                             PORT(port) ->CFGHR |= b01 << ((pin - 8) << 2)



// #define P_NC(port, pin)     pin < 8 ?                                                                                   \
//                             PORT(port) ->CFGLR = (PORT(port) ->CFGLR & ~(b1100 << (pin << 2))) | (b0100 << (pin << 2))  \
//                             PORT(port) ->CFGLR = (PORT(port) ->CFGHR & ~(b1100 << ((pin - 8) << 2))) | (b0100 << ((pin - 8) << 2))






// #define GPIO(port, pin)     PAD_CONFIG->PORT_ ## port ## _CFG &= ~(0b11 << ( pin << 1))
// #define SERIAL(port, pin)   PAD_CONFIG->PORT_ ## port ## _CFG |= (0b01 << ( pin << 1))
// #define TIMER(port, pin)    PAD_CONFIG->PORT_ ## port ## _CFG |= (0b10 << ( pin << 1))
// #define ANALOG(port, pin)   PAD_CONFIG->PORT_ ## port ## _CFG |= (0b11 << ( pin << 1))

// #define DS_2MA(port, pin)   PAD_CONFIG->PORT_ ## port ## _DS |= (0b00 << ( pin << 1))
// #define DS_4MA(port, pin)   PAD_CONFIG->PORT_ ## port ## _DS |= (0b01 << ( pin << 1))
// #define DS_8MA(port, pin)   PAD_CONFIG->PORT_ ## port ## _DS |= (0b10 << ( pin << 1))

// #define P_NC(port, pin)     PAD_CONFIG->PORT_ ## port ## _PUPD &= ~(0b11 << ( pin << 1))
// #define P_VCC(port, pin)    PAD_CONFIG->PORT_ ## port ## _PUPD |= (0b01 << ( pin << 1))
// #define P_GND(port, pin)    PAD_CONFIG->PORT_ ## port ## _PUPD |= (0b10 << ( pin << 1))

// #define IN(port, pin)       GPIO_ ## port ->DIRECTION_IN = (1 << pin)
// #define OUT(port, pin)      GPIO_ ## port ->DIRECTION_OUT = (1 << pin)

// #define SET(port, pin)      GPIO_ ## port ->SET = (1 << pin)
// #define CLR(port, pin)      GPIO_ ## port ->CLEAR = (1 << pin)
// #define INV(port, pin)      GPIO_ ## port ->OUTPUT ^= (1 << pin)
// #define GET(port, pin)      (GPIO_ ## port ->STATE & (1 << pin))

#define MASK(port, pin)     (1 << pin)
#define OUTPUT(port, pin)   (GPIO ## port ->OUTDR)
#define STATE(port, pin)    (GPIO_ ## port ->INDR)



#define SET(port, pin)      GPIO ## port ->BSHR = (1 << pin)
#define CLR(port, pin)      GPIO ## port ->BCR = (1 << pin)
#define INV(port, pin)      GPIO ## port ->OUTDR ^= (1 << pin)
#define GET(port, pin)      (GPIO ## port ->INDR & (1 << pin))