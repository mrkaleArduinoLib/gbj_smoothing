/*
  NAME:
  Usage of gbj_filter_smoothing library with one statistics.

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
#include "gbj_filter_smoothing.h"
#define SKETCH "GBJ_FILTER_SMOOTHING_BASIC 1.0.0"

const unsigned int PERIOD_MEASURE = 3000;      // Time in milliseconds between measurements

// Upper limit of random values for mimicking real physical measurement
const unsigned int SENSOR_DATA_MAX = 1023;

// Variables and constants for measurement
unsigned int filterData;
gbj_filter_smoothing Smoother = gbj_filter_smoothing();


void setup()
{
  Serial.begin(9600);
  Serial.println(SKETCH);
  Serial.println("Libraries:");
  Serial.println(GBJ_FILTER_SMOOTHING_VERSION);
  Serial.println("---");
  Serial.print("Readings: ");
  Serial.println(Smoother.getBufferLen());
  Serial.println("Smoothed average:");
}


void loop()
{
  while (Smoother.registerData(random(SENSOR_DATA_MAX + 1)));  // Readings to buffer
  filterData = Smoother.getAverage();
  Serial.println(filterData, 1);
  delay(PERIOD_MEASURE);
}
