// #define WR_FORSED     // Аппаратное тактирование
#include "display.h"

// Цветовая модель

// #define RGB         RGB12   // 4x4x4 bit
#define RGB         RGB16   // 5x6x5 bit
// #define RGB         RGB18   // 6x6x6 bit
// #define RGB         RGB24   // 8x8x8 bit


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
// #define LCD Display<ST7735<SPI<2>, RGB>, LCD_ROT>  // RGB12, RGB16, RGB18
// #define LCD Display<ILI9225_SOFT, LCD_ROT>       // RGB16
// #define LCD Display<ILI9225<SPI<2>>, LCD_ROT>      // RGB16
// #define LCD Display<ST7789<RGB>, LCD_ROT>        // RGB12, RGB16, RGB18
// #define LCD Display<ILI9486_8<RGB>, LCD_ROT>     // RGB16, RGB18
// #define LCD Display<ILI9486_16<RGB>, LCD_ROT>    // RGB16, RGB18
#define LCD Display<NT35510<RGB>, LCD_ROT>       // RGB16, RGB18, RBB24


// Шрифты

extern const struct Font arial_14;
extern const struct Font micro_5x6;
extern const struct Font system_5x7;
extern const struct Font standard_5x8;
extern const struct Font sans_24;
extern const struct Font serif_18i;


// Палитра

