// #include "pinout.h"
#pragma once


// ADC

// #define ADC0(f)         f(1, 5)
// #define ADC1(f)         f(1, 7)
// #define ADC2(f)         f(0, 2)
// #define ADC3(f)         f(0, 4)
// #define ADC4(f)         f(0, 7)
// #define ADC5(f)         f(0, 9)
// #define ADC6(f)         f(0, 11)
// #define ADC7(f)         f(0, 13)

// DAC

// #define DAC0(f)         f(1, 12)
// #define DAC1(f)         f(1, 13)

// Aref

// #define AREF(f)         f (1, 11)

// SPI


#define SPI1_MOSI(f) f(A, 7)
#define SPI1_MISO(f) f(A, 6)
#define SPI1_SCK(f)  f(A, 5)
#define SPI1_NSS(f)  f(A, 4)

#define SPI2_MOSI(f) f(B, 15)
#define SPI2_MISO(f) f(B, 14)
#define SPI2_SCK(f)  f(B, 13)
#define SPI2_NSS(f)  f(B, 12)

// JTAG

// #define TDI(f)          f(0, 11)
// #define TCK(f)          f(0, 12)
// #define TMS(f)          f(0, 13)
// #define TRST(f)         f(0, 14)
// #define TDO(f)          f(0, 15)

// I2C

// #define I2C0_SDA(f)     f(0, 9)
// #define I2C0_SCL(f)     f(0, 10)

// #define I2C1_SDA(f)     f(1, 12)
// #define I2C1_SCL(f)     f(1, 13)

// UART

// #define UART0_RXD(f)    f(0, 5)
// #define UART0_TXD(f)    f(0, 6)
// #define UART0_CTS(f)    f(0, 7)
// #define UART0_RTS(f)    f(0, 8)

// #define UART1_RXD(f)    f(1, 8)
// #define UART1_TXD(f)    f(1, 9)
// #define UART1_CTS(f)    f(1, 10)
// #define UART1_RTS(f)    f(1, 11)


// ACE-UNO

#define USER_B(f)    f(A, 0)
#define USER_L(f)    f(B, 2)

#define D0(f)        f(B, 0)
#define D1(f)        f(B, 1)
#define D2(f)        f(B, 2)
#define D3(f)        f(B, 3)
#define D4(f)        f(B, 4)
#define D5(f)        f(B, 5)
#define D6(f)        f(B, 6)
#define D7(f)        f(B, 7)
#define D8(f)        f(B, 8)
#define D9(f)        f(B, 9)
#define D10(f)       f(B, 10)
#define D11(f)       f(B, 11)
#define D12(f)       f(B, 12)
#define D13(f)       f(B, 13)
#define D14(f)       f(B, 14)
#define D15(f)       f(B, 15)

// #define D18(f)          f(1, 12)
// #define D19(f)          f(1, 13)

// #define A0(f)           f(1, 5)
// #define A1(f)           f(1, 7)
// #define A2(f)           f(0, 4)
// #define A3(f)           f(0, 7)
// #define A45(f)          f(0, 9)

// #define SELA45(f)       f(1, 15)
// #define NSS0(f)         f(1, 10)
// #define NSS1(f)         f(1, 6)
