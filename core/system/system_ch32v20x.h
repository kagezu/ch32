/********************************** (C) COPYRIGHT *******************************
 * File Name          : system_ch32v20x.h
 * Author             : WCH
 * Version            : V1.0.0
 * Date               : 2021/06/06
 * Description        : CH32V20x Device Peripheral Access Layer System Header File.
 *********************************************************************************
 * Copyright (c) 2021 Nanjing Qinheng Microelectronics Co., Ltd.
 * Attention: This software (modified or not) and binary are used for
 * microcontroller manufactured by Nanjing Qinheng Microelectronics.
 *******************************************************************************/
#ifndef __SYSTEM_ch32v20x_H
#define __SYSTEM_ch32v20x_H

// #define SYSCLK_FREQ_HSE    HSE_VALUE
// #define SYSCLK_FREQ_48MHz_HSE  48000000
// #define SYSCLK_FREQ_56MHz_HSE  56000000
// #define SYSCLK_FREQ_72MHz_HSE  72000000
// #define SYSCLK_FREQ_96MHz_HSE  96000000
// #define SYSCLK_FREQ_120MHz_HSE  120000000
#define SYSCLK_FREQ_144MHz_HSE 144000000
// #define SYSCLK_FREQ_HSI    HSI_VALUE
// #define SYSCLK_FREQ_48MHz_HSI  48000000
// #define SYSCLK_FREQ_56MHz_HSI  56000000
// #define SYSCLK_FREQ_72MHz_HSI  72000000
// #define SYSCLK_FREQ_96MHz_HSI  96000000
// #define SYSCLK_FREQ_120MHz_HSI  120000000
// #define SYSCLK_FREQ_144MHz_HSI  144000000

#ifdef SYSCLK_FREQ_HSE
#define F_CPU SYSCLK_FREQ_HSE
#elif defined SYSCLK_FREQ_48MHz_HSE
#define F_CPU SYSCLK_FREQ_48MHz_HSE
#elif defined SYSCLK_FREQ_56MHz_HSE
#define F_CPU SYSCLK_FREQ_56MHz_HSE
#elif defined SYSCLK_FREQ_72MHz_HSE
#define F_CPU SYSCLK_FREQ_72MHz_HSE
#elif defined SYSCLK_FREQ_96MHz_HSE
#define F_CPU SYSCLK_FREQ_96MHz_HSE
#elif defined SYSCLK_FREQ_120MHz_HSE
#define F_CPU SYSCLK_FREQ_120MHz_HSE
#elif defined SYSCLK_FREQ_144MHz_HSE
#define F_CPU SYSCLK_FREQ_144MHz_HSE
#elif defined SYSCLK_FREQ_48MHz_HSI
#define F_CPU SYSCLK_FREQ_48MHz_HSI
#elif defined SYSCLK_FREQ_56MHz_HSI
#define F_CPU SYSCLK_FREQ_56MHz_HSI
#elif defined SYSCLK_FREQ_72MHz_HSI
#define F_CPU SYSCLK_FREQ_72MHz_HSI
#elif defined SYSCLK_FREQ_96MHz_HSI
#define F_CPU SYSCLK_FREQ_96MHz_HSI
#elif defined SYSCLK_FREQ_120MHz_HSI
#define F_CPU SYSCLK_FREQ_120MHz_HSI
#elif defined SYSCLK_FREQ_144MHz_HSI
#define F_CPU SYSCLK_FREQ_144MHz_HSI
#else
#define F_CPU HSI_VALUE
#endif

#ifdef __cplusplus
extern "C" {
#endif

extern uint32_t SystemCoreClock;

/* System_Exported_Functions */
extern void SystemInit(void);
extern void SystemCoreClockUpdate(void);
extern void SystemError(uint32_t div);

#ifdef __cplusplus
}
#endif

#endif /*__CH32V20x_SYSTEM_H */
