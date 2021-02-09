//------------------------------------------------------------------------------
// This is Open source software. You can place this code on your site, but don't
// forget a link to my YouTube-channel: https://www.youtube.com/channel/UChButpZaL5kUUl_zTyIDFkQ
// ��� ����������� ����������� ���������������� ��������. �� ������ ���������
// ��� �� ����� �����, �� �� �������� ������� ������ �� ��� YouTube-����� 
// "����������� � ���������" https://www.youtube.com/channel/UChButpZaL5kUUl_zTyIDFkQ
// �����: �������� ������ / Nadyrshin Ruslan
//------------------------------------------------------------------------------
#include "dispcolor.h"

//==============================================================================
// ��������� ����� ������� 2 �������� int16_t
//==============================================================================
static void SwapInt16Values(int16_t *pValue1, int16_t *pValue2)
{
  int16_t TempValue = *pValue1;
  *pValue1 = *pValue2;
  *pValue2 = TempValue;
}
//==============================================================================

//==============================================================================
// ��������� ������������� ������� �������
//==============================================================================
void dispcolor_SetBrightness(uint8_t Value)
{
  if (Value > 100)
    Value = 100;
  ST7789SetBL(Value);
}
//==============================================================================


//==============================================================================
// ��������� ���������� 1 ������� �������
//==============================================================================
void dispcolor_DrawPixel(int16_t x, int16_t y, uint16_t color)
{
  ST7789DrawPixel(x, y, color);
}
//==============================================================================

//==============================================================================
// ��������� ���������� �������������� ������ color
//==============================================================================
void dispcolor_FillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color)
{
  ST7789FillRect(x, y, w, h, color);
}
//==============================================================================


//==============================================================================
// ��������� ����������� ����� ������ color
//==============================================================================
void dispcolor_FillScreen(uint16_t color)
{
  ST7789FillScreen(color);
}
//==============================================================================


//==============================================================================
// ��������� ������� ����� (����������� ������)
//==============================================================================
void dispcolor_ClearScreen(void)
{
  ST7789FillScreen(0);
}
//==============================================================================


//==============================================================================
// ��������� ������ ������ ����� �� ������� ��������� ������� (�����������)
//==============================================================================
static void dispcolor_DrawLine_Slow(int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color) 
{
  const int16_t deltaX = abs(x2 - x1);
  const int16_t deltaY = abs(y2 - y1);
  const int16_t signX = x1 < x2 ? 1 : -1;
  const int16_t signY = y1 < y2 ? 1 : -1;

  int16_t error = deltaX - deltaY;

  ST7789DrawPixel(x2, y2, color);

  while (x1 != x2 || y1 != y2) 
  {
    ST7789DrawPixel(x1, y1, color);
    const int16_t error2 = error * 2;
 
    if (error2 > -deltaY) 
    {
      error -= deltaY;
      x1 += signX;
    }
    if (error2 < deltaX)
    {
      error += deltaX;
      y1 += signY;
    }
  }
}
//==============================================================================


//==============================================================================
// ��������� ������ ������ ����� �� �������
//==============================================================================
void dispcolor_DrawLine(int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color) 
{
  // ������������ �����
  if (x1 == x2)
  {
    // ������������ ����� ������� �������
    if (y1 > y2)
      ST7789FillRect(x1, y2, 1, y1 - y2 + 1, color);
    else
      ST7789FillRect(x1, y1, 1, y2 - y1 + 1, color);
    return;
  }
  
  // �������������� �����
  if (y1 == y2)
  {
    // ������������ ����� ������� �������
    if (x1 > x2)
      ST7789FillRect(x2, y1, x1 - x2 + 1, 1, color);
    else
      ST7789FillRect(x1, y1, x2 - x1 + 1, 1, color);
    return;
  }
  
  // ������������ ����� �����������
  dispcolor_DrawLine_Slow(x1, y1, x2, y2, color);
}
//==============================================================================


//==============================================================================
// ��������� ������ ������������� �� �������
//==============================================================================
void dispcolor_DrawRectangle(int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color) 
{
  dispcolor_DrawLine(x1, y1, x1, y2, color);
  dispcolor_DrawLine(x2, y1, x2, y2, color);
  dispcolor_DrawLine(x1, y1, x2, y1, color);
  dispcolor_DrawLine(x1, y2, x2, y2, color);
}
//==============================================================================


