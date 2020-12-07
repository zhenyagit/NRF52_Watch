
#include "FrameBufferLib.h"

void FBFillAll(bool* _Fbuffer,bool color){
	for (int i=0; i<40*72;i++)
		_Fbuffer[i]=color;
}

void FBSetPixel(bool* _Fbuffer, int x, int y, bool OnOff){
	const int Width = 72;
	_Fbuffer[y*Width+ x] = OnOff;
}

	
void FBDrawLine(bool* _Fbuffer,uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1,bool sing) {
 
  int dx = abs(x1-x0), sx = x0<x1 ? 1 : -1;
  int dy = abs(y1-y0), sy = y0<y1 ? 1 : -1; 
  int err = (dx>dy ? dx : -dy)/2, e2;
 
  for(;;){
    FBSetPixel(_Fbuffer,x0,y0,sing);
    if (x0==x1 && y0==y1) break;
    e2 = err;
    if (e2 >-dx) { err -= dy; x0 += sx; }
    if (e2 < dy) { err += dx; y0 += sy; }
  }
}

void FBToDisplayBuffer(bool* _FBuffer, uint8_t* _newBuffer){

	for (int i =0; i <72*40/8; i++)
	{
		uint8_t temp=0;
		for (int j=0;j<8;j++)
		{
			uint8_t tbitt;
			int qtty = j * 72 + (72 * 8 * (i / 72))+i%72;
			if (_FBuffer[qtty])
				tbitt = 1;
			else
				tbitt = 0;

			temp = temp | (tbitt << (j));
			
		}
		_newBuffer[i] = temp;
	}
}
	
Coordinates PolarToDecard(int r, int f){
	Coordinates cor;
	cor.x = r*cos(f * (M_PI / 180.0));
	cor.y = r*sin(f * (M_PI / 180.0));
	return cor;
}
	
	

/*void FBAnimLine(int r,int x,int y){
	Coordinates cor;
	const uint8_t Wight = 72;
	const uint8_t Height = 40;
	bool *FBuffer1 = malloc(Wight*Height);
	uint8_t *DisplayFBuffer1 = malloc(Wight*Height/8);
	FBFillAll(FBuffer1,false);
		
	for(int i=0;i<360;i++)
	{
		//FBFillAll(FBuffer1,false);
		cor = PolarToDecard(r,i-91);
		FBDrawLine(FBuffer1,x,y,x+cor.x,y+cor.y,false);
		
		cor = PolarToDecard(r,i-90);
		FBDrawLine(FBuffer1,x,y,x+cor.x,y+cor.y,true);
		FBToDisplayBuffer(FBuffer1,DisplayFBuffer1);
		OledSendDataBuff(DisplayFBuffer1);
	
	}
	FBFillAll(FBuffer1,false);
	FBToDisplayBuffer(FBuffer1,DisplayFBuffer1);
	OledSendDataBuff(DisplayFBuffer1);
	
	free(FBuffer1);
	free(DisplayFBuffer1);
}
*/
