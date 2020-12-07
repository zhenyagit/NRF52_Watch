#ifndef SSD1306  
#define SSD1306
#include "nrf_drv_twi.h"
#include "nrf_log.h"
#include "nrf_delay.h"
#include<stdlib.h>

static nrf_drv_twi_t twissd1306;
void OledSetDrvTwi(nrf_drv_twi_t *drv);
void OledSendCommand(uint8_t _Command);
void OledSendData(uint8_t _Command);
void OledSendDataBuff(uint8_t* _Data);
void OledFillAll(uint8_t color);
void OledSetDisplayOn(const uint8_t on);
void OledInit(void) ;
void OledSetPixel(uint8_t x, uint8_t y, bool color);
void OledFrameAnim(void);


#endif 
