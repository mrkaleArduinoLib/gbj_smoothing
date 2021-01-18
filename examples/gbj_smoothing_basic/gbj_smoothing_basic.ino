/*
  NAME:
  Usage of gbj_smoothing library with one statistics.

  DESCRIPTION:
  This sketch demonstrates the use of smoothing without determining
  optional arguments, so that it uses the default values of them.
  - As a measured value the sketch utilizes the random integers within
    the range 0 to 1023.
  - The sketch calculates just the average from all statistical values
    provided by the library.

  LICENSE:
  This program is free software; you can redistribute it and/or modify
  it under the terms of the MIT License (MIT).

  CREDENTIALS:
  Author: Libor Gabaj
*/
#include "gbj_smoothing.h"
#define SKETCH "GBJ_SMOOTHING_BASIC 1.2.0"

const unsigned int PERIOD_MEASURE =
  3000; // Time in milliseconds between measurements

// Upper limit of random values for mimicking real physical measurement
const unsigned int SENSOR_DATA_MAX = 1023;

// Variables and constants for measurement
unsigned int filterData;
gbj_smoothing filter = gbj_smoothing();

void setup()
{
  Serial.begin(9600);
  Serial.println(SKETCH);
  Serial.println("Libraries:");
  Serial.println(gbj_smoothing::VERSION);
  Serial.println(gbj_apphelpers::VERSION);
  Serial.println("---");
  Serial.println("Statics:");
  Serial.println("getFilterMin:" + String(gbj_smoothing::getFilterMin()));
  Serial.println("getFilterMax:" + String(gbj_smoothing::getFilterMax()));
  Serial.println("getBufferLenMin:" +
                 String(gbj_smoothing::getBufferLenMin()));
  Serial.println("getBufferLenMax:" +
                 String(gbj_smoothing::getBufferLenMax()));
  Serial.println("getBufferLenDft:" +
                 String(gbj_smoothing::getBufferLenDft()));
  Serial.println("getDelayMin:" + String(gbj_smoothing::getDelayMin()));
  Serial.println("getDelayMax:" + String(gbj_smoothing::getDelayMax()));
  Serial.println("getDelayDft:" + String(gbj_smoothing::getDelayDft()));
  Serial.println("---");
  Serial.print("Readings: ");
  Serial.println(filter.getBufferLen());
  Serial.println("Smoothed average:");
}

void loop()
{
  while (filter.registerData(random(SENSOR_DATA_MAX + 1)))
    ; // Readings to buffer
  filterData = filter.getAverage();
  Serial.println(filterData, 1);
  delay(PERIOD_MEASURE);
}
