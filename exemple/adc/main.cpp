#include "lcd.h"
#include "adc.h"
#include "pinout.h"
#include "timer.h"

LCD lcd;
constexpr int ADC_CH  = 3;  // Номер канала ADC
constexpr int ADC_MAX = 1 << ADC::DEPTH;
constexpr int ADC_DIV = ((ADC_MAX << 1) / lcd.max_x());
constexpr int speed   = 5;
// int speed             = 5;

///////////////////////////////////////////////////////////////////////////////

Pin<PA, 0> EN_A;
Pin<PA, 1> EN_B;
Pin<PA, 2> USER_SW;

void init_encoder() {
  EN_A.init(GP_VCC);
  EN_B.init(GP_VCC);
  USER_SW.init(GP_VCC);

  tim2.TOP(500);
  tim2.enable(1);
  tim2.enable(2);
  tim2.encoder();
  tim2.enable();
  TIM2->CNT = 200;
}

int main(void) {
  init_encoder();
  lcd.init();
  lcd.font(sans_24, 0, 0);

  ADC::init(ADC_CH);
  ADC::delay(10);
  ADC::start();
  // ADC::gain(2);

  uint32_t x                    = 1;
  const uint16_t xx             = (lcd.max_x() + 1) >> 1;
  uint16_t x2                   = xx + (xx >> 1);
  uint16_t pix[lcd.max_y() + 1] = {};
  // uint16_t yy                   = 0;
  // uint16_t old                  = 0;
  uint16_t med                  = 0;

  lcd.area(xx - 1, 0, xx - 1, lcd.max_y(), RGB(32, 32, 255));

  while (true) {
    delay_us(tim2.CNT());
    uint16_t kk = ADC::value() / ADC_DIV;
    uint16_t y  = x % (lcd.max_y() + 1);

    if (!(x & ((1 << speed) - 1))) {
      uint16_t y2 = lcd.max_y() - (x >> speed) % (lcd.max_y() + 1);
      uint16_t k  = ADC::value();
      med         = ((med << 2) - med + k) >> 2;
      uint16_t k2 = k > med ? (k - med) / ADC_DIV : (med - k) / ADC_DIV;
      if (k2 > (xx >> 1) - 1) k2 = -k2;
      if (k2 > (xx >> 1) - 1) k2 = (xx >> 1) - 1;
      lcd.scroll(lcd.max_y() - y2 + 1);
      lcd.area(xx, y2, x2 - k2 - 1, y2, Black);
      lcd.area(x2 - k2, y2, x2 + k2, y2, RGB(64, 255, 64));
      lcd.area(x2 + k2 + 1, y2, lcd.max_x(), y2, Black);
    }

    lcd.color(Black);
    // lcd.w_line(pix[y] > old ? old : pix[y], y, pix[y] > old ? pix[y] : old);
    lcd.pixel(pix[y], y);
    lcd.color(RGB(127, 255, 255));
    // lcd.w_line(yy > kk ? kk : yy, y, yy > kk ? yy : kk);
    lcd.pixel(kk, y);

    // old    = pix[y];
    // yy     = kk;
    pix[y] = kk;

    x++;
  }
}
