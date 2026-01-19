#include "lcd.h"
#include "adc.h"
#include "pinout.h"
#include "timer.h"

LCD lcd;
constexpr int ADC_CH  = 3;  // Номер канала ADC
constexpr int ADC_MAX = 1 << ADC::DEPTH;
constexpr int ADC_DIV = ((ADC_MAX << 1) / lcd.max_x());
constexpr int speed   = 8;

int main(void) {
  // delay_ms(100);
  lcd.init();
  lcd.font(sans_24, 0, 0);
  // lcd.background(Green);
  // lcd.clear();

  // ADC::init(ADC_CH);
  // ADC::delay(90);
  // ADC::start();

  ADC_IN3(INA);
  ADC1->CTLR2 = ADC_ADON;
  ADC1->RSQR1 = ADC_L_0;
  ADC1->RSQR3 = ADC_CH;
  ADC1->CTLR2 |= ADC_RSTCAL;
  while (ADC1->CTLR2 & ADC_RSTCAL);
  ADC1->SAMPTR2 = ADC_SMP_6;
  ADC1->CTLR2   = ADC_SWSTART | ADC_CONT | ADC_ADON;


  uint32_t x                    = 1;
  const uint16_t xx             = (lcd.max_x() + 1) >> 1;
  uint16_t x2                   = xx + (xx >> 1);
  uint16_t pix[lcd.max_y() + 1] = {};
  uint16_t yy                   = 0;
  uint16_t old                  = 0;
  uint16_t med                  = 0;

  lcd.area(xx - 1, 0, xx - 1, lcd.max_y(), RGB(32, 32, 255));

  while (true) {
    uint16_t kk = ADC::value() / ADC_DIV;
    uint16_t y  = x % (lcd.max_y() + 1);

    if (!(x & ((1 << speed) - 1))) {
      uint16_t y2 = (x >> speed) % (lcd.max_y() + 1);
      uint16_t k  = ADC::value();
      med         = ((med << 2) - med + k) >> 2;
      uint16_t k2 = k > med ? (k - med) / ADC_DIV : (med - k) / ADC_DIV;
      if (k2 > (xx >> 1) - 1) k2 = -k2;
      if (k2 > (xx >> 1) - 1) k2 = (xx >> 1) - 1;
      lcd.scroll(y2 + 1);
      lcd.area(xx, y2, x2 - k2 - 1, y2, Black);
      lcd.area(x2 - k2, y2, x2 + k2, y2, RGB(64, 255, 64));
      lcd.area(x2 + k2 + 1, y2, lcd.max_x(), y2, Black);
    }

    lcd.color(Black);
    lcd.w_line(pix[y] > old ? old : pix[y], y, pix[y] > old ? pix[y] : old);
    // lcd.pixel(pix[y], y);
    lcd.color(RGB(127, 255, 255));
    lcd.w_line(yy > kk ? kk : yy, y, yy > kk ? yy : kk);
    // lcd.pixel(kk, y);

    old    = pix[y];
    yy     = kk;
    pix[y] = kk;

    x++;
  }
}
