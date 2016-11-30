/* FILE:    HCMAX7219.cpp
   DATE:    30/11/16
   VERSION: 0.3
   AUTHOR:  Andrew Davies
   
11/03/15 version 0.1: Original version
13/04/15 version 0.2: Updated timings to work with 1.x.x versions of Arduino IDE
30/11/16 version 0.3: Updated to support ESP8266
					  Text font moved to program memory to save ram.

Library for WS2812 serial RGB LEDs.

You may copy, alter and reuse this code in any way you like, but please leave
reference to HobbyComponents.com in your comments if you redistribute this code.
This software may not be used directly for the purpose of selling products that
directly compete with Hobby Components Ltd's own range of products.

THIS SOFTWARE IS PROVIDED "AS IS". HOBBY COMPONENTS MAKES NO WARRANTIES, WHETHER
EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE, ACCURACY OR LACK OF NEGLIGENCE.
HOBBY COMPONENTS SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR ANY DAMAGES,
INCLUDING, BUT NOT LIMITED TO, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR ANY
REASON WHATSOEVER.
*/

#include "HCWS2812.h"
#include "Arduino.h"

/* Output buffer */
byte RGBBuffer[3][NUMBEROFLEDS];


/* Constructor to initialise the GPIO and library */
HCWS2812::HCWS2812(void)
{
  /* If device is ESP8266 set pin D2 */
  #if defined (ESP8266)
  pinMode(DOUT_PIN, OUTPUT);
  #else
  /* Else if Arduino board set pin D8 to an output */
  DOUT_DDR |= DOUT_PIN;
  #endif
  /* Clear the output buffer with the selected background colour */
  SetBG(0, 0, 0);
  ClearBuffer();
  
#ifdef DOTMATRIX  
  SetFontFG(100, 100, 100);
  SetFontBG(0, 0, 0);
#endif
}

/* Loads a string of text to the output buffer */
#ifdef DOTMATRIX 
void HCWS2812::print(char TextString[], unsigned int Offset)
{
  _CopyToBuffer(TextString, strlen(TextString), Offset);
}  
#endif


 #ifdef DOTMATRIX 
/* Loads a decimal number into the output buffer. 
   Number is a decimal number of type int.
   Offset is the display column number to to start the text from. */
void HCWS2812::print(long number, unsigned int Offset)
{ 
  char Digits[10];
  byte index = 0;
  long Temp;

  Temp = number;
  
  /*Is the number negative ? If so then remove the sign */
  if (Temp < 0)
    Temp *= -1;

  /* Is the number zero ? */	
  if (Temp == 0)
  {
    Digits[index] = '0';
    index++;
  }else
  {
	/* Convert the number to an ASCII decimal string */
    while (Temp)
    {
      Digits[index] = (Temp % 10) + 48;
      Temp /= 10;
      index++;
    } 
  }

  /* If the number was negative add the sign */ 
  if (number < 0)
  {
    Digits[index] = '-';
    index++;
  }

  _ReverseArray(Digits, index);
  
  /* Copy the number to the output buffer */
  _CopyToBuffer(Digits, index, Offset);
}
#endif


 #ifdef DOTMATRIX 
/* Loads a decimal number with decimal point into the output buffer. 
   Number is a decimal number of type int.
   decimalPlace is the digit position to add a decimal point to.
   Offset is the display column number to to start the text from */
void HCWS2812::print(long number, byte decimalPlace, unsigned int Offset)
{
  char Digits[10];
  unsigned charindex;
  byte index = 0;
  long Temp;
  unsigned int bufferindex;

  Temp = number;
  
  /*Is the number negative ? If so then remove the sign */
  if (Temp < 0)
    Temp *= -1;

  /* Is the number zero ? */	
  if (Temp == 0)
  {
    Digits[index] = '0';
    index++;
  }else
  {
	/* Convert the number to an ASCII decimal string */
    while (Temp)
    {
	  if(index == decimalPlace)
	  {
		Digits[index] = '.';
		index++;
	  }
      Digits[index] = (Temp % 10) + 48;
      Temp /= 10;
      index++;
    } 
  }
  
  /* If decimal place is at the beginning of the number then pad it 
     with a zero */
  if (decimalPlace == index)
  {
    Digits[index] = '.';
    index++;
	Digits[index] = '0';
    index++;
  }
   
  /* If the number was negative add the sign */ 
  if (number < 0)
  {
    Digits[index] = '-';
    index++;
  }
  
  _ReverseArray(Digits, index);
  
  /* Copy the number to the output buffer */  
  _CopyToBuffer(Digits, index, Offset);
}
#endif


