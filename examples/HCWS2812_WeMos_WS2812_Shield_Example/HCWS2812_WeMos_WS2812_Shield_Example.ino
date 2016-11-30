/* FILE:    HCWS2812_WeMos_WS2812_Shield_Example
   DATE:    30/11/16
   VERSION: 0.1
   AUTHOR:  Andrew Davies

11/03/15 version 0.1: Original version

This example shows how to use the HCWS2812 to control the WS2812 shield for the 
WeMos D1 mini and mini Pro boards (HCWEMO0002). he example will cycle the RGB LED 
on the shield through the different colours.

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

/* Include the ESP266WiFi.h library */
#include <ESP8266WiFi.h>

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

  /* The library requires very precise timing. Therefore WiFi needs to be disabled whilst  
     updating the LEDs to that is doesn't get interrupted. */
  WiFi.mode(WIFI_OFF);

  /* After you have updated the LED WiFi can be turn back on using one of the following modes:
   * WiFi.mode(WIFI_AP);
   * WiFi.mode(WIFI_STA);
   * WiFi.mode(WIFI_AP_STA);
   */
}

void loop() 
{

  /* Fade colour from red to green */
  for(int i = 0; i <= 255; i++)
  {
     RGBBuffer[RED][0] = 255 - i;
     RGBBuffer[GREEN][0] = i;
     HCWS2812.Refresh();
     delay(10);
  }

  /* Fade colour from green to blue */
  for(int i = 0; i <= 255; i++)
  {
     RGBBuffer[GREEN][0] = 255 - i;
     RGBBuffer[BLUE][0] = i;
     HCWS2812.Refresh();
     delay(10);
  }

  /* Fade colour from blue to red */
  for(int i = 0; i <= 255; i++)
  {
     RGBBuffer[BLUE][0] = 255 - i;
     RGBBuffer[RED][0] = i;
     HCWS2812.Refresh();
     delay(10);
  }

}