//==============================================================================
// ��������� ������ ����������� ������������� �� �������
//==============================================================================
void dispcolor_DrawRectangleFilled(int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t fillcolor) 
{
  if (x1 > x2)
    SwapInt16Values(&x1, &x2);
  if (y1 > y2)
    SwapInt16Values(&y1, &y2);
  
  dispcolor_FillRect(x1, y1, x2 - x1, y2 - y1, fillcolor);
}
//==============================================================================


//==============================================================================
// ��������� ������ ���������� �� �������. x0 � y0 - ���������� ������ ����������
//==============================================================================
void dispcolor_DrawCircle(int16_t x0, int16_t y0, int16_t radius, uint16_t color) 
{
  int x = 0;
  int y = radius;
  int delta = 1 - 2 * radius;
  int error = 0;

  while (y >= 0)
  {
    ST7789DrawPixel(x0 + x, y0 + y, color);
    ST7789DrawPixel(x0 + x, y0 - y, color);
    ST7789DrawPixel(x0 - x, y0 + y, color);
    ST7789DrawPixel(x0 - x, y0 - y, color);
    error = 2 * (delta + y) - 1;

    if (delta < 0 && error <= 0) 
    {
      ++x;
      delta += 2 * x + 1;
      continue;
    }
	
    error = 2 * (delta - x) - 1;
		
    if (delta > 0 && error > 0) 
    {
      --y;
      delta += 1 - 2 * y;
      continue;
    }
	
    ++x;
    delta += 2 * (x - y);
    --y;
  }
}
//==============================================================================


//==============================================================================
// ��������� ������ ����������� ���������� �� �������. x0 � y0 - ���������� ������ ����������
//==============================================================================
void dispcolor_DrawCircleFilled(int16_t x0, int16_t y0, int16_t radius, uint16_t fillcolor) 
{
  int x = 0;
  int y = radius;
  int delta = 1 - 2 * radius;
  int error = 0;

  while (y >= 0)
  {
    dispcolor_DrawLine(x0 + x, y0 - y, x0 + x, y0 + y, fillcolor);
    dispcolor_DrawLine(x0 - x, y0 - y, x0 - x, y0 + y, fillcolor);
    error = 2 * (delta + y) - 1;

    if (delta < 0 && error <= 0) 
    {
      ++x;
      delta += 2 * x + 1;
      continue;
    }
	
    error = 2 * (delta - x) - 1;
		
    if (delta > 0 && error > 0) 
    {
      --y;
      delta += 1 - 2 * y;
      continue;
    }
	
    ++x;
    delta += 2 * (x - y);
    --y;
  }
}
//==============================================================================


void dispcolor_DrawFont(uint8_t x, uint8_t y, uint16_t color,const uint8_t mass[])
{
	uint16_t PixBuff[240];
	uint8_t r = color>>8 & 0xF8;
	uint8_t g = color>>3 & 0xFC;
	uint8_t b = color<<3 & 0xF8;
	for (int i=0;i<mass[1];i++)
	{
		for (int j=0; j<mass[0];j++)
			PixBuff[j] = RGB565(mass[2+i*mass[0]+j]*r/255,mass[2+i*mass[0]+j]*g/255,mass[2+i*mass[0]+j]*b/255);
		dispcolor_DrawPartXY(x, y+i, mass[0], 1, PixBuff);
	}
}	
void dispcolor_DrawFontInBuff(uint8_t x, uint8_t y, uint16_t color,uint8_t number, uint16_t * FrameBuffer)
{
	
	uint8_t r = color>>8 & 0xF8;
	uint8_t g = color>>3 & 0xFC;
	uint8_t b = color<<3 & 0xF8;
	uint8_t width = numbers[number][0];
	uint8_t heigth = numbers[number][1];	
	for (int i=0;i<heigth;i++)
	{
		for (int j=0; j<width;j++)
		{
			if (j+x<240)
				if (i+y<240)
					FrameBuffer[i*240+j+x+y*240] = RGB565(numbers[number][2+i*width+j]*r/255,numbers[number][2+i*width+j]*g/255,numbers[number][2+i*width+j]*b/255);
		}
	}
}
void dispcolor_DrawMain()
{
	dispcolor_DrawFont(0,100, WHITE, number7);
	
}
//==============================================================================
// ��������� ���������� ������������� ������� �� ������ (������� ���������� ������� Y, X
//==============================================================================
void dispcolor_DrawPartYX(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t *pBuff)
{
  ST7789DrawPartYX(x, y, w, h, pBuff);
}
//==============================================================================


