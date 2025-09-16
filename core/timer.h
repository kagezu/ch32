#pragma once
#include "core.h"

namespace TimerConfig {

constexpr u16
  INT = 0x0001,  // CCxIE
  DMA = 0x0100,  // CCxDE
  INV = 0x0002,  // CCxP
  OVR = 0x0030,  // OCxM
  HIG = 0x0010,  // OCxM
  LOW = 0x0020,  // OCxM

  PWM = 0x0060;  // OCxM

constexpr uc32
  SINGLE = (1 << 3);


constexpr u32
  CMP = 1,
  CAP = 3,
  DIR = 4,
  REV = 5,
  BI = 6,
  ENC = 7;


};  // namespace TimerConfig
