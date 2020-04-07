/*------------------------------------------------------------------------------
  24/02/2020
  Author: Esterlin Polanco
  Platforms: Arduino Nano
  Language: C#/Arduino
  File: fisica_2.ino
  ------------------------------------------------------------------------------
  Description: 
  Code for Water flow metter system, built for physics 2 class at Dominico Americano
  University, Santo Domingo D.R. Connects to ESP8266 via i2c. 
  ------------------------------------------------------------------------------
  License:
  Please see attached LICENSE.txt file for details.
------------------------------------------------------------------------------*/
#include <Wire.h>

//Flow Sensor variables:
int sendvalue;
const int sensorPin = 2;
const int measureInterval = 1000;
volatile int pulseConter;
const float factorK = 7.5;
float frequency;
float flow_Lmin;

//i2c Address:
const byte SlaveDeviceId = 1;

/*------------------------------------------------------------------------------
  Flow sensor methods:
------------------------------------------------------------------------------*/ 
  void ISRCountPulse()
  {
    pulseConter++;
  }
  
  float GetFrequency()
  {
    pulseConter = 0;
  
    interrupts();
    delay(measureInterval);
    noInterrupts();
  
    return (float)pulseConter * 1000 / measureInterval;
  }

  int measure()
  {
    frequency = GetFrequency(); 
    flow_Lmin = frequency / factorK;
    
    //Multiply to make the value suitable for integer to be sent:
    sendvalue = flow_Lmin * 100;
    return sendvalue;

  }

/*------------------------------------------------------------------------------
  I2C methods:
------------------------------------------------------------------------------*/ 
  void requestCallback()
  {
    int input = sendvalue;
    
    // Send two bytes to master.
    uint8_t buffer[2];
    buffer[0] = input >> 8;
    buffer[1] = input & 0xff;  
    Wire.write(buffer, 2);
  }

void setup()
{
  // Start I2C bus as a slave
  Wire.begin(SlaveDeviceId); 
  // Set the callback to call when data is requested. 
  Wire.onRequest(requestCallback);

  //interrupts initialization:
  attachInterrupt(digitalPinToInterrupt(sensorPin), ISRCountPulse, RISING);
}

void loop()
{
  //Calculate the water flow and stores the value to send over i2c:
  sendvalue = measure();
  
}