/* Clears the output buffer using the background colour */
 void HCWS2812::ClearBuffer(void)
{ 
  int index;

  for(index = 0; index < NUMBEROFLEDS; index++)
  {
	RGBBuffer[GREEN][index] = _BGColour[GREEN];
	RGBBuffer[RED][index] = _BGColour[RED];
	RGBBuffer[BLUE][index] = _BGColour[BLUE];
  }
} 

 #ifdef DOTMATRIX 
/* Copy a character array to the output buffer */
 void HCWS2812::_CopyToBuffer(char Array[], int ArraySize, unsigned int Offset)
 {
  unsigned int bufferindex;
  unsigned int charindex;
  byte CurCharacter;
  byte PixelIndex;

  if(Offset)
  {
    /* Set output buffer pointer */
    bufferindex = Offset * 8;
    if(bufferindex >= NUMBEROFLEDS)
    {
	  bufferindex = NUMBEROFLEDS;
    }
    
	/*If text runs beyond the output buffer then crop it */
    charindex = 0;
    if (Offset > (NUMBEROFLEDS / 8))
      charindex = Offset - (NUMBEROFLEDS / 8);

    /* Copy the text into the buffer */
	while(bufferindex && charindex < (ArraySize * 8))
	{
	  CurCharacter = Array[charindex / 8] - 32;

	  for(PixelIndex = 0; PixelIndex < 8; PixelIndex++)
	  {
		bufferindex--;
		if((pgm_read_byte ( &(Font8x8[CurCharacter][(charindex%8)])) >> PixelIndex) & 0x01)
		{
		  RGBBuffer[BLUE][bufferindex] = _FontFGColour[BLUE];
		  RGBBuffer[RED][bufferindex] = _FontFGColour[RED];
		  RGBBuffer[GREEN][bufferindex] = _FontFGColour[GREEN];
		}else
		{
		  RGBBuffer[RED][bufferindex] = _FontBGColour[RED];
		  RGBBuffer[BLUE][bufferindex] = _FontBGColour[BLUE];
		  RGBBuffer[GREEN][bufferindex] = _FontBGColour[GREEN];
		}
	  }
	  charindex++;
	}	
  }
}
#endif
 
/* Write the contents of the output buffer to the LED's. 
   To ensure accurate timing interrupts are disabled whilst this 
   function is executed */
void HCWS2812::Refresh(void)
{
  int index;
  
  cli();
  for(index = 0; index < NUMBEROFLEDS; index++)
  {
    _SendByte(RGBBuffer[GREEN][index]); 
    _SendByte(RGBBuffer[RED][index]);
    _SendByte(RGBBuffer[BLUE][index]);     
  }
  sei();
  delayMicroseconds(100);
}

#ifdef DOTMATRIX
/* Sets the RGB colour if the font 
   R is an integer number of type byte between 0 and 255 
   which sets the intensity of the red element.
   G is an integer number of type byte between 0 and 255
   which sets the intensity of the green element.
   B is an integer number of type byte between 0 and 255
   which sets the intensity of the blue element. */
void HCWS2812::SetFontFG(byte R, byte G, byte B)
{
  _FontFGColour[RED] = R;
  _FontFGColour[GREEN] = G;
  _FontFGColour[BLUE] = B;
}
#endif

