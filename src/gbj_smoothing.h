/*
  NAME:
  gbj_smoothing

  DESCRIPTION:
  The library provides storing subsequent data values (measurement burst)
  in 16-bit resolution in a batch of defined number of them and then
  calculates some statistical values from the data batch.
  - A calculated statistical value represents a data batch.
  - A series of those statistical values represents smoothed measurement
    from sensors with reduced digital noise, i.e., smoothing filtering.

  LICENSE:
  This program is free software; you can redistribute it and/or modify
  it under the terms of the license GNU GPL v3
  http://www.gnu.org/licenses/gpl-3.0.html (related to original code) and MIT
  License (MIT) for added code.

  CREDENTIALS:
  Author: Libor Gabaj
  GitHub: https://github.com/mrkaleArduinoLib/gbj_smoothing.git
*/
#ifndef GBJ_SMOOTHING_H
#define GBJ_SMOOTHING_H

#if defined(__AVR__)
  #include "Arduino.h"
  #include <inttypes.h>
#elif defined(ESP8266) || defined(ESP32)
  #include <Arduino.h>
#elif defined(PARTICLE)
  #include "Particle.h"
  #include "math.h"
#endif
#include "gbj_apphelpers.h"

// Macro functions
#define divide(a, b)                                                           \
  {                                                                            \
    (uint16_t) round((float)(a) / (float)(b))                                  \
  }

class gbj_smoothing
{
public:
  static const String VERSION;

  /*
    Constructor.

    DESCRIPTION:
    Constructor creates the data buffer within a class instance object, which
    holds a serie of data values that should be substituted by a statistical
    value.
    - Class enables delay between registering data values in order to settle
      a sensor, especially its ADC between readings.
    - For each measuring parameter (usually sensor) a separate instance object
      has to be created with separated data buffer length and measurement
      delay, if default values are not suitable.
    - Smoothing sensor readings eliminates the measurement noise in the
      analog data.
    - The class may be used for original maximal 16-bit non-negative digital
      data as well, where the statistical processing is desirable.
    - The constructor has all arguments defaulted.

    PARAMETERS:
    valueMax - Maximal valid sensor value for registering.
              - Data type: non-negative integer
              - Default value: 65535
              - Limited range: 0 ~ 65535

    valueMin - Minimal valid sensor value for registering.
              - Data type: non-negative integer
              - Default value: 0
              - Limited range: 0 ~ 65535

    bufferLen - Number of 16-bit values, which a statistical value is
                going to be calculated from.
                - The buffer length should be odd number. If it is not, the
                  constructor adds 1 to it right before limiting the length
                  in order to make it odd.
                - Data type: positive odd integer
                - Default value: 5
                - Limited range: 3 ~ 11

    sensorDelay - Number of milliseconds, which the system is going to be
                  suspended after registering each data value to the data
    buffer.
                  - The method does not utilize system delay() function but just
                    timestamp in milliseconds calculated by millis() function.
                  - The delay enables to settle or consolidate a sensor after
                    reading, especially its shared (multiplexed) analog-digital
                    converter.
                  - Data type: non-negative integer
                  - Default value: 20
                  - Limited range: 0 ~ 100

    RETURN:
    Library instance object.
  */
  gbj_smoothing(uint16_t valueMax = Limits::FILTER_MAX,
                uint16_t valueMin = Limits::FILTER_MIN,
                uint8_t bufferLen = Limits::BUFFERLEN_DFT,
                uint8_t sensorDelay = Limits::DELAY_DFT);

  /*
    Reset all counters and status flags.

    DESCRIPTION:
    The method initiates all internal counters and status flags of a class
    instance object to default values as they are right after power up of
    a microcontroler.

    PARAMETERS: none

    RETURN: none
  */
  inline void init() { bufferCnt_ = 0; };

  /*
    Put measured value to the buffer.

    DESCRIPTION:
    The method stores a measured sensor value to the data buffer for further
    statistical processing, which is in the valid range of values.
    - The parameter of smoothing like filtering range and measurement delay
    should be defined before registering the data by the constructor or setters.
    - The method ignores a value, which is outside of the valid range defined
      by the corresponding input arguments in the constructor, or defined by
      the filter settersm and does not count that value.
    - The method returns false status until the object buffer is full, i.e.,
      entire set of values determined by the buffer length for the current run
      is registered and statistical processing can be provided. It enables
      put the method to a while loop as a parameter.

    PARAMETERS:
    sensorValue - 16-bit value to be stored for statistical processing.
                  - Data type: non-negative integer
                  - Default value: none
                  - Limited range: 0 ~ 65535

    RETURN:
    Status flag about filling the data buffer (a free position in it).
    - true:  The data buffer expects additional values to register.
    - false: The data buffer is full of registered values.
  */
  bool registerData(uint16_t sensorValue);

