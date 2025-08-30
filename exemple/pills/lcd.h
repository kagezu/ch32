#include "display.h"

// Цветовая модель

// #define RGB         bool   // 1 bit
// #define RGB         RGB12   // 4x4x4 bit
#define RGB         RGB16   // 5x6x5 bit
// #define RGB         RGB18   // 6x6x6 bit
// #define RGB         RGB32   // 8x8x8 bit


// Поворот дисплея

// #define LCD_ROT     R_0
// #define LCD_ROT     R_90
// #define LCD_ROT     R_180
#define LCD_ROT     R_270
// #define LCD_ROT     R_X
// #define LCD_ROT     R_Y
// #define LCD_ROT     R_EX
// #define LCD_ROT     R_EX_XY


// #define LCD Display<ST7735_SOFT<RGB>, LCD_ROT>   // RGB12, RGB16, RGB18
// #define LCD Display<ST7735<SPI1, RGB>, LCD_ROT>  // RGB12, RGB16, RGB18
// #define LCD Display<ILI9225_SOFT, LCD_ROT>       // RGB16
// #define LCD Display<ST7789<RGB>, LCD_ROT>        // RGB12, RGB16, RGB18
// #define LCD Display<ILI9486_8<RGB>, LCD_ROT>     // RGB16, RGB18
#define LCD Display<ILI9486_16<RGB>, LCD_ROT>    // RGB16, RGB18
// #define LCD Display<NT35510<RGB>, LCD_ROT>       // RGB16, RGB18, RBB24
// #define LCD Display<SSD1306, LCD_ROT>            // bool


// Шрифты

extern const struct Font arial_14;
extern const struct Font micro_5x6;
extern const struct Font system_5x7;
extern const struct Font standard_5x8;
extern const struct Font sans_24;
extern const struct Font o_sans_18b;
extern const struct Font serif_18i;
