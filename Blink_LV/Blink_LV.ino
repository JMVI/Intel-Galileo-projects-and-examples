/* -----------------------------------------------------------------------------
 * Copyright (C) 2020 Jaime M. Villegas I. <jaime7592@gmail.com>
 * -----------------------------------------------------------------------------
 * Filename      : Blink_LV.ino
 * Description   : Blink sketch emulation using Intel Galileo and LabVIEW. The
                   Intel Galileo board toggles a virtual LED setted up in LV.
 * Version       : 01.00
 * Revision      : 00
 * Last modified : 12/30/2020
 * -----------------------------------------------------------------------------
 */

//----------------------------------------------------------------------------//
//                            General definitions                             //
//----------------------------------------------------------------------------//

// Serial port baud rate
#define BAUD_RATE 9600

// LED status
int LED_stat = 0x00;

// On-board LED pin
int LED_pin = 13;

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
@brief  Serial port and LED pin configuration
*/
void setup()
{
  // Serial port configuration (9600 bps, 8 data bits, 1 stop bit, no parity)
  Serial.begin(BAUD_RATE);
  
  // Waits for serial port to connect
  while (!Serial) {}
  
  // Initializes the digital pin as an output.
  pinMode(LED_pin, OUTPUT); 
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
  if(LED_stat == 0x00)
  {
    // Turn the LED on (HIGH is the voltage level)
    LED_stat = 0x01;
    digitalWrite(LED_pin, HIGH);
  }
  else
  {
    // Turn the LED off by making the voltage LOW
    LED_stat = 0x00;
    digitalWrite(LED_pin, LOW);
  }
  
  // Waits 100 ms
  delay(100);
  
  // Write status through serial port
  Serial.print(LED_stat, HEX);
}

