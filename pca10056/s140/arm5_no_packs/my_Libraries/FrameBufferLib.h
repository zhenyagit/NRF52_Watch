#ifndef OledFrameBuffer  
#define OledFrameBuffer

#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include <stdint.h>
//#include "OledSSD1306Lib.h"

#define M_PI 3.14

typedef struct {
    int x,y;
} Coordinates;

void FBFillAll(bool* Fbuffer,bool color);
void FBSetPixel(bool* Fbuffer, int x, int y, bool OnOff);
void FBDrawLine(bool* Fbuffer,uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1,bool sing);
void FBToDisplayBuffer(bool* FBuffer, uint8_t* newBuffer);
//void FBAnimLine(int r,int x,int y);
Coordinates PolarToDecard(int r, int f);


#endif 
