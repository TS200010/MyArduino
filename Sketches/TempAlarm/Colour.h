#ifndef COLOUR_H
#define COLOUR_H

// COLOURS AND COLOUR MANAGEMENT
// =============================

struct colour {
public:
int r, g, b;   
};

// See https://www.rapidtables.com/web/color/RGB_Color.html
const colour g_red =        {255, 0,   0   };
const colour g_green =      {0,   255, 0   };
const colour g_blue =       {0,   0,   255 };
const colour g_yellow =     {255, 255, 0   };
const colour g_cyan =       {0,   255, 255 };
const colour g_magenta =    {255, 0,   255 };
const colour g_black =      {0,   0,   0   };
const colour g_white =      {255, 255, 255 };

/*
class Colour {
public:
  Colour( const colour &c ){ col = c; };
private:
  colour col;
};
*/

#endif
