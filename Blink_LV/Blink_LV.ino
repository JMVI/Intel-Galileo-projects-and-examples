/* -----------------------------------------------------------------------------
 * Copyright (C) 2020 Jaime M. Villegas I. <jaime7592@gmail.com>
 * -----------------------------------------------------------------------------
 * Filename      : Blink_LV.ino
 * Description   : Blink sketch emulation using Intel Galileo and LabVIEW. The
                   Intel Galileo board toggles a virtual LED setted up in LV.
                   Version with simulation control.
 * Version       : 02.00
 * Revision      : 00
 * Last modified : 01/03/2021
 * -----------------------------------------------------------------------------
 */

//----------------------------------------------------------------------------//
//                                  Macros                                    //
//----------------------------------------------------------------------------//

/********************* Serial communication management ************************/

// ASCII bytes
#define STX 0x02
#define ETX 0x03
#define ACK 0x06

// Command header
#define SIM_CONTROL 'X'   // Simulation control (Start / Stop)

// Serial port baud rate
#define BAUD_RATE 9600


/**************************** Simulation control ******************************/

// Start simulation dynamic cycle
#define START_SEQUENCE "START"

// Reset simulation and parameters 
#define RESET_SEQUENCE "RESET"

// Stop simulation dynamic cycle
#define STOP_SEQUENCE "STOP"

//----------------------------------------------------------------------------//
//                            General definitions                             //
//----------------------------------------------------------------------------//

// LED status
int LED_stat = 0x00;

// On-board LED pin
int LED_pin = 13;


/********************* Serial communication management ************************/

// UART buffer status
typedef enum
{
  BUFFER_FREE = 0,    // Buffer is free
  BUFFER_FILLING,     // Filling buffer with new data
  BUFFER_READY,       // Ready to process received data
}
BUFFER_STAT;

// Status flag
int Buffer_status = BUFFER_FREE;

// Rx buffer
String Buffer_In = "";   // Input buffer
int byte_count = 0;      // Buffer size

// New data to be processed
boolean new_data = false;


/**************************** Simulation control ******************************/

// Simulation execution flags
boolean running_simulation = false;
boolean end_simulation = false;

//----------------------------------------------------------------------------//
//                              Local functions                               //
//----------------------------------------------------------------------------//

/********************* Serial communication management ************************/

/**
@brief Serial event. Processes received frame
@param none
@retval none
*/

void serialEvent()
{
  char Rx_byte;  // Received byte
  
  // Verifies if new data was received
  while (Serial.available())
  {
    // Gets a new byte
    Rx_byte = (char)Serial.read();
    
    switch(Buffer_status)
    {
      case BUFFER_FREE:
      
        // Initiate new bytes reception
        if(Rx_byte == STX)
        {
          Buffer_status = BUFFER_FILLING;
        }
        
        break;
        
      case BUFFER_FILLING:
        
        // Check for the end of frame
        if(Rx_byte == ETX)
        {
          Buffer_status = BUFFER_READY;
        }
        else
        {
          // If the end hasn't been reached, add byte to buffer
          Buffer_In += Rx_byte;
          byte_count++;
        }
        
        break;
    }
    
    // Resets states machine if buffer is ready to process
    if(Buffer_status == BUFFER_READY)
    {
      // Frees buffer
      Buffer_status = BUFFER_FREE;
      
      // Sends acknoledge
      Serial.write(ACK);
      
      // There is new data to process
      new_data = true;
    }
  }
}

/**
@brief Parsing of received data
@param none
@retval none
*/
void buffer_parsing()
{
  char cmd_type = Buffer_In[0];  // Type of command
  
  // Deletes type of command
  Buffer_In = Buffer_In.substring(1);
  
  if(cmd_type == SIM_CONTROL)
  {
    // Simulation control ('X')
    if(Buffer_In == START_SEQUENCE)
    {
      // Runs simulation
      run_simulation();
    }
    else if(Buffer_In == RESET_SEQUENCE)
    {
      // Resets simulation
      reset_simulation();
    }
    else if(Buffer_In == STOP_SEQUENCE)
    {
      // Stops simulation
      stop_simulation();
    }
  }
  
  // Disables new data flag
  new_data = false;
        
  // Clears input bufffer
  Buffer_In = "";
  byte_count = 0;
}

/**************************** Simulation control ******************************/

/**
@brief Runs simulation and performs additional tasks if needed
@param none
@retval none
*/
void run_simulation()
{
  // Enables simulation execution flag
  running_simulation = true;
}

/**
@brief Resets simulation to default parameters when Reset control 
       line is received
@param none
@retval none
*/
void reset_simulation()
{
  // Disables simulation execution flag
  running_simulation = false;
  
  /*************** Simulation parameters ***************/
  
  // Resets virtual LED status
  LED_stat = 0x00;
  
  // Turn on-board LED off
  digitalWrite(LED_pin, LOW);
  
  /*****************************************************/
}

/**
@brief Runs simulation and performs additional tasks if needed
@param none
@retval none
*/
void stop_simulation()
{
  // Disables simulation execution flag
  running_simulation = false;
  
  // End of execution
  end_simulation = true;
  
  /*************** Simulation parameters ***************/
  
  // Resets virtual LED status
  LED_stat = 0x00;
  
  // Turn on-board LED off
  digitalWrite(LED_pin, LOW);
  
  /*****************************************************/
}
//----------------------------------------------------------------------------//
//                                Sketch setup                                //
//----------------------------------------------------------------------------//

/**
@brief  Serial port and LED pin configuration. Reservation for Rx buffer.
@param none
@retval none
*/
void setup()
{
  // Serial port configuration (9600 bps, 8 data bits, 1 stop bit, no parity)
  Serial.begin(BAUD_RATE);
  
  // Waits for serial port to connect
  while (!Serial) {}
  
  // Initializes the digital pin as an output.
  pinMode(LED_pin, OUTPUT);
  
  // Reserve 10 bytes for Rx buffer
  Buffer_In.reserve(10);
}

//----------------------------------------------------------------------------//
//                                 Main loop                                  //
//----------------------------------------------------------------------------//

/**
@brief  Dynamic loop of simulation
*/
void loop()
{
  serialEvent();
  
  // Parsing of data if available
  if(new_data == true)
  {
    buffer_parsing();
  }
  
  if(running_simulation == true)
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
  
  // End simulation?
  if(end_simulation == true)
  {
    /*** End of execution ***/
    while(true){};
  }
}

