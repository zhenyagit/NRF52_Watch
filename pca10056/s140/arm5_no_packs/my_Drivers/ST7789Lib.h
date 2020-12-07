#ifndef ST7789  
#define ST7789

#include "nrf_gpio.h"
#include "nrf_delay.h"
#include "nrf_drv_spi.h"

#include "app_error.h"
#define DCpin 43
#define RSpin 20
#define RGB565(r, g, b)         (((r & 0xF8) << 8) | ((g & 0xFC) << 3) | ((b & 0xF8) >> 3))
extern uint8_t * FBufferTemp8;
static  nrfx_spim_t spiips;  /**< SPI instance. */
static volatile bool spi_xferips;

void ST7789SetDrvSpi(nrfx_spim_t *_spiips,uint8_t * _FBuffer8,volatile bool *_spi_xfer); 
void ST7789SendCommand(uint8_t Cmd);
void ST7789SendData(uint8_t Data);
void ST7789HardReset(void);
void ST7789SoftReset(void);
void ST7789SleepMode(uint8_t Mode);
void ST7789ColorModeSet(uint8_t ColorMode);
void ST7789MemAccessModeSet(uint8_t Rotation, uint8_t VertMirror, uint8_t HorizMirror, uint8_t IsBGR);
void ST7789InversionMode(uint8_t Mode);
static void ST7789RowSet(uint16_t RowStart, uint16_t RowEnd);
void ST7789RamWrite(uint16_t *_FBuffer, uint16_t Len);
static void ST7789ColumnSet(uint16_t ColumnStart, uint16_t ColumnEnd);
void ST7789SetWindow(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1);
void ST7789FillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);
void ST7789FillScreen(uint16_t color);
void ST7789SetBL(uint8_t Value);
void ST7789DisplayPower(uint8_t On);
void ST7789Init(void);
void ST7789FastClearScreen(uint8_t *_FBuffer);
void ST7789FastSendBuffer(uint16_t *_FBuffer,uint8_t *_FBuffer8);
void ST7789DrawPixel(int16_t x, int16_t y, uint16_t color);
void ST7789DrawPartYX(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t *pBuff);
void ST7789DrawPartXY(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t *pBuff);

#endif 
