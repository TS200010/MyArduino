#ifndef LCD_WND_H
#define LCD_WND_H

#include "LiquidCrystal.h"

class CLCDWnd {
public:
  CLCDWnd( uint8_t _col, uint8_t _row, LiquidCrystal* _lcd );
  void print( float _val );

private:
  uint8_t col;
  uint8_t row;
  LiquidCrystal *lcd;
};

#endif
