#pragma once
#include "gpio.h"

//===================== Config =============================

// ENCODER_A   Вывод А
// ENCODER_B   Вывод В
// ENCODER_SW  Кнопка


#ifdef MIK32V2

Pin<1, 0> ENCODER_A;
Pin<1, 1> ENCODER_B;
Pin<2, 6> ENCODER_SW;


// #define ENCODER_A   D2
// #define ENCODER_GND D3
// #define ENCODER_B   D4
// #define ENCODER_SW  USER_B
#endif

#ifdef __AVR__

Pin<PD, 5> ENCODER_SW;
Pin<PD, 7> ENCODER_A;
Pin<PD, 6> ENCODER_B;


// #define ENCODER_A   D2
// #define ENCODER_GND D3
// #define ENCODER_B   D4
// #define ENCODER_SW  USER_B
#endif

// #ifdef MIK32V2
// #define ENCODER_A   D19
// #define ENCODER_B   D18
// #define ENCODER_SW  D10
// #endif
