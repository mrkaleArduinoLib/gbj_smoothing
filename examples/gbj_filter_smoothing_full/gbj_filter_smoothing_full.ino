/*
  NAME:
  Usage of gbj_filter_smoothing library with all statistics.

  DESCRIPTION:
  This sketch demonstrates the use of smoothing filtering with determining all
  arguments that it provides with their default values.
  - As a measured value the sketch utilizes the random integers within
    the range 0 to 1023, but registers just the values withing range 128 ~ 768
    for demostrating filtering.
  - Random values within the valid range are marked with the asterisk.
  - The sketch calculates all statistics that the library provides.

  LICENSE:
  This program is free software; you can redistribute it and/or modify
  it under the terms of the MIT License (MIT).

  CREDENTIALS:
  Author: Libor Gabaj
*/
#include "gbj_filter_smoothing.h"
#define SKETCH "GBJ_FILTER_SMOOTHING_FULL 1.1.0"

const unsigned int PERIOD_MEASURE = 3000; // Time in miliseconds between measurements

// Limits of random values for mimicking real physical measurement
const unsigned int SENSOR_DATA_MIN = 0;
const unsigned int SENSOR_DATA_MAX = 1023;

// Valid range of values ensured by filtering
const unsigned int SENSOR_FILTER_MIN = 128;
const unsigned int SENSOR_FILTER_MAX = 768;

// Variables and constants for measurement
const unsigned int SMOOTH_SAMPLES = gbj_filter_smoothing::getBufferLenDef();
const byte SMOOTH_PERIOD = gbj_filter_smoothing::getDelayDef();

gbj_filter_smoothing Smoother = gbj_filter_smoothing(
    SENSOR_FILTER_MAX, SENSOR_FILTER_MIN, SMOOTH_SAMPLES, SMOOTH_PERIOD);
unsigned int demoData;

void setup()
{
  Serial.begin(9600);
  Serial.println(SKETCH);
  Serial.println("Libraries:");
  Serial.println(gbj_filter_smoothing::VERSION);
  Serial.println(gbj_apphelpers::VERSION);
  Serial.println("---");
  Serial.print("Readings: ");
  Serial.println(Smoother.getBufferLen());
  Serial.print("Valid range: ");
  Serial.print(Smoother.getValueMin());
  Serial.print(" ~ ");
  Serial.println(Smoother.getValueMax());
  Serial.println("Values => Median Average MidAvg Min Max:");
}

void loop()
{
  do
  {
    demoData = random(SENSOR_DATA_MIN, SENSOR_DATA_MAX + 1);
    if (demoData >= Smoother.getValueMin() && demoData <= Smoother.getValueMax())
    {
      Serial.print("*");
    }
    Serial.print(demoData);
    Serial.print(" ");
  } while (Smoother.registerData(demoData));
  Serial.print("=> ");
  Serial.print(Smoother.getMedian());
  Serial.print(" ");
  Serial.print(Smoother.getAverage());
  Serial.print(" ");
  Serial.print(Smoother.getMidAverage());
  Serial.print(" ");
  Serial.print(Smoother.getMinimum());
  Serial.print(" ");
  Serial.print(Smoother.getMaximum());
  Serial.println();
  delay(PERIOD_MEASURE);
}
