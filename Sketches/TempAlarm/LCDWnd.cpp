#ifndef LCD_WND_CPP
#define LCD_WND_CPP

#include "LCDWnd.h"
#include "LiquidCrystal.h"

  CLCDWnd::CLCDWnd( uint8_t _col, uint8_t _row, LiquidCrystal* _lcd ) 
                    : col(_col ), row( _row ), lcd( _lcd ){};

  void CLCDWnd::print( float _val  ){
    lcd->setCursor( col, row );
    lcd->print( _val, 1 );
  };

#endif
