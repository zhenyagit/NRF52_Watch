#include "OledSSD1306Lib.h"

void OledSetDrvTwi(nrf_drv_twi_t *drv){
	twissd1306 = *drv;
}
void OledSendCommand(uint8_t _Command){
	uint8_t command[2] = {0x00,_Command};
	ret_code_t err_code = nrf_drv_twi_tx(&twissd1306, 0x3c, command, sizeof(command),true);
}
void OledSendData(uint8_t _Command){
	uint8_t command[2] = {0x40,_Command};
	ret_code_t err_code = nrf_drv_twi_tx(&twissd1306, 0x3c, command, sizeof(command),true);

}
void OledSendDataBuff(uint8_t* _Data){
	uint8_t* total = malloc(73); // array to hold the result
	total[0] = 0x40;
	for(int i=0;i<5;i++)
	{
		memcpy(total + 1, _Data+i*72, 72); 
		OledSendCommand(0xb0+i);  
		OledSendCommand(0x00| (28 & 0xF));
		OledSendCommand(0x10| (28 >>4));
		ret_code_t err_code = nrf_drv_twi_tx(&twissd1306, 0x3c, total, sizeof(uint8_t)*73,true);
	}
	free(total);
        
}
void OledFillAll(uint8_t color){
	uint8_t Buffer[360];
	if (color ==1)
		for(int i =0;i<360;i++)
			Buffer[i] = 0xff;
	else
		for(int i =0;i<360;i++)
			Buffer[i] = 0x00;
	OledSendDataBuff(Buffer);
}
void OledSetDisplayOn(const uint8_t on){
    uint8_t value;
    if (on) {
        value = 0xAF; 
    } else {
        value = 0xAE; 
    }
    OledSendCommand(value);
}

void OledInit() {
    nrf_delay_ms(200);
		uint8_t InitCommands[23] =   {0xd5,0x80,0xa8,0x27,0xd3, 0, 0xad, 0x30,0x8d,0x14,0x40,0xa6,0xa4,0xa1,0xc8,0xda,0x12,0x81,0xaf,0xd9,0x22,0xdb,0x20};
		OledSetDisplayOn(0);
		for (int i =0; i<23; i++)
			OledSendCommand(InitCommands[i]);
		OledSetDisplayOn(1);
		OledFillAll(0);
			nrf_delay_ms(100);
}




void OledSetPixel(uint8_t x, uint8_t y, bool color){

  uint8_t page;
	uint8_t newy;
	uint8_t newx;
	
  if(y < 8)
	{
		page = 0;
	}
	else if (y >= 8)
	{
		page = (int)(y/8);       
	}
	newy = y%8;
	newx = x+28;
	OledSendCommand(0xb0+page);  
	OledSendCommand(0x00| (newx & 0xF));
	OledSendCommand(0x10| (newx >>4));
        
	if(color)
	{
		OledSendData(1 << newy);
	}
	else 
	{
		OledSendData(0 << newy);
	}  
}  
void OledFrameAnim(){
	int time=3;
	for(int i=0;i<40;i++)
	{
		OledSetPixel(0,i,true);
		nrf_delay_ms(time);
		OledSetPixel(0,i,false);
	}
	for(int i=0;i<72;i++)
	{
		OledSetPixel(i,39,true);
		nrf_delay_ms(time);
		OledSetPixel(i,39,false);
	}
	for(int i=39;i>0;i--)
	{
		OledSetPixel(69,i,true);
		nrf_delay_ms(time);
		OledSetPixel(69,i,false);
	}
	for(int i=71;i>0;i--)
	{
		OledSetPixel(i,0,true);
		nrf_delay_ms(time);
		OledSetPixel(i,0,false);
	}
}
