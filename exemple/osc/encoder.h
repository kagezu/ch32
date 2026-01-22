// #include "encoder.h"
#pragma once
#include "timer16.h"

Pin<PA, 0> EN_A;
Pin<PA, 1> EN_B;
Pin<PA, 2> USER_SW;

class Encoder {
public:
 volatile int count = 0, c1 = 0, c2 = 0;
  bool push         = 0;

public:
  void init() {
    EN_A.init(GP_VCC);
    EN_B.init(GP_VCC);
    USER_SW.init(GP_VCC);

  tim2.TOP(0xffff);
  tim2.enable(1);
  tim2.enable(2);
  tim2.encoder();
  tim2.enable();
  tim2.CNT(0);
  }

  bool is_push() {
    bool detect = !USER_SW.get();
    if (detect) push = true;
    else if (push) {
      push = false;
      return true;
    }
    return false;
  }

  int scan() {
    int result;
    count  = tim2.CNT();
    c1     = count >> 2;  // один "щелчок" энкодера соответствует 4-рём позициям
    result = c1 - c2;     // если произошёл сдвиг на 4 позиции, фиксируем поворот
    c2     = c1;
    return result;
  }
};
