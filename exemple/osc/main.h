#include "menu.h"

enum AppModeEnum :int {
  MODE_OSC,
  MODE_FFT,
  MODE_SPEC
};

// enum Trigger :int {
//   Front,
//   Cutoff,
//   Maximum
// };

const int _fq[] = { 1, 2, 5, 10, 20, 50, 100, 200, 500, 1000, 2000, 5000, 10000, 20000, 50000 };
const int _vsc[] = { 200, 100, 50, 20, 10, 5, 2, 1 };
const char *_vt[] = { "AC", "DC" };
const char *_ft[] = { "Off", "Sum" };
const char *_tt[] = { "RISING", "FALLING", "MAXIMUM", "MINIMUM" };

ADD_MINT(FqScale, "Time", 9, _fq);
ADD_MINT(VScale, "Voltage", 0, _vsc);
ADD_MTEXT(VType, "Current", 0, _vt);
ADD_MVALUE(ZeroLevel, "Setup zero", 0, -200, 200);
ADD_MVALUE(Segment, "Scale", 25, 15, 60);
ADD_MTEXT(FType, "Filter", 0, _ft);
ADD_MTEXT(TType, "Trigger", 0, _tt);
ADD_MITEM(MExit, "< < <");

MItem OSC_Menu = { "\bOSC", ListType, 0, 0, 0, 6 , { &MExit, &FqScale, &VScale, &VType, &ZeroLevel, &TType, &Segment } };
MItem FFT_Menu = { "\bFFT", ListType, 0, 0, 0, 3 , { &MExit, &FqScale, &VScale, &FType } };
MItem SPEC_Menu = { "\bSPEC", ListType, 0, 0, 0, 0 , { &MExit } };

MItem menu = { "", ListType, 0, 0, 0, 2, { &OSC_Menu, &FFT_Menu, &SPEC_Menu } };

#define RTC_REG_SAVED 2
