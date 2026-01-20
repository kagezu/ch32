#include "lcd.h"
#include "adc.h"
#include "pinout.h"
#include "timer.h"

LCD lcd;
constexpr int ADC_CH  = 3;  // Номер канала ADC
constexpr int ADC_MAX = 1 << ADC::DEPTH;
constexpr int ADC_DIV = ((ADC_MAX << 1) / lcd.max_x());
constexpr int speed   = 10;

///////////////////////////////////////////////////////////////////////////////

#define ADC_CHANNEL_TO_USE ADC_Channel_0
#define ADC_GPIO_BANK      GPIOA
#define ADC_GPIO_PIN       GPIO_Pin_3
#define ADC_GPIO_CLOCK     RCC_APB2Periph_GPIOA

void ADC_Function_Init(void) {
  ADC_InitTypeDef ADC_InitStructure   = {0};
  GPIO_InitTypeDef GPIO_InitStructure = {0};

  RCC_APB2PeriphClockCmd(ADC_GPIO_CLOCK, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);

  // Determines clock for ADC.
  // At PCLK2 = 48MHz, ADCCLOCK = PCLK2 / 8 = 6MHz
  RCC_ADCCLKConfig(RCC_PCLK2_Div8);

  GPIO_InitStructure.GPIO_Pin  = ADC_GPIO_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
  GPIO_Init(ADC_GPIO_BANK, &GPIO_InitStructure);

  ADC_DeInit(ADC1);
  ADC_InitStructure.ADC_Mode               = ADC_Mode_Independent;
  ADC_InitStructure.ADC_ScanConvMode       = DISABLE;
  ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
  ADC_InitStructure.ADC_ExternalTrigConv   = ADC_ExternalTrigConv_None;
  ADC_InitStructure.ADC_DataAlign          = ADC_DataAlign_Right;
  ADC_InitStructure.ADC_NbrOfChannel       = 1;
  ADC_Init(ADC1, &ADC_InitStructure);

  ADC_Cmd(ADC1, ENABLE);

  ADC_BufferCmd(ADC1, ENABLE);  // enable buffer
}

u16 Get_ADC_Val(u8 ch) {
  u16 val = 0;
  ADC_RegularChannelConfig(ADC1, ch, 1, ADC_SampleTime_13Cycles5);
  ADC_SoftwareStartConvCmd(ADC1, ENABLE);
  while (!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC));
  val = ADC_GetConversionValue(ADC1);
  return val;
}


///////////////////////////////////////////////////////////////////////////////

int main(void) {
  lcd.init();
  lcd.font(sans_24, 0, 0);

  ADC_Function_Init();
  /*
    ADC_IN3(INA);
    ADC1->CTLR2 = ADC_ADON;
    ADC1->RSQR1 = 0;
    ADC1->RSQR3 = ADC_CH;
    ADC1->CTLR2 |= ADC_RSTCAL;
    while (ADC1->CTLR2 & ADC_RSTCAL);
    ADC1->SAMPTR2 = ADC_SMP_6;
    ADC1->CTLR1 |= (u32)1 << 26;
    ADC1->CTLR2 = ADC_SWSTART | ADC_CONT | ADC_ADON;
  */

  uint32_t x                    = 1;
  const uint16_t xx             = (lcd.max_x() + 1) >> 1;
  uint16_t x2                   = xx + (xx >> 1);
  uint16_t pix[lcd.max_y() + 1] = {};
  uint16_t yy                   = 0;
  uint16_t old                  = 0;
  uint16_t med                  = 0;

  lcd.area(xx - 1, 0, xx - 1, lcd.max_y(), RGB(32, 32, 255));

  while (true) {
    uint16_t kk = Get_ADC_Val(ADC_CH) / ADC_DIV;
    // uint16_t kk = ADC::value() / ADC_DIV;
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