RGB color[] = {
  RGB(0xCD5C5C),  // #CD5C5C
  RGB(0xF08080),  // #F08080
  RGB(0xFA8072),  // #FA8072
  RGB(0xE9967A),  // #E9967A
  RGB(0xFFA07A),  // #FFA07A
  RGB(0xDC143C),  // #DC143C
  RGB(0xFF0000),  // #FF0000
  RGB(0xB22222),  // #B22222
  RGB(0x8B0000),  // #8B0000
  RGB(0xFFC0CB),  // #FFC0CB
  RGB(0xFFB6C1),  // #FFB6C1
  RGB(0xFF69B4),  // #FF69B4
  RGB(0xFF1493),  // #FF1493
  RGB(0xC71585),  // #C71585
  RGB(0xDB7093),  // #DB7093
  RGB(0xFFA07A),  // #FFA07A
  RGB(0xFF7F50),  // #FF7F50
  RGB(0xFF6347),  // #FF6347
  RGB(0xFF4500),  // #FF4500
  RGB(0xFF8C00),  // #FF8C00
  RGB(0xFFA500),  // #FFA500
  RGB(0xFFD700),  // #FFD700
  RGB(0xFFFF00),  // #FFFF00
  RGB(0xFFFFE0),  // #FFFFE0
  RGB(0xFFFACD),  // #FFFACD
  RGB(0xFAFAD2),  // #FAFAD2
  RGB(0xFFEFD5),  // #FFEFD5
  RGB(0xFFE4B5),  // #FFE4B5
  RGB(0xFFDAB9),  // #FFDAB9
  RGB(0xEEE8AA),  // #EEE8AA
  RGB(0xF0E68C),  // #F0E68C
  RGB(0xBDB76B),  // #BDB76B
  RGB(0xE6E6FA),  // #E6E6FA
  RGB(0xD8BFD8),  // #D8BFD8
  RGB(0xDDA0DD),  // #DDA0DD
  RGB(0xEE82EE),  // #EE82EE
  RGB(0xDA70D6),  // #DA70D6
  RGB(0xFF00FF),  // #FF00FF
  RGB(0xFF00FF),  // #FF00FF
  RGB(0xBA55D3),  // #BA55D3
  RGB(0x9370DB),  // #9370DB
  RGB(0x8A2BE2),  // #8A2BE2
  RGB(0x9400D3),  // #9400D3
  RGB(0x9932CC),  // #9932CC
  RGB(0x8B008B),  // #8B008B
  RGB(0x800080),  // #800080
  RGB(0x4B0082),  // #4B0082
  RGB(0x6A5ACD),  // #6A5ACD
  RGB(0x483D8B),  // #483D8B
  RGB(0xFFF8DC),  // #FFF8DC
  RGB(0xFFEBCD),  // #FFEBCD
  RGB(0xFFE4C4),  // #FFE4C4
  RGB(0xFFDEAD),  // #FFDEAD
  RGB(0xF5DEB3),  // #F5DEB3
  RGB(0xDEB887),  // #DEB887
  RGB(0xD2B48C),  // #D2B48C
  RGB(0xBC8F8F),  // #BC8F8F
  RGB(0xF4A460),  // #F4A460
  RGB(0xDAA520),  // #DAA520
  RGB(0xB8860B),  // #B8860B
  RGB(0xCD853F),  // #CD853F
  RGB(0xD2691E),  // #D2691E
  RGB(0x8B4513),  // #8B4513
  RGB(0xA0522D),  // #A0522D
  RGB(0xA52A2A),  // #A52A2A
  RGB(0x800000),  // #800000
  RGB(0x000000),  // #000000
  RGB(0xADFF2F),  // #ADFF2F
  RGB(0x7FFF00),  // #7FFF00
  RGB(0x7CFC00),  // #7CFC00
  RGB(0x00FF00),  // #00FF00
  RGB(0x32CD32),  // #32CD32
  RGB(0x98FB98),  // #98FB98
  RGB(0x90EE90),  // #90EE90
  RGB(0x00FA9A),  // #00FA9A
  RGB(0x00FF7F),  // #00FF7F
  RGB(0x00FFFF),  // #00FFFF
  RGB(0x00FFFF),  // #00FFFF
  RGB(0xE0FFFF),  // #E0FFFF
  RGB(0xAFEEEE),  // #AFEEEE
  RGB(0x7FFFD4),  // #7FFFD4
  RGB(0x40E0D0),  // #40E0D0
  RGB(0x48D1CC),  // #48D1CC
  RGB(0x00CED1),  // #00CED1
  RGB(0x5F9EA0),  // #5F9EA0
  RGB(0x4682B4),  // #4682B4
  RGB(0xB0C4DE),  // #B0C4DE
  RGB(0xB0E0E6),  // #B0E0E6
  RGB(0xADD8E6),  // #ADD8E6
  RGB(0x87CEEB),  // #87CEEB
  RGB(0x87CEFA),  // #87CEFA
  RGB(0x00BFFF),  // #00BFFF
  RGB(0x1E90FF),  // #1E90FF
  RGB(0x6495ED),  // #6495ED
  RGB(0x7B68EE),  // #7B68EE
  RGB(0x4169E1),  // #4169E1
  RGB(0x0000FF),  // #0000FF
  RGB(0x0000CD),  // #0000CD
  RGB(0x00008B),  // #00008B
  RGB(0x000080),  // #000080
  RGB(0x191970),  // #191970
  RGB(0xFFFFFF),  // #FFFFFF
  RGB(0xFFFAFA),  // #FFFAFA
  RGB(0xF0FFF0),  // #F0FFF0
  RGB(0xF5FFFA),  // #F5FFFA
  RGB(0xF0FFFF),  // #F0FFFF
  RGB(0xF0F8FF),  // #F0F8FF
  RGB(0xF8F8FF),  // #F8F8FF
  RGB(0xF5F5F5),  // #F5F5F5
  RGB(0xFFF5EE),  // #FFF5EE
  RGB(0xF5F5DC),  // #F5F5DC
  RGB(0xFDF5E6),  // #FDF5E6
  RGB(0xFFFAF0),  // #FFFAF0
  RGB(0xFFFFF0),  // #FFFFF0
  RGB(0xFAEBD7),  // #FAEBD7
  RGB(0xFAF0E6),  // #FAF0E6
  RGB(0xFFF0F5),  // #FFF0F5
  RGB(0xFFE4E1),  // #FFE4E1
  RGB(0xDCDCDC),  // #DCDCDC
  RGB(0xD3D3D3),  // #D3D3D3
  RGB(0xC0C0C0),  // #C0C0C0
  RGB(0xA9A9A9),  // #A9A9A9
  RGB(0x808080),  // #808080
  RGB(0x696969),  // #696969
  RGB(0x778899),  // #778899
  RGB(0x708090),  // #708090
  RGB(0x2F4F4F),  // #2F4F4F
  RGB(0x000000),  // #000000
};
