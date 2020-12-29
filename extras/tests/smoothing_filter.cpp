/*
  NAME:
  Unit tests of library "gbj_filter_smoothing" for smoothing.

  DESCRIPTION:
  The test suite provides test cases for calculating filtered value for two
  consecutive values.
  - The test runner is Unity Project - ThrowTheSwitch.org.

  LICENSE:
  This program is free software; you can redistribute it and/or modify
  it under the terms of the MIT License (MIT).

  CREDENTIALS:
  Author: Libor Gabaj
*/
#include <Arduino.h>
#include <unity.h>
#include <gbj_filter_smoothing.h>

// Valid range of values ensured by filtering
const unsigned int SENSOR_FILTER_MIN = 128;
const unsigned int SENSOR_FILTER_MAX = 768;
const unsigned int SMOOTH_SAMPLES = 5;

gbj_filter_smoothing Filter = gbj_filter_smoothing(
    SENSOR_FILTER_MAX, SENSOR_FILTER_MIN, SMOOTH_SAMPLES);

void setup_test()
{
    // Read testing sample
    byte i = 0;
    uint16_t sample[] = {510, 835, 77, 152, 85, 652, 994, 179, 839, 279};
    while (Filter.registerData(sample[i++]));
    // Result:
    // Median = 279
    // Average = 354
    // Middle Average = 323
    // Minimum = 152
    // Maximum = 652
}

void test_smoothing_median(void)
{
    TEST_ASSERT_EQUAL_INT16(279, Filter.getMedian());
}

void test_smoothing_average(void)
{
    TEST_ASSERT_EQUAL_INT16(354, Filter.getAverage());
}

void test_smoothing_midaverage(void)
{
    TEST_ASSERT_EQUAL_INT16(323, Filter.getMidAverage());
}

void test_smoothing_minimum(void)
{
    TEST_ASSERT_EQUAL_INT16(152, Filter.getMinimum());
}

void test_smoothing_maximum(void)
{
    TEST_ASSERT_EQUAL_INT16(652, Filter.getMaximum());
}


void setup() {
    setup_test();
    UNITY_BEGIN();

    RUN_TEST(test_smoothing_median);
    RUN_TEST(test_smoothing_average);
    RUN_TEST(test_smoothing_midaverage);
    RUN_TEST(test_smoothing_minimum);
    RUN_TEST(test_smoothing_maximum);

    UNITY_END();
}

void loop(){}