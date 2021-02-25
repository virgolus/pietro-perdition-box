#include "LedControl.h" //  need the library
LedControl lc=LedControl(6,8,7,4); // 

// pin 12 is connected to the MAX7219 pin 1
// pin 11 is connected to the CLK pin 13
// pin 10 is connected to LOAD pin 12
// 1 as we are only using 1 MAX7219

void setup()
{
  lc.shutdown(0,false);// turn off power saving, enables display
  lc.setIntensity(0,8);// sets brightness (0~15 possible values)
  lc.clearDisplay(0);// clear screen

  lc.shutdown(1,false);// turn off power saving, enables display
  lc.setIntensity(1,8);// sets brightness (0~15 possible values)
  lc.clearDisplay(1);// clear screen
  
  lc.shutdown(2,false);// turn off power saving, enables display
  lc.setIntensity(2,8);// sets brightness (0~15 possible values)
  lc.clearDisplay(1);// clear screen

  lc.shutdown(3,false);// turn off power saving, enables display
  lc.setIntensity(3,8);// sets brightness (0~15 possible values)
  lc.clearDisplay(1);// clear screen
}

void loop()
{
  for (int row=0; row<8; row++)
  {
    for (int col=0; col<8; col++)
    {
      lc.setLed(0,col,row,true); // turns on LED at col, row
      lc.setLed(1,col,row,false); // turns on LED at col, row
      lc.setLed(2,col,row,false); // turns on LED at col, row
      lc.setLed(3,col,row,false); // turns on LED at col, row
      delay(25);
    }
  }

    for (int row=0; row<8; row++)
  {
    for (int col=0; col<8; col++)
    {
      lc.setLed(0,col,row,false); // turns on LED at col, row
      lc.setLed(1,col,row,true); // turns on LED at col, row
      lc.setLed(2,col,row,false); // turns on LED at col, row
      lc.setLed(3,col,row,false); // turns on LED at col, row
      delay(25);
    }
  }

    for (int row=0; row<8; row++)
  {
    for (int col=0; col<8; col++)
    {
      lc.setLed(0,col,row,false); // turns on LED at col, row
      lc.setLed(1,col,row,false); // turns on LED at col, row
      lc.setLed(2,col,row,true); // turns on LED at col, row
      lc.setLed(3,col,row,false); // turns on LED at col, row
      delay(25);
    }
  }

    for (int row=0; row<8; row++)
  {
    for (int col=0; col<8; col++)
    {
      lc.setLed(0,col,row,false); // turns on LED at col, row
      lc.setLed(1,col,row,false); // turns on LED at col, row
      lc.setLed(2,col,row,false); // turns on LED at col, row
      lc.setLed(3,col,row,true); // turns on LED at col, row
      delay(25);
    }
  }
}