  /*
    Calculate statistical value from all registered values.

    DESCRIPTION:
    The particular method calculates corresponding statistical value from
    currently registered sensor values in the data buffer.
    - The calculated statistic represents a series of sensor values stored
      in the data buffer.
    - A series of statistics from subsequent runs represent smoothed measurement
      of sensor data.
    - Usually a statistic is calculated after filling the entire data buffer
      when the registering method returns false, but the calculation can be
      executed before it just from currently registered values, if it is
      reasonable.

    PARAMETERS:  none

    RETURN:
    16-bit sensor statistic or zero value if buffer is empty, i.e., without any
    registered value.
  */
  uint16_t getMedian(); // 50% percentile
  uint16_t getAverage(); // Arithmetic mean
  uint16_t
  getMidAverage(); // Arithmetic mean without the lowest and greatest value
  uint16_t getMaximum(); // The highest value
  uint16_t getMinimum(); // The lowest value

  // Public getters
  inline uint8_t getReadings() { return bufferCnt_; };
  inline uint16_t getValueMin() { return valueMin_; };
  inline uint16_t getValueMax() { return valueMax_; };
  //
  static inline uint16_t getFilterMin() { return Limits::FILTER_MIN; };
  static inline uint16_t getFilterMax() { return Limits::FILTER_MAX; };
  //
  inline uint8_t getBufferLen() { return bufferLen_; };
  static inline uint8_t getBufferLenMin() { return Limits::BUFFERLEN_MIN; };
  static inline uint8_t getBufferLenMax() { return Limits::BUFFERLEN_MAX; };
  static inline uint8_t getBufferLenDft() { return Limits::BUFFERLEN_DFT; };
  //
  inline uint8_t getDelay() { return sensorDelay_; };
  static inline uint8_t getDelayMin() { return Limits::DELAY_MIN; };
  static inline uint8_t getDelayMax() { return Limits::DELAY_MAX; };
  static inline uint8_t getDelayDft() { return Limits::DELAY_DFT; };

  // Public setters
  inline void setFilterMax(uint16_t valueMax)
  {
    valueMax_ = constrain(valueMax, getFilterMin(), getFilterMax());
  };

  inline void setFilterMin(uint16_t valueMin)
  {
    valueMin_ = constrain(valueMin, Limits::FILTER_MIN, Limits::FILTER_MAX);
  };

  inline void setFilter(uint16_t valueMax, uint16_t valueMin)
  {
    setFilterMax(valueMax);
    setFilterMin(valueMin);
  };

  inline void setBufferLen(uint8_t bufferLen)
  {
    // Adjust buffer length to odd number
    bufferLen_ = bufferLen | 1;
    bufferLen_ = constrain(bufferLen_, getBufferLenMin(), getBufferLenMax());
    // Create of adjust data buffer if needed
    if (buffer_ == NULL)
    {
      buffer_ = (uint16_t *)calloc(getBufferLen(), sizeof(uint16_t));
    }
    else if (getBufferLen() != sizeof(buffer_) / sizeof(buffer_[0]))
    {
      buffer_ = (uint16_t *)realloc(buffer_, getBufferLen() * sizeof(uint16_t));
    }
  };

  inline void setDelay(uint8_t sensorDelay)
  {
    sensorDelay_ = constrain(sensorDelay, getDelayMin(), getDelayMax());
  };

private:
  enum Limits
  {
    FILTER_MIN = 0x0000, // Minimal valid sensor data
    FILTER_MAX = 0xFFFF, // Maximal valid sensor data
    BUFFERLEN_MIN = 3, // Minimal valid buffer length
    BUFFERLEN_MAX = 11, // Maximal valid buffer length
    BUFFERLEN_DFT = 5, // Default buffer length
    DELAY_MIN = 0, // Minimal registering delay in milliseconds
    DELAY_MAX = 100, // Maximal registering delay in milliseconds
    DELAY_DFT = 20, // Default registering delay in milliseconds
  };

  uint16_t *buffer_ = NULL; // Dynamic data buffer
  uint16_t valueMin_;
  uint16_t valueMax_;
  uint8_t bufferLen_;
  uint8_t bufferCnt_;
  uint8_t sensorDelay_;
};

#endif
