// #include "encoder.h"
#pragma once
#include "core.h"
#include "pins.h"

class Encoder {
public:
  int count = 0, c1 = 0, c2 = 0;
  bool a0, b0, push = 0;

public:
  Encoder()
  {
    ENCODER_A.init(GP_VCC);
    ENCODER_B.init(GP_VCC);
    ENCODER_SW.init(GP_VCC);

    // ENCODER_A(P_VCC);
    // ENCODER_B(P_VCC);
    // ENCODER_SW(P_VCC);

  #ifdef ENCODER_GND
    ENCODER_GND.init(GPO_2mA);
    ENCODER_GND.clr();
  #endif
  }

  bool is_push()
  {
    bool detect =
    #ifdef ENCODER_GND
      ENCODER_SW.get();
  #else
      !ENCODER_SW.get();
  #endif
    if (detect) push = true;
    else if (push) { push = false; return true; }
    return false;
  }

  int scan()
  {
    int result;

    // состояние контактов A и B ?
    bool a = (bool)ENCODER_A.get();
    bool b = (bool)ENCODER_B.get();

    // если состояние контакта A изменилось и оно такое как B
    // то попорот по часовой, иначе против
    if (a != a0) count += a ^ b ? -1 : 1;

    // если состояние контакта B изменилось и и оно такое как A
    // то попорот против часовой, иначе по часовой
    if (b != b0) count += a ^ b ? 1 : -1;

    // запоминаем состояние контактов
    a0 = a;
    b0 = b;

    // один "щелчок" энкодера соответствует 4-рём позициям
    c1 = count >> 2;
    // если произошёл сдвиг на 4 позиции, фиксируем поворот
    result = c1 - c2;
    c2 = c1;
    return result;
  }
};