//==============================================================================
// ��������� ���������� ������������� ������� �� ������ (������� ���������� ������� X, Y
//==============================================================================
void dispcolor_DrawPartXY(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t *pBuff)
{
  ST7789DrawPartXY(x, y, w, h, pBuff);
}
//==============================================================================


//==============================================================================
// ������� ������ ������� Char �� �������. ���������� ������ ����������� �������
//==============================================================================
static uint8_t dispcolor_DrawChar_General(int16_t X, int16_t Y, uint8_t FontID, 
                                          uint8_t Char, uint16_t TextColor, 
                                          uint16_t BgColor, uint8_t TransparentBg)
{
  // ��������� �� ����������� ����������� ������� ������
  uint8_t *pCharTable = font_GetFontStruct(FontID, Char);
  uint8_t CharWidth = font_GetCharWidth(pCharTable);    // ������ �������
  uint8_t CharHeight = font_GetCharHeight(pCharTable);  // ������ �������
  pCharTable += 2;
  
  if (FontID == FONTID_6X8M)
  {
    for (uint8_t row = 0; row < CharHeight; row++)
    {
      for (uint8_t col = 0; col < CharWidth; col++)
      {
        if (pCharTable[row] & (1 << (7 - col)))
          dispcolor_DrawPixel(X + col, Y + row, TextColor);
        else if (!TransparentBg)
          dispcolor_DrawPixel(X + col, Y + row, BgColor);
      }
    }
  }
  else
  {
    for (uint8_t row = 0; row < CharHeight; row++)
    {
      for (uint8_t col = 0; col < CharWidth; col++)
      {
        if (col < 8)
        {
          if (pCharTable[row * 2] & (1 << (7 - col)))
            dispcolor_DrawPixel(X + col, Y + row, TextColor);
          else if (!TransparentBg)
            dispcolor_DrawPixel(X + col, Y + row, BgColor);
        }
        else
        {
          if (pCharTable[(row * 2) + 1] & (1 << (15 - col)))
            dispcolor_DrawPixel(X + col, Y + row, TextColor);
          else if (!TransparentBg)
            dispcolor_DrawPixel(X + col, Y + row, BgColor);
        }
      }
    }
  }
  
  return CharWidth;
}
//==============================================================================


//==============================================================================
// ������� ������ ������� Char �� �������. ���������� ������ ����������� �������
//==============================================================================
uint8_t dispcolor_DrawChar(int16_t X, int16_t Y, uint8_t FontID, uint8_t Char, 
                           uint16_t TextColor)
{
  return dispcolor_DrawChar_General(X, Y, FontID, Char, TextColor, 0, 1);
}
//==============================================================================


//==============================================================================
// ������� ������ ������� Char �� �������. ���������� ������ ����������� �������
//==============================================================================
uint8_t dispcolor_DrawChar_Bg(int16_t X, int16_t Y, uint8_t FontID, uint8_t Char, 
                              uint16_t TextColor, uint16_t BgColor)
{
  return dispcolor_DrawChar_General(X, Y, FontID, Char, TextColor, BgColor, 0);
}
//==============================================================================


//==============================================================================
// ������� ������ ������ �� ������ Str �� �������
//==============================================================================
static int16_t dispcolor_DrawString_General(int16_t X, int16_t Y, uint8_t FontID, 
                                         uint8_t *Str, uint16_t TextColor, 
                                         uint16_t BgColor, uint8_t TransparentBg)
{
  uint8_t done = 0;             // ���� ��������� ������
  int16_t Xstart = X;           // ���������� ���� ����� ���������� ������� ��� �������� �� ����� ������
  uint8_t StrHeight = 8;        // ������ �������� � �������� ��� �������� �� ��������� ������

  // ����� ������
  while (!done)
  {
    switch (*Str)
    {
    case '\0':  // ����� ������
      done = 1;
      break;
    case '\n':  // ������� �� ��������� ������
      Y += StrHeight;
      break;
    case '\r':  // ������� � ������ ������
      X = Xstart;
      break;
    default:    // ������������ ������
      if (TransparentBg)
        X += dispcolor_DrawChar(X, Y, FontID, *Str, TextColor);
      else
        X += dispcolor_DrawChar_Bg(X, Y, FontID, *Str, TextColor, BgColor);
        
      StrHeight = font_GetCharHeight(font_GetFontStruct(FontID, *Str));
      break;
    }
    Str++;
  }
  return X;
}
//==============================================================================

