#pragma once

#include "ch32v20x.h"

void GPIO_Init_Pin(GPIO_TypeDef *GPIOx, GPIOMode_TypeDef GPIO_Mode, uint8_t pin);

#define IN(port, pin)    GPIO_Init_Pin(GPIO##port, GPIO_Mode_IN_FLOATING, pin)
#define INA(port, pin)   GPIO_Init_Pin(GPIO##port, GPIO_Mode_AIN, pin)
#define OUT(port, pin)   GPIO_Init_Pin(GPIO##port, GPIO_Mode_Out_PP, pin)
#define OUTA(port, pin)  GPIO_Init_Pin(GPIO##port, GPIO_Mode_AF_PP, pin)
#define P_NC(port, pin)  GPIO_Init_Pin(GPIO##port, GPIO_Mode_IN_FLOATING, pin)
#define P_VCC(port, pin) GPIO_Init_Pin(GPIO##port, GPIO_Mode_IPU, pin)
#define P_GND(port, pin) GPIO_Init_Pin(GPIO##port, GPIO_Mode_IPD, pin)

#define PORT(port, pin)  GPIO##port
#define GPIO(port, pin)
#define MASK(port, pin)   (1 << pin)
#define OUTPUT(port, pin) (GPIO##port->OUTDR)
#define STATE(port, pin)  (GPIO_##port->INDR)

#define SET(port, pin)    GPIO##port->BSHR = (1 << pin)
#define CLR(port, pin)    GPIO##port->BCR = (1 << pin)
#define INV(port, pin)    GPIO##port->OUTDR ^= (1 << pin)
#define GET(port, pin)    (GPIO##port->INDR & (1 << pin))

#define GPIO_Imput_Analog ((uint32_t)0x00)
#define GPIO_Imput_Float  ((uint32_t)0x04)
#define GPIO_Imput_Pull   ((uint32_t)0x08)
#define GPIO_Output_PP    ((uint32_t)0x00)
#define GPIO_Output_OD    ((uint32_t)0x04)
#define GPIO_Output_AF    ((uint32_t)0x08)


constexpr u32 PA = GPIOA_BASE;
constexpr u32 PB = GPIOB_BASE;
constexpr u32 PC = GPIOC_BASE;

constexpr u32 GP_IO     = 0x00;  // GPIO
constexpr u32 GP_Serial = 0x08;  // AF
constexpr u32 GP_Timer  = 0x08;  // AF
constexpr u32 GP_Analog = 0x00;  // Input Analog

constexpr u32 GP_Float = 0x04;   // floating
constexpr u32 GP_VCC   = 0x18;   // pull-up
constexpr u32 GP_GND   = 0x08;   // pull-down
constexpr u32 GP_Open  = 0x04;   // open-drain

constexpr u32 GPO_10MHz = 0x01;
constexpr u32 GPO_2MHz  = 0x02;
constexpr u32 GPO_50MHz = 0x03;

// Для совместимости
constexpr u32 GPO_2mA = 0x03;
constexpr u32 GPO_4mA = 0x01;
constexpr u32 GPO_8mA = 0x02;
constexpr u32 GPO_Max = 0x02;


template <uc32 GPIOx, uc32 PINx>
class Pin {
public:
  INLINE void set() { ((GPIO_TypeDef *)GPIOx)->BSHR = 1 << PINx; }
  INLINE void clr() { ((GPIO_TypeDef *)GPIOx)->BCR = 1 << PINx; }
  INLINE void inv() { ((GPIO_TypeDef *)GPIOx)->OUTDR ^= 1 << PINx; }
  INLINE u32 get() { return ((GPIO_TypeDef *)GPIOx)->INDR & (1 << PINx); }
  INLINE void out(bool data) {
    if (data) set();
    else clr();
  }
  INLINE void init(uc32 conf) {
    if (PINx < 8)
      ((GPIO_TypeDef *)GPIOx)->CFGLR =
        (((GPIO_TypeDef *)GPIOx)->CFGLR & ~(((u32)0x0F) << (PINx << 2))) |
        ((conf & 0x0F) << (PINx << 2));
    else
      ((GPIO_TypeDef *)GPIOx)->CFGHR =
        (((GPIO_TypeDef *)GPIOx)->CFGHR & ~(((u32)0x0F) << ((PINx - 8) << 2))) |
        ((conf & 0x0F) << ((PINx - 8) << 2));
    if (conf == GP_VCC) ((GPIO_TypeDef *)GPIOx)->BSHR = (((u32)0x01) << PINx);
    if (conf == GP_GND) ((GPIO_TypeDef *)GPIOx)->BCR = (((u32)0x01) << PINx);
  }
};

template <uc32 GPIOx, uc32 PINS>
class Port {
public:
  INLINE u32 get() { return ((GPIO_TypeDef *)GPIOx)->INDR; }
  INLINE void out(u32 data) { ((GPIO_TypeDef *)GPIOx)->OUTDR = data; }
  void init(uc32 conf = GPO_2MHz) {
    for (u32 pin = 0; pin < 8; pin++) {
      if (((u32)0x001 << pin) & PINS)
        ((GPIO_TypeDef *)GPIOx)->CFGLR =
          (((GPIO_TypeDef *)GPIOx)->CFGLR &
           ~(((u32)0x0F) << (pin << 2))) |
          ((conf & 0x0F) << (pin << 2));
      if (((u32)0x100 << pin) & PINS)
        ((GPIO_TypeDef *)GPIOx)->CFGHR =
          (((GPIO_TypeDef *)GPIOx)->CFGHR &
           ~(((u32)0x0F) << (pin << 2))) |
          ((conf & 0x0F) << (pin << 2));
    }
    if (conf == GP_VCC) ((GPIO_TypeDef *)GPIOx)->BSHR = PINS;
    if (conf == GP_GND) ((GPIO_TypeDef *)GPIOx)->BCR = PINS;
  }
};
