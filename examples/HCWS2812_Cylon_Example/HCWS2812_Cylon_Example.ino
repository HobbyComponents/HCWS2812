/* FILE:    HCWS2812_Cylon_Example
   DATE:    26/03/15
   VERSION: 0.1
   AUTHOR:  Andrew Davies

11/03/15 version 0.1: Original version

This is an example of how to use the HCMAX7219 library to control one or more
RGB LEDS. The example will set each LED to a random colour.

To use this example connect one or more LEDs in series (Dout --> Din) and connect
the first LED's Din pin to digital pin 8 of your Arduino.

By default the library is set to control 100 LEDs. You can change this by editing the
following line in the MCMAX7219.h header file:

#define NUMBEROFLEDS 200 <--- Change this number to match the number of LEDS connected

You can download the library from the software section of our support forum here:
http://forum.hobbycomponents.com/viewforum.php?f=58

Or from Github here:
https://github.com/HobbyComponents/HCWS2812


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

/* Include the HCWS2812 library */
#include "HCWS2812.h"

/* Create an instance of the library */
HCWS2812 HCWS2812;

void setup() 
{
  /* Set the R,G,B background colours to zero */ 
  HCWS2812.SetBG(0, 0, 0);
  /* Clear the output buffer */
  HCWS2812.ClearBuffer();
}


/* Main program */
void loop() 
{
  int index;
  
  /* Step forward through each LED */
  for(index = 0; index < NUMBEROFLEDS; index++)
  {  
    HCWS2812.ClearBuffer();
    RGBBuffer[RED][index] = 255;
    HCWS2812.Refresh();
    delay(100);
  }
  
  /* Step backward through each LED */
  for(index = NUMBEROFLEDS; index; index--)
  {  
    HCWS2812.ClearBuffer();
    RGBBuffer[RED][index - 1] = 255;
    HCWS2812.Refresh();
    delay(100);
  }
}
