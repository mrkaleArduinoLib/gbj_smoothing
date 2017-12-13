/*
  NAME:
  gbj_filter_smoothing

  DESCRIPTION:
  The library provides storing subsequent data values (measurement burst)
  in 16-bit resolution in a batch of defined number of them and then
  calculates some statistical values from the data batch.
  - A calculated statistical value represents a data batch.
  - A series of those statistical values represents smoothed measurement
    from sensors with reduced digital noise, i.e., smoothing filtering.

  LICENSE:
  This program is free software; you can redistribute it and/or modify
  it under the terms of the license GNU GPL v3 http://www.gnu.org/licenses/gpl-3.0.html
  (related to original code) and MIT License (MIT) for added code.

  CREDENTIALS:
  Author: Libor Gabaj
  GitHub: https://github.com/mrkaleArduinoLib/gbj_filter_smoothing.git
 */
#ifndef GBJ_FILTER_SMOOTHING_H
#define GBJ_FILTER_SMOOTHING_H
#define GBJ_FILTER_SMOOTHING_VERSION "GBJ_FILTER_SMOOTHING 1.0.0"

#if defined(__AVR__)
  #if ARDUINO >= 100
    #include "Arduino.h"
  #else
    #include "WProgram.h"
  #endif
  #include <inttypes.h>
#elif defined(PARTICLE)
    #include "Particle.h"
    #include "math.h"
#endif

// Limits
#define GBJ_FILTER_SMOOTHING_MIN          0       // Minimal valid sensor data
#define GBJ_FILTER_SMOOTHING_MAX          65535   // Maximal valid sensor data
#define GBJ_FILTER_SMOOTHING_BUFFER_DEF   5       // Default number of readings in a run
#define GBJ_FILTER_SMOOTHING_BUFFER_MIN   3       // Minimal number of readings in a run
#define GBJ_FILTER_SMOOTHING_BUFFER_MAX   11      // Maximal number of readings in a run
#define GBJ_FILTER_SMOOTHING_DELAY_DEF    20      // Default registering delay in milliseconds
#define GBJ_FILTER_SMOOTHING_DELAY_MIN    0       // Minimal registering delay in milliseconds
#define GBJ_FILTER_SMOOTHING_DELAY_MAX    100     // Maximal registering delay in milliseconds

// Macro functions
#define swapdata(a, b) { if ((a) > (b)){int16_t t = a; a = b; b = t;} }
#define divide(a, b) { (uint16_t)round((float)(a)/(float)(b)) }


class gbj_filter_smoothing
{
public:
//------------------------------------------------------------------------------
// Public methods
//------------------------------------------------------------------------------


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
  - The constructor has all arguments defaulted. If some argument after
    some defaulted arguments should have a specific value, use corresponding
    constants in place of those defaulted arguments.

  PARAMETERS:
  valueMax - Maximal valid sensor value for registering.
             - Data type: non-negative integer
             - Default value: GBJ_FILTER_SMOOTHING_MAX
             - Limited range: GBJ_FILTER_SMOOTHING_MIN ~ GBJ_FILTER_SMOOTHING_MAX

  valueMin - Minimal valid sensor value for registering.
             - Data type: non-negative integer
             - Default value: GBJ_FILTER_SMOOTHING_MIN
             - Limited range: GBJ_FILTER_SMOOTHING_MIN ~ GBJ_FILTER_SMOOTHING_MAX

  bufferLen - Number of 16-bit values, which a statistical value is
              going to be calculated from.
              - The buffer length should be odd number. If it is not, the
                constructor adds 1 to it right before limiting the length
                in order to make it odd.
              - Data type: positive odd integer
              - Default value: GBJ_FILTER_SMOOTHING_BUFFER_DEF
              - Limited range: GBJ_FILTER_SMOOTHING_BUFFER_MIN ~ GBJ_FILTER_SMOOTHING_BUFFER_MAX

  sensorDelay - Number of milliseconds, which the system is going to be
                suspended after registering each data value to the data buffer.
                - The method does not utilize system delay() function but just
                  timestamp in milliseconds calculated by millis() function.
                - The delay enables to settle or consolidate a sensor after
                  reading, especially its shared (multiplexed) analog-digital
                  converter.
                - Data type: non-negative integer
                - Default value: GBJ_FILTER_SMOOTHING_DELAY_DEF
                - Limited range: GBJ_FILTER_SMOOTHING_DELAY_MIN ~ GBJ_FILTER_SMOOTHING_DELAY_MAX

