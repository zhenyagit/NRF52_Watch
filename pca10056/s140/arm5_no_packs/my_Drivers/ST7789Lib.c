#include "ST7789Lib.h"
uint8_t * FBufferTemp8;

#include "blinker.h"
#include "nrf_log.h"

#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"
void ST7789SetDrvSpi(nrfx_spim_t *_spiips,uint8_t * _FBuffer8,volatile bool *_spi_xfer){
	spiips = *_spiips;
	spi_xferips = *_spi_xfer;
	nrf_gpio_cfg_output(DCpin);
	nrf_gpio_cfg_output(RSpin);
}
void ST7789SendCommand(uint8_t Cmd){
  nrf_gpio_pin_clear(DCpin);
  //ST77xx_CS_LOW();
	nrfx_spim_xfer_desc_t xfer_desc = NRFX_SPIM_SINGLE_XFER(&Cmd, 1, NULL, 0);
	nrfx_spim_xfer(&spiips, &xfer_desc, 0);
	//nrf_drv_spi_transfer(&spi, &Cmd, 1, NULL, 1);
	//ST77xx_CS_HIGH();
	//nrf_delay_us(5);
  
  nrf_gpio_pin_set(DCpin);
	//nrf_delay_us(5);
  
}

void ST7789SendData(uint8_t Data){
	nrfx_spim_xfer_desc_t xfer_desc = NRFX_SPIM_SINGLE_XFER(&Data, 1, NULL, 0);
	nrfx_spim_xfer(&spiips, &xfer_desc, 0);
	//nrf_drv_spi_transfer(&spi, &Data, 1, NULL, 1);
	//nrf_delay_ms(1);
	//nrf_delay_us(1);
  
}
void ST7789HardReset(void){
  nrf_gpio_pin_clear(RSpin);
  nrf_delay_ms(10);
  nrf_gpio_pin_set(RSpin);
  nrf_delay_ms(150);
}
void ST7789SoftReset(void){
  ST7789SendCommand(0x01);
  nrf_delay_ms(130);
}
void ST7789SleepMode(uint8_t Mode){
  if (Mode)
    ST7789SendCommand(0x10);
  else
    ST7789SendCommand(0x11);
  nrf_delay_ms(500);
}
void ST7789ColorModeSet(uint8_t ColorMode){
  ST7789SendCommand(0x3a);
  ST7789SendData(ColorMode & 0x77);  
}
void ST7789MemAccessModeSet(uint8_t Rotation, uint8_t VertMirror, uint8_t HorizMirror, uint8_t IsBGR){
  uint8_t Value;
  Rotation &= 7; 

  ST7789SendCommand(0x36);
  
  switch (Rotation)
  {
  case 0:
    Value = 0;
    break;
  case 1:
    Value = 0x40;
    break;
  case 2:
    Value = 0x80;
    break;
  case 3:
    Value = 0x40 | 0x80;
    break;
  case 4:
    Value = 0x20;
    break;
  case 5:
    Value = 0x20 | 0x40;
    break;
  case 6:
    Value = 0x20 | 0x80;
    break;
  case 7:
    Value = 0x20 | 0x40 | 0x80;
    break;
  }
  
  if (VertMirror)
    Value = 0x10;
  if (HorizMirror)
    Value = 0x04;
  
  if (IsBGR)
    Value |= 0x08;
  
  ST7789SendData(Value);
}
void ST7789InversionMode(uint8_t Mode){
  if (Mode)
    ST7789SendCommand(0x21);
  else
    ST7789SendCommand(0x20);
}
static void ST7789RowSet(uint16_t RowStart, uint16_t RowEnd){
  if (RowStart > RowEnd)
    return;
  if (RowEnd > 240)
    return;
  
  RowStart += 0;
  RowEnd += 0;
  
  ST7789SendCommand(0x2b);
  ST7789SendData(RowStart >> 8);  
  ST7789SendData(RowStart & 0xFF);  
  ST7789SendData(RowEnd >> 8);  
  ST7789SendData(RowEnd & 0xFF);  
}
void ST7789RamWrite(uint16_t *_FBuffer, uint16_t Len){
  while (Len--)
  {
		uint8_t array[2];
    array[0]= *_FBuffer & 0xff;
		array[1]=(*_FBuffer >> 8);
		nrfx_spim_xfer_desc_t xfer_desc = NRFX_SPIM_SINGLE_XFER(&array, 2, NULL, 0);
		nrfx_spim_xfer(&spiips, &xfer_desc, 0);
	}  
}
static void ST7789ColumnSet(uint16_t ColumnStart, uint16_t ColumnEnd){
  if (ColumnStart > ColumnEnd)
    return;
  if (ColumnEnd > 240)
    return;
  
  ColumnStart += 0;
  ColumnEnd += 0;
  
  ST7789SendCommand(0x2a);
  ST7789SendData(ColumnStart >> 8);  
  ST7789SendData(ColumnStart & 0xFF);  
  ST7789SendData(ColumnEnd >> 8);  
  ST7789SendData(ColumnEnd & 0xFF);  
}
void ST7789SetWindow(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1){
  ST7789ColumnSet(x0, x1);
  ST7789RowSet(y0, y1);
  
  ST7789SendCommand(0x2c);
}
void ST7789FillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color){
  if ((x >= 240) || (y >= 240))
    return;
  
  if ((x + w) > 240)
    w = 240 - x;
  
  if ((y + h) > 240)
    h = 240 - y;
	ST7789SetWindow(x, y, x + w - 1, y + h - 1);
	for (uint32_t i = 0; i < (h * w); i++)
    ST7789RamWrite(&color, 1);
//  uint32_t pos = 0, stp = 0;
//	nrfx_err_t err_code;
//  nrfx_spim_xfer_desc_t xfer_desc1;// = NRFX_SPIM_XFER_TX(_FBuffer, 240*240);
//	for (pos = 0; pos < len; pos += stp)
//	{
//		stp = MIN((len - pos), 65534);
//		xfer_desc1.tx_length = stp;
//		for (int i=0;i<stp/2;i++)
//		{
//			newTempBuff[i*2]=   color & 0xff;
//			newTempBuff[i*2+1]=(color >> 8);
//		
//		}
//		xfer_desc1.p_tx_buffer = newTempBuff;
//		err_code = nrfx_spim_xfer_dcx(&spiips, &xfer_desc1, NRFX_SPIM_FLAG_NO_XFER_EVT_HANDLER, 0);
//		APP_ERROR_CHECK(err_code);
//		blinkone();
	
}
void ST7789FillScreen(uint16_t color){
  ST7789FillRect(0, 0,  240, 240, color);
}
void ST7789SetBL(uint8_t Value){
	nrf_gpio_pin_set(13);
  //BackLight 
}
void ST7789DisplayPower(uint8_t On){
  if (On)
    ST7789SendCommand(0x29);
  else
    ST7789SendCommand(0x28);
}
void ST7789Init(){
  int ST7789_Width = 240;
  int ST7789_Height = 240;
 
  nrf_delay_ms(100);
  
  ST7789HardReset();

  ST7789SoftReset();
  ST7789SleepMode(0);

  ST7789ColorModeSet(0x50 | 0x05);
  nrf_delay_ms(10);

  ST7789MemAccessModeSet(4, 1, 1, 0);
  nrf_delay_ms(10);

  ST7789InversionMode(1);

  nrf_delay_ms(10);

  ST7789FillScreen(0x0000);
  
  ST7789SetBL(100);
  ST7789DisplayPower(1);

  nrf_delay_ms(100);

}
void ST7789FastClearScreen(uint8_t *_FBuffer){
	ST7789SetWindow(0, 0, 240 - 1, 240 - 1);
	memset(_FBuffer, 0, 57600);
  nrfx_spim_xfer_desc_t xfer_desc = NRFX_SPIM_SINGLE_XFER(_FBuffer, 57600, NULL, 0);
	nrfx_spim_xfer(&spiips, &xfer_desc, 0);
  nrfx_spim_xfer(&spiips, &xfer_desc, 0);

	//nrfx_spim_xfer(&spiips, &xfer_desc, 0);
	
}
void ST7789FastSendBuffer(uint16_t *_FBuffer,uint8_t *_FBuffer8){
	
	ST7789SetWindow(0, 0, 240 - 1, 240 - 1);
	uint32_t len = 240*240*2;
  uint32_t pos = 0, stp = 0;
	nrfx_err_t err_code;
  nrfx_spim_xfer_desc_t xfer_desc1;// = NRFX_SPIM_XFER_TX(_FBuffer, 240*240);
	for (pos = 0; pos < len; pos += stp)
	{
		stp = MIN((len - pos), 65534);
		xfer_desc1.tx_length = stp;
		for (int i=0;i<stp/2;i++)
		{
			_FBuffer8[i*2]=   _FBuffer[i+pos/2] & 0xff;
			_FBuffer8[i*2+1]=(_FBuffer[i+pos/2] >> 8);
		
		}
		xfer_desc1.p_tx_buffer = _FBuffer8;
		err_code = nrfx_spim_xfer_dcx(&spiips, &xfer_desc1, NRFX_SPIM_FLAG_NO_XFER_EVT_HANDLER, 0);
		APP_ERROR_CHECK(err_code);
	}
	// nrfx_spim_xfer(&spiips, &xfer_desc1, 0);
	// APP_ERROR_CHECK(nrfx_spim_xfer_dcx(&spiips, &xfer_desc1, NRFX_SPIM_FLAG_NO_XFER_EVT_HANDLER,0));

	
	//nrfx_spim_xfer(&spiips, &xfer_desc, 0);
}
void ST7789DrawPixel(int16_t x, int16_t y, uint16_t color){
  if ((x < 0) ||(x >= 240) || (y < 0) || (y >= 240))
    return;

  ST7789SetWindow(x, y, x, y);
  ST7789RamWrite(&color, 1);
}

void ST7789DrawPartYX(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t *pBuff){
  if ((x >= 240) || (y >= 240))
    return;
  
  if ((x + w - 1) >= 240)
    w = 240 - x;
  
  if ((y + h - 1) >= 240)
    h = 240 - y;

  ST7789SetWindow(x, y, x + w - 1, y + h - 1);

  for (uint32_t i = 0; i < (h * w); i++)
    ST7789RamWrite(pBuff++, 1);
}

void ST7789DrawPartXY(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t *pBuff){
  if ((x >= 240) || (y >= 240))
    return;
  
  if ((x + w - 1) >= 240)
    w = 240 - x;
  
  if ((y + h - 1) >= 240)
    h = 240 - y;

  for (uint16_t iy = y; iy < y + h; iy++)
  {
    ST7789SetWindow(x, iy, x + w - 1, iy + 1);
    for (x = w; x > 0; x--)
      ST7789RamWrite(pBuff++, 1);
  }
}


