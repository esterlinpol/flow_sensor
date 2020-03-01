/*------------------------------------------------------------------------------
  24/02/2020
  Author: Esterlin Polanco
  Platforms: Arduino Nano
  Language: C#/Arduino
  File: fisica_2.ino
  ------------------------------------------------------------------------------
  Description: 
  Code for Water flow metter system, built for physics 2 class at Dominico Americano
  University, Santo Domingo D.R. Connects to ESP8266 via serial. 
  ------------------------------------------------------------------------------
  License:
  Please see attached LICENSE.txt file for details.
------------------------------------------------------------------------------*/
int sendvalue;
const int sensorPin = 2;
const int measureInterval = 1000;
volatile int pulseConter;
const float factorK = 7.5;
float frequency;
float flow_Lmin;
 
/*------------------------------------------------------------------------------
  Flow Sensor methods:
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

void measure()
 {
   frequency = GetFrequency(); 
   flow_Lmin = frequency / factorK;
   //Multiply to make the value suitable for integer to be sent:
   sendvalue = flow_Lmin * 1000;
 } 
void setup() {
  // Begin the Serial at 9600 Baud
  Serial.begin(115200);
  attachInterrupt(digitalPinToInterrupt(sensorPin), ISRCountPulse, RISING);
  
}

void loop() {
  measure();
  Serial.print(sendvalue); //Write the serial data
  delay(400);
}