void dispcolor_DrawBMP(uint8_t x, uint8_t y, const uint8_t * img, uint16_t *ST7789FB)
{
	uint32_t imageOffset = img[10] | (img[11] << 8) | (img[12] << 16) | (img[13] << 24);
	uint32_t imageWidth  = img[18] | (img[19] << 8) | (img[20] << 16) | (img[21] << 24);
	uint32_t imageHeight = img[22] | (img[23] << 8) | (img[24] << 16) | (img[25] << 24);
	uint16_t imagePlanes = img[26] | (img[27] << 8);

	uint16_t imageBitsPerPixel = img[28] | (img[29] << 8);
	uint32_t imageCompression  = img[30] | (img[31] << 8) | (img[32] << 16) | (img[33] << 24);
	int cursore=imageOffset;
	uint8_t imageRow[(240 * 3 + 3) & ~3];
	uint16_t PixBuff[240];

	for (uint32_t i = y; (i < imageHeight)&&(i < 240); i++)
	{
		memcpy(imageRow,img+cursore, (imageWidth * 3 + 3) & ~3);
		cursore = cursore+((imageWidth * 3 + 3) & ~3) ;
			
		uint32_t rowIdx = 0;
		for (uint32_t j = 0; (j < imageWidth)&&(j < 240); j++)
		{
			uint8_t b = imageRow[rowIdx++];
			uint8_t g = imageRow[rowIdx++];
			uint8_t r = imageRow[rowIdx++];
			PixBuff[x] = RGB565(r, g, b);
			ST7789FB[i*240+j] = RGB565(r, g, b);
		}
	}
}
//==============================================================================
// ������� ������ ������ �� ������ Str �� �������
//==============================================================================
int16_t dispcolor_DrawString(int16_t X, int16_t Y, uint8_t FontID, uint8_t *Str, uint16_t TextColor)
{
  return dispcolor_DrawString_General(X, Y, FontID,  Str, TextColor, 0, 1);
}
//==============================================================================


//==============================================================================
// ������� ������ ������ �� ������ Str �� �������
//==============================================================================
int16_t dispcolor_DrawString_Bg(int16_t X, int16_t Y, uint8_t FontID, uint8_t *Str, uint16_t TextColor, uint16_t BgColor)
{
  return dispcolor_DrawString_General(X, Y, FontID,  Str, TextColor, BgColor, 0);
}
//==============================================================================


//==============================================================================
// ��������� ������� �� ������� ��������������� ������
//==============================================================================
int16_t dispcolor_printf(int16_t X, int16_t Y, uint8_t FontID, uint16_t TextColor, const char *args, ...)
{
  char StrBuff[100];
  
  va_list ap;
  va_start(ap, args);
  char len = vsnprintf(StrBuff, sizeof(StrBuff), args, ap);
  va_end(ap);
  
  return dispcolor_DrawString(X, Y, FontID, (uint8_t *)StrBuff, TextColor);
}
//==============================================================================


//==============================================================================
// ��������� ������� �� ������� ��������������� ������
//==============================================================================
int16_t dispcolor_printf_Bg(int16_t X, int16_t Y, uint8_t FontID, uint16_t TextColor, uint16_t BgColor, const char *args, ...)
{
  char StrBuff[100];
  
  va_list ap;
  va_start(ap, args);
  char len = vsnprintf(StrBuff, sizeof(StrBuff), args, ap);
  va_end(ap);
  
  return dispcolor_DrawString_Bg(X, Y, FontID, (uint8_t *)StrBuff, TextColor, BgColor);
}
//==============================================================================
