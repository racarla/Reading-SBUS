/*
* Brian R Taylor
* brian.taylor@bolderflight.com
* 
* Copyright (c) 2018 Bolder Flight Systems
* 
* Permission is hereby granted, free of charge, to any person obtaining a copy of this software 
* and associated documentation files (the "Software"), to deal in the Software without restriction, 
* including without limitation the rights to use, copy, modify, merge, publish, distribute, 
* sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is 
* furnished to do so, subject to the following conditions:
* 
* The above copyright notice and this permission notice shall be included in all copies or 
* substantial portions of the Software.
* 
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING 
* BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND 
* NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, 
* DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, 
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

/*
* Description: this program will output a sine wave on all 16 SBUS channels for 
* testing the Teensy SBUS Backpack (http://bolderflight.com/products/teensy/sbus). 
* The sine amplitude will be set to make the servo travel across its entire range
* and the excitation frequency will be 1 Hz.
*/

#include "SBUS.h"

/* The Teensy SBUS Backpack interfaces on Serial2 */
SBUS sbus(Serial2);
/* 16 SBUS channels, reading */
float SbusRead[16];
/* Failsafe and lost frame status */
bool Failsafe, LostFrame;

void setup()
{
  /* Serial for printing the command */
  Serial.begin(115200);
  while (!Serial) {}
  /* Starting SBUS communication */
  sbus.begin();
}

void loop()
{
  if (sbus.readCal(&SbusRead[0], &Failsafe, &LostFrame)) {
    for (unsigned int i = 0; i < sizeof(SbusRead) / sizeof(float); i++) {
      Serial.print(SbusRead[i]);
      Serial.print("\t");
    }
    Serial.print(LostFrame);
    Serial.print("\t");
    Serial.println(Failsafe);
  }
}