/* -----------------------------------------------------------------------------
 * Copyright (C) 2020 Jaime M. Villegas I. <jaime7592@gmail.com>
 * -----------------------------------------------------------------------------
 * Filename      : Blink_LV.ino
 * Description   : Blink sketch emulation using Intel Galileo and LabVIEW. The
                   Intel Galileo board toggles a virtual LED setted up in LV.
 * Version       : 01.00
 * Revision      : 00
 * Last modified : 12/27/2020
 * -----------------------------------------------------------------------------
 */

//----------------------------------------------------------------------------//
//                            General definitions                             //
//----------------------------------------------------------------------------//

// Serial port baud rate
#define BAUD_RATE 9600

// LED status
int LED_pin = 0x00;

//----------------------------------------------------------------------------//
//                              Local functions                               //
//----------------------------------------------------------------------------//

/**
@brief  Function description
@param  none
@retval none
*/

// TODO: Add event to start / stop emulation

//----------------------------------------------------------------------------//
//                                Sketch setup                                //
//----------------------------------------------------------------------------//

/**
@brief  Serial port configuration
@note   Additional information
*/
void setup()
{
  // Sets serial port baud rate
  Serial.begin(BAUD_RATE);
  
  // Waits for serial port to connect
  while (!Serial) {}
}

//----------------------------------------------------------------------------//
//                                 Main loop                                  //
//----------------------------------------------------------------------------//

/**
@brief  Main program description
@note   Additional information
*/
void loop()
{
  // Toggles virtual LED status
  if(LED_pin == 0x00)
  {
    LED_pin = 0x01;
  }
  else
  {
    LED_pin = 0x00;
  }
  
  delay(1000);
  
  // Write status through serial port
  Serial.print(LED_pin, HEX);
}

