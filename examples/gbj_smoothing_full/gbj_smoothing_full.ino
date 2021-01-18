/*
  NAME:
  Usage of gbj_smoothing library with all statistics.

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
#include "gbj_smoothing.h"
#define SKETCH "GBJ_SMOOTHING_FULL 1.2.0"

const unsigned int PERIOD_MEASURE =
  3000; // Time in miliseconds between measurements

// Limits of random values for mimicking real physical measurement
const unsigned int SENSOR_DATA_MIN = 0;
const unsigned int SENSOR_DATA_MAX = 1023;

// Valid range of values ensured by filtering
const unsigned int SENSOR_FILTER_MIN = 128;
const unsigned int SENSOR_FILTER_MAX = 768;

// Variables and constants for measurement
const unsigned int SMOOTH_SAMPLES = gbj_smoothing::getBufferLenDft();
const byte SMOOTH_PERIOD = gbj_smoothing::getDelayDft();

gbj_smoothing filter = gbj_smoothing(SENSOR_FILTER_MAX,
                                       SENSOR_FILTER_MIN,
                                       SMOOTH_SAMPLES,
                                       SMOOTH_PERIOD);
unsigned int demoData;

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
  Serial.println("getBufferLenMin:" + String(gbj_smoothing::getBufferLenMin()));
  Serial.println("getBufferLenMax:" + String(gbj_smoothing::getBufferLenMax()));
  Serial.println("getBufferLenDft:" + String(gbj_smoothing::getBufferLenDft()));
  Serial.println("getDelayMin:" + String(gbj_smoothing::getDelayMin()));
  Serial.println("getDelayMax:" + String(gbj_smoothing::getDelayMax()));
  Serial.println("getDelayDft:" + String(gbj_smoothing::getDelayDft()));
  Serial.println("---");
  Serial.print("Readings: ");
  Serial.println(filter.getBufferLen());
  Serial.print("Valid range: ");
  Serial.print(filter.getValueMin());
  Serial.print(" ~ ");
  Serial.println(filter.getValueMax());
  Serial.println("Values => Median Average MidAvg Min Max:");
}

void loop()
{
  do
  {
    demoData = random(SENSOR_DATA_MIN, SENSOR_DATA_MAX + 1);
    if (demoData >= filter.getValueMin() &&
        demoData <= filter.getValueMax())
    {
      Serial.print("*");
    }
    Serial.print(demoData);
    Serial.print(" ");
  } while (filter.registerData(demoData));
  Serial.print("=> ");
  Serial.print(filter.getMedian());
  Serial.print(" ");
  Serial.print(filter.getAverage());
  Serial.print(" ");
  Serial.print(filter.getMidAverage());
  Serial.print(" ");
  Serial.print(filter.getMinimum());
  Serial.print(" ");
  Serial.print(filter.getMaximum());
  Serial.println();
  delay(PERIOD_MEASURE);
}