  RETURN:
  Library instance object.
*/
  gbj_filter_smoothing( \
    uint16_t valueMax    = GBJ_FILTER_SMOOTHING_MAX, \
    uint16_t valueMin    = GBJ_FILTER_SMOOTHING_MIN, \
    uint8_t  bufferLen   = GBJ_FILTER_SMOOTHING_BUFFER_DEF, \
    uint8_t  sensorDelay = GBJ_FILTER_SMOOTHING_DELAY_DEF);


/*
  Reset all counters and status flags.

  DESCRIPTION:
  The method initiates all internal counters and status flags of a class
  instance object to default values as they are right after power up of
  a microcontroler.

  PARAMETERS: none

  RETURN: none
*/
  void init();


/*
  Put measured value to the buffer.

  DESCRIPTION:
  The method stores a measured sensor value to the data buffer for further
  statistical processing, which is in the valid range of values.
  - The parameter of smoothing like filtering range and measurement delay should
    be defined before registering the data by the constructor or setters.
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
  The particular method calculates corresponding statistical value from currently
  registered sensor values in the data buffer.
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
  uint16_t getMedian();       // 50% percentile
  uint16_t getAverage();      // Arithmetic mean
  uint16_t getMidAverage();   // Arithmetic mean without the lowest and greatest value
  uint16_t getMaximum();      // The highest value
  uint16_t getMinimum();      // The lowest value


//------------------------------------------------------------------------------
// Public setters
//------------------------------------------------------------------------------


/*
  Set filter for valid registered values.

  DESCRIPTION:
  The corresponding method redefines minimal, maximal, or both valid values
  for registered sensor data set in the constructor.

  PARAMETERS:
  valueMax - Maximal valid sensor value for registering.
             - Data type: non-negative integer
             - Default value: none
             - Limited range: as in the constructor

  valueMin - Minimal valid sensor value for registering.
             - Data type: non-negative integer
             - Default value: none
             - Limited range: as in the constructor

  RETURN: none
*/
  void setFilter(uint16_t valueMax, uint16_t valueMin);
  void setFilterMax(uint16_t valueMax);
  void setFilterMin(uint16_t valueMin);


/*
  Set delay for registering values.

  DESCRIPTION:
  The method redefines delay between registering data values set in the
  constructor.

  PARAMETERS:
  sensorDelay - The delay redefines the value set in the constructor.
                - Data type: non-negative integer
                - Default value: none
                - Limited range: as in the constructor

  RETURN: none
*/
  void setDelay(uint8_t sensorDelay);


//------------------------------------------------------------------------------
// Public getters
//------------------------------------------------------------------------------

/*
  Getter of the actual data buffer length.

  DESCRIPTION:
  The method returns current length of the data buffer used for registering
  sensor data.
  - Usually the returned value is the same as length put to the constructor.
  - If class has adjusted the input buffer length to the nearest odd number,
    the method returns the actual length.
  - The method is useful if the length has been put to the constructor as
    a numeric literal and there is no variable of the length to use it in
    other statements.

  PARAMETERS: none

  RETURN:
  Actual length of the data buffer.
*/
  uint8_t getBufferLen();


/*
  Getter of the actual number of registered sensor values.

  DESCRIPTION:
  The method returns number of currently registered sensor values in the
  data buffer.
  - The calculation of a statistical value can be provided before
    registering the full data buffer. In that case the method returns the
    number of sensor values, which a statistic is calculated from.
  - Usually the returned value should be the same as length of the data buffer
    at the end of a measurement cycle when method registerData() returns false.
  - Before end of a measurement cycle the method registerData() returns true
    and number of readings is less than length of the data buffer.

  PARAMETERS: none

  RETURN:
  Actual number of registered sensor values in the data buffer.
*/
  uint8_t getReadings();


/*
  Getters of the actual minimal and maximal valid sensor value.

  DESCRIPTION:
  The particular method returns currently set minimal or maximal value valid for
  registering.

  PARAMETERS: none

  RETURN:
  Actual minimal or maximal valid value
*/
  uint16_t getValueMin();
  uint16_t getValueMax();


/*
  Getter of the actual sensor delay.

  DESCRIPTION:
  The method returns currently set delay between registering data values for
  registering.

  PARAMETERS: none

  RETURN:
  Actual sensor delay in milliseconds.
*/
  uint8_t getDelay();


private:
//------------------------------------------------------------------------------
// Private attributes
//------------------------------------------------------------------------------
  uint16_t _buffer[GBJ_FILTER_SMOOTHING_BUFFER_MAX];   // Data buffer
  uint16_t _valueMin = GBJ_FILTER_SMOOTHING_MIN;       // Minimal valid value
  uint16_t _valueMax = GBJ_FILTER_SMOOTHING_MAX;       // Maximal valid value
  uint8_t _bufferLen;   // Data buffer length in data items
  uint8_t _bufferCnt;   // Current number of data items in buffer
  uint8_t _sensorDelay; // Default sensor delay


//------------------------------------------------------------------------------
// Private methods
//------------------------------------------------------------------------------
  void sort();          // Sort values in the data buffer in ascending order


  /*
    Wait for a period of time.

    DESCRIPTION:
    The method wait in the loop until input delay expires.

    PARAMETERS:
    delay - Waiting time period in milliseconds.
            - Data type: non-negative integer
            - Default value: none
            - Limited range: 0 ~ 2^32 - 1

    RETURN: none
  */
    void wait(uint32_t delay);
};

#endif