#ifdef DOTMATRIX
/* Sets the RGB colour if the fonts background.
   R is an integer number of type byte between 0 and 255 
   which sets the intensity of the red element.
   G is an integer number of type byte between 0 and 255
   which sets the intensity of the green element.
   B is an integer number of type byte between 0 and 255
   which sets the intensity of the blue element. */
void HCWS2812::SetFontBG(byte R, byte G, byte B)
{
  _FontBGColour[RED] = R;
  _FontBGColour[GREEN] = G;
  _FontBGColour[BLUE] = B;
}
#endif

/* Sets the RGB colour if the background.
   R is an integer number of type byte between 0 and 255 
   which sets the intensity of the red element.
   G is an integer number of type byte between 0 and 255
   which sets the intensity of the green element.
   B is an integer number of type byte between 0 and 255
   which sets the intensity of the blue element. */
void HCWS2812::SetBG(byte R, byte G, byte B)
{
  _BGColour[RED] = R;
  _BGColour[GREEN] = G;
  _BGColour[BLUE] = B;
}

/* Sends one byte of data to the LEDs */
void HCWS2812::_SendByte(byte Data)
{

  if(Data & BIT7)
  {
	DOUT_HIGH;
    HIGHDELAYHIGH;
    DOUT_LOW;
    HIGHDELAYLOW;
  }else
  {
    DOUT_HIGH;
    LOWDELAYHIGH;
    DOUT_LOW;
    LOWDELAYLOW;
  }
     
  if(Data & BIT6)
  {
    DOUT_HIGH;
    HIGHDELAYHIGH; 
    DOUT_LOW;
    HIGHDELAYLOW;
  }else
  {
    DOUT_HIGH;
    LOWDELAYHIGH;
    DOUT_LOW;
    LOWDELAYLOW;
  }
  
  if(Data & BIT5)
  {
    DOUT_HIGH;
    HIGHDELAYHIGH;
    DOUT_LOW;
    HIGHDELAYLOW;
  }else
  {
    DOUT_HIGH;
    LOWDELAYHIGH;
    DOUT_LOW;
    LOWDELAYLOW;
  }
     
  if(Data & BIT4)
  {
    DOUT_HIGH;
    HIGHDELAYHIGH; 
    DOUT_LOW;
    HIGHDELAYLOW;
  }else
  {
    DOUT_HIGH;
    LOWDELAYHIGH;
    DOUT_LOW;
    LOWDELAYLOW;
  }
 
  if(Data & BIT3)
  {
    DOUT_HIGH;
    HIGHDELAYHIGH;
    DOUT_LOW;
    HIGHDELAYLOW;
  }else
  {
    DOUT_HIGH;
    LOWDELAYHIGH;
    DOUT_LOW;
    LOWDELAYLOW;
  }
     
  if(Data & BIT2)
  {
    DOUT_HIGH;
    HIGHDELAYHIGH; 
    DOUT_LOW;
    HIGHDELAYLOW;
  }else
  {
    DOUT_HIGH;
    LOWDELAYHIGH;
    DOUT_LOW;
    LOWDELAYLOW;
  }
  
  if(Data & BIT1)
  {
    DOUT_HIGH;
    HIGHDELAYHIGH;
    DOUT_LOW;
    HIGHDELAYLOW;
  }else
  {
    DOUT_HIGH;
    LOWDELAYHIGH;
    DOUT_LOW;
    LOWDELAYLOW;
  }
     
  if(Data & BIT0)
  {
    DOUT_HIGH;
    HIGHDELAYHIGH; 
    DOUT_LOW;
    HIGHDELAYLOW;
  }else
  {
    DOUT_HIGH;
    LOWDELAYHIGH;
    DOUT_LOW;
    LOWDELAYLOW;
  }  
}

#ifdef DOTMATRIX
/* Used in the print functions */
void HCWS2812::_ReverseArray(char Array[], byte size)
{
  byte index1, index2;
  byte temp;

  for(index1=0,index2=size-1;index1<size/2;index1++,index2--)
  {
    temp=Array[index1];
	Array[index1]=Array[index2];
	Array[index2]=temp;
  }
} 
#endif