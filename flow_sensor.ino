/*------------------------------------------------------------------------------
  24/02/2020
  Author: Esterlin Polanco
  Platforms: Arduino Nano
  Language: C#/Arduino
  File: fisica_2.ino
  ------------------------------------------------------------------------------
  Description: 
  Code for Water flow metter system, built for physics 2 class at Dominico Americano
  University, Santo Domingo D.R. Connects to ESP8266
  ------------------------------------------------------------------------------
  License:
  Please see attached LICENSE.txt file for details.
------------------------------------------------------------------------------*/
#include <Wire.h>

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
 } 

void requestEvent() 
 {
    measure();
    char result[8];
    dtostrf(flow_Lmin, 6, 2, result);
    Wire.write(result); // respond with message of 6 bytes
  
 }

void setup()
{
   Serial.begin(9600);
   attachInterrupt(digitalPinToInterrupt(sensorPin), ISRCountPulse, RISING);
   Wire.begin(8); // join i2c bus with address #8
   Wire.onRequest(requestEvent);
}
 
void loop()
 {
   measure();
    char result[8];
    dtostrf(flow_Lmin, 6, 2, result);
    Wire.beginTransmission(7);
    Wire.write(result); // respond with message of 6 bytes
    Wire.endTransmission();
   delay(100);
 }