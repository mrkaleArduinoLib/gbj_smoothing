<a id="library"></a>
# gbjFilterSmoothing
The library provides storing particular amount of subsequent readings (measurement burst) in 16 bit resolution and calculates relevant statistical values from them (median, average, middle average, minimum, and maximum).


<a id="dependency"></a>
## Dependency

#### Particle platform
- **Particle.h**: Includes alternative (C++) data type definitions.

#### Arduino platform
- **Arduino.h**: Main include file for the Arduino SDK version greater or equal to 100.
- **WProgram.h**: Main include file for the Arduino SDK version less than 100.
- **inttypes.h**: Integer type conversions. This header file includes the exact-width integer definitions and extends them with additional facilities provided by the implementation.


<a id="Constants"></a>
## Constants
- **GBJ\_FILTER\_SMOOTHING\_VERSION**: Name and semantic version of the library.
- **GBJ\_FILTER\_SMOOTHING\_MIN**: Minimal valid value for sensor data.
- **GBJ\_FILTER\_SMOOTHING\_MAX**: Maximal valid value for sensor data.
- **GBJ\_FILTER\_SMOOTHING\_BUFFER\_DEF**: Default number of readings in a run.
- **GBJ\_FILTER\_SMOOTHING\_BUFFER\_MIN**: Minimal number of readings in a run.
- **GBJ\_FILTER\_SMOOTHING\_BUFFER\_MAX**: Maximal number of readings in a run.
- **GBJ\_FILTER\_SMOOTHING\_DELAY\_DEF**: Default registering delay in milliseconds.
- **GBJ\_FILTER\_SMOOTHING\_DELAY\_MIN**: Minimal registering delay in milliseconds.
- **GBJ\_FILTER\_SMOOTHING\_DELAY\_MAX**: Maximal registering delay in milliseconds.


<a id="interface"></a>
## Interface
- [gbj_filter_smoothing()](#gbj_filter_smoothing)
- [init()](#init)
- [registerData()](#registerData)
- [getMedian()](#getMedian)
- [getAverage()](#getAverage)
- [getMidAverage()](#getMidAverage)
- [getMaximum()](#getMaximum)
- [getMinimum()](#getMinimum)

#### Setters
- [setFilterMin()](#setFilter)
- [setFilterMax()](#setFilter)
- [setFilter()](#setFilter)
- [setDelay()](#setDelay)

#### Getters
- [getValueMin()](#getValueRange)
- [getValueMax()](#getValueRange)
- [getDelay()](#getDelay)
- [getBufferLen()](#getBufferLen)
- [getReadings()](#getReadings)


<a id="gbj_filter_smoothing"></a>
## gbj_filter_smoothing()
#### Description
Constructor creates the data buffer within a class instance object, which holds a series of data values that should be substituted by a statistical value.
- Class enables delay between registering data values in order to settle a sensor, especially its analog-digital converter between readings.
- One class instance object can be employ for statistical processing of multiple parameters (sensors).
- Input arguments values are considered as global and valid for all processed statistical variables and all their readings.
- If some input argument value is not suitable for particular statistical variable, you should create a separate instance object for each variable (sensor).
- Smoothing sensor readings eliminates the measurement noise in the analog data.
- The class may be used for original maximal 16-bit non-negative digital data as well, where the statistical processing is desirable.

#### Syntax
    gbj_filter_smoothing(uint16_t valueMax, uint16_t valueMin, uint8_t bufferLen, uint8_t sensorDelay);

#### Parameters
<a id="prm_valueMax"></a>
- **valueMax**: Maximal valid sensor value for registering.
  - *Valid values*: non-negative integer 0 ~ 65365 ([GBJ\_FILTER\_SMOOTHING\_MIN ~ GBJ\_FILTER\_SMOOTHING\_MAX](#constants))
  - *Default value*: 65365 ([GBJ\_FILTER\_SMOOTHING\_MAX](#constants))

<a id="prm_valueMin"></a>
- **valueMin**: Minimal valid sensor value for registering.
  - *Valid values*: non-negative integer 0 ~ 65365 ([GBJ\_FILTER\_SMOOTHING\_MIN ~ GBJ\_FILTER\_SMOOTHING\_MAX](#constants))
  - *Default value*: 0 ([GBJ\_FILTER\_SMOOTHING\_MIN](#constants))

<a id="prm_bufferLen"></a>
- **bufferLen**: Number of 16-bit values, which a statistical value is going to be calculated from.
  - The buffer length should be odd number. If it is not, the constructor adds 1 to it right before limiting the length in order to make it odd.
  - *Valid values*: positive odd integer in the range 3 to 11 ([GBJ\_FILTER\_SMOOTHING\_BUFFER\_MIN ~ GBJ\_FILTER\_SMOOTHING\_BUFFER\_MAX](#constants))
  - *Default value*: 5 ([GBJ\_FILTER\_SMOOTHING\_BUFFER\_DEF](#constants))

<a id="prm_sensorDelay"></a>
- **sensorDelay**: Default number of milliseconds, which the system is going to be suspended after registering each data value to the data buffer.
  - The delay enables to settle or consolidate a sensor after reading, especially its shared (multiplexed) analog-digital converter.
  - *Valid values*: non-negative integer 0 ~ 100 ms ([GBJ\_FILTER\_SMOOTHING\_DELAY\_MIN ~ GBJ\_FILTER\_SMOOTHING\_DELAY\_MAX](#constants))
  - *Default value*: 20 ms ([GBJ\_FILTER\_SMOOTHING\_DELAY\_DEF](#constants))

#### Returns
Object preforming the smoothing data from sensors.

#### Example
The constructor has all arguments defaulted. The constructor instance without any parameters is equivalent to an instance with all arguments set by corresponding constant with default value:

``` cpp
gbj_filter_smoothing Samples = gbj_filter_smoothing(); // It is equivalent to
gbj_filter_smoothing Samples = gbj_filter_smoothing(GBJ_FILTER_SMOOTHING_MAX, GBJ_FILTER_SMOOTHING_MIN, GBJ_FILTER_SMOOTHING_BUFFER_DEF, GBJ_FILTER_SMOOTHING_DELAY_DEF);
```

If some argument after some defaulted arguments should have a specific value, use corresponding constants in place of those defaulted arguments, e.g.

``` cpp
gbj_filter_smoothing Samples = gbj_filter_smoothing(GBJ_FILTER_SMOOTHING_MAX, GBJ_FILTER_SMOOTHING_MIN, 11); // Specific buffer length
gbj_filter_smoothing Samples = gbj_filter_smoothing(GBJ_FILTER_SMOOTHING_MAX, GBJ_FILTER_SMOOTHING_MIN, GBJ_FILTER_SMOOTHING_BUFFER_DEF, 10); // Specific delay
```

Typical usage is just with setting a valid range in the constructor, e.g., at 10-bit ADC:

``` cpp
gbj_filter_smoothing Samples = gbj_filter_smoothing(1023);
gbj_filter_smoothing Samples = gbj_filter_smoothing(768, 16);
```

#### See also
[setFilterMax()](#setFilter)

[setFilterMin()](#setFilter)

[setFilter()](#setFilter)

[setDelay()](#setDelay)

[Back to interface](#interface)


<a id="init"></a>
## init()
#### Description
The method initiates all internal counters and status flags of a class instance object to default values as they are right after power up of a microcontroller.

#### Syntax
    void init();

#### Parameters
None

#### Returns
None

[Back to interface](#interface)


<a id="registerData"></a>
## registerData()
#### Description
The method stores a measured sensor value to the data buffer for further statistical processing.
- The method ignores a value, which is outside of the valid range defined by the corresponding input arguments in the constructor or defined by the [filter setters](#setFilter).
- The method returns true status until the object buffer is full, i.e., entire set of values determined by the buffer length for the current run is registered and statistical processing can be provided. It enables put the method to a while loop as a parameter.

#### Syntax
    boolean registerData(uint16_t sensorValue);

#### Parameters
- **sensorValue**: 16-bit value to be stored for statistical processing.
  - *Valid values*: non-negative integer 0 ~ 65535, but restricted by current filter range
  - *Default value*: none

#### Returns
Status flag about filling the data buffer (free position in it).
- **true**: the data buffer expects additional values to register
- **false**: the data buffer is full of registered values

#### Example

``` cpp
while (Samples.registerData(analogRead(pin)));
```

#### See also
[gbj_filter_smoothing()](#gbj_filter_smoothing)

[Back to interface](#interface)


<a id="getMedian"></a>
## getMedian()
#### Description
The method calculates median (50% percentile) from currently  registered sensor values in the data buffer as the statistic representing a series of sensor values stored in the buffer.
- A series of statistics from subsequent runs represents smoothed measurement of sensor data.
- Usually the statistic is calculated after filling the entire data buffer when the [registering method](#registerData) returns false. However, the calculation can be executed before it just from currently registered values, if it is reasonable.

#### Syntax
    uint16_t getMedian();

#### Parameters
None

#### Returns
16-bit sensor median or 0, if no data is registered.

#### See also
[getAverage()](#getAverage)

[Back to interface](#interface)


<a id="getAverage"></a>
## getAverage()
#### Description
The method calculates average (arithmetic mean) from currently registered sensor values in the data buffer as the statistic representing a series of sensor values stored in the buffer.
- A series of statistics from subsequent runs represents smoothed measurement of sensor data.
- Usually the statistic is calculated after filling the entire data buffer when the [registering method](#registerData) returns false. However, the calculation can be executed before it just from currently registered values, if it is reasonable.

#### Syntax
    uint16_t getAverage();

#### Parameters
None

#### Returns
16-bit sensor average or 0, if no data is registered.

#### See also
[getMedian()](#getMedian)

[Back to interface](#interface)


<a id="getMidAverage"></a>
## getMidAverage()
#### Description
The method calculates average (arithmetic mean) from currently registered sensor values in the data buffer, but without the lowest as well as the greatest value, as the statistic representing a middle series of sensor values stored in the buffer.
- A series of statistics from subsequent runs represents smoothed measurement of sensor data.
- Usually the statistic is calculated after filling the entire data buffer when the [registering method](#registerData) returns false. However, the calculation can be executed before it just from currently registered values, if it is reasonable.

#### Syntax
    uint16_t getMidAverage();

#### Parameters
None

#### Returns
16-bit sensor middle average or 0, if no data is registered.

#### See also
[getAverage()](#getAverage)

[Back to interface](#interface)


<a id="getMaximum"></a>
## getMaximum()
#### Description
The method calculates maximum (the highest value) from currently registered sensor values in the data buffer as the statistic representing a series of sensor values stored in the  buffer.
- A series of statistics from subsequent runs represents smoothed measurement of sensor data.
- Usually the statistic is calculated after filling the entire data buffer when the [registering method](#registerData) returns false. However, the calculation can be executed before it just from currently registered values, if it is reasonable.

#### Syntax
    uint16_t getMaximum();

#### Parameters
None

#### Returns
16-bit sensor maximum or 0, if no data is registered.

#### See also
[getMinimum()](#getMinimum)

[Back to interface](#interface)


<a id="getMinimum"></a>
## getMinimum()
#### Description
The method calculates minimum (the lowest value) from currently registered sensor values in the data buffer as the statistic representing a series of sensor values stored in the buffer.
- A series of statistics from subsequent runs represents smoothed measurement of sensor data.
- Usually the statistic is calculated after filling the entire data buffer when the [registering method](#registerData) returns false. However, the calculation can be executed before it just from currently registered values, if it is reasonable.

#### Syntax
    uint16_t getMinimum();

#### Parameters
None

#### Returns
16-bit sensor minimum or 0, if no data is registered.

#### See also
[getMaximum()](#getMaximum)

[Back to interface](#interface)


<a id="setFilter"></a>
## setFilter(), setFilterMax(), setFilterMin()
#### Description
The corresponding method redefines minimal, maximal, or both valid values for registered sensor data set in the constructor defined there by default or explicitly.

#### Syntax
    void setFilter(uint16_t valueMax, uint16_t valueMin);
    void setFilterMax(uint16_t valueMax);
    void setFilterMin(uint16_t valueMin);

#### Parameters
- **valueMax**: Maximal valid sensor value for registering.
  - *Valid values*: as in the constructor argument [valueMax](#prm_valueMax)
  - *Default value*: as in the constructor argument [valueMax](#prm_valueMax)

- **valueMin**: Minimal valid sensor value for registering.
  - *Valid values*: as in the constructor argument [valueMin](#prm_valueMin)
  - *Default value*: as in the constructor argument [valueMin](#prm_valueMin)

#### Returns
None

#### See also
[getValueMax()](#getValueRange)

[getValueMin()](#getValueRange)

[gbj_filter_smoothing()](#gbj_filter_smoothing)

[registerData()](#registerData)

[Back to interface](#interface)


<a id="getValueRange"></a>
## getValueMax(), getValueMin()
#### Description
The corresponding method returns currently set minimal or maximal value valid for registering.

#### Syntax
    uint16_t getValueMax();
    uint16_t getValueMin();

#### Parameters
None

#### Returns
Actual minimal or maximal value of the valid data range.

#### See also
[setFilter()](#setFilter)

[setFilterMax()](#setFilter)

[setFilterMin()](#setFilter)

[Back to interface](#interface)


<a id="setDelay"></a>
## setDelay()
#### Description
The method redefines delay between registering data values set in the constructor defined there by default or explicitly.
- The method does not utilize the system function *delay()*, but the timestamp obtained by the system function *millis()*.

#### Syntax
    void setDelay(uint8_t sensorDelay);

#### Parameters
- **sensorDelay**: The delay redefines the value set in the constructor.
  - *Valid values*: as in the constructor argument [sensorDelay](#prm_sensorDelay)
  - *Default value*: as in the constructor argument [sensorDelay](#prm_sensorDelay)

#### Returns
None

#### See also
[getDelay()](#getDelay)

[gbj_filter_smoothing()](#gbj_filter_smoothing)

[register()](#register)

[Back to interface](#interface)


<a id="getDelay"></a>
## getDelay()
#### Description
The method returns currently set delay between registering data values for registering.

#### Syntax
    uint8_t getDelay();

#### Parameters
None

#### Returns
Actual sensor delay in milliseconds.

#### See also
[setDelay()](#setDelay)

[Back to interface](#interface)


<a id="getBufferLen"></a>
## getBufferLen()
#### Description
The method returns current length of the data buffer used for registering sensor data.
- Usually the returned value is the same as length put to the [constructor](#gbj_filter_smoothing) as parameter [bufferLen](#prm_bufferLen).
- If class has adjusted the input buffer length to the nearest odd number, the method return the actual length.
- The method is useful if the length has been put to the constructor as a numeric literal and there is no variable of the length to use it in other statements.

#### Syntax
    uint8_t getBufferLen();

#### Parameters
None

#### Returns
Actual length of the data buffer.

#### See also
[gbj_filter_smoothing()](#gbj_filter_smoothing)

[Back to interface](#interface)


<a id="getReadings"></a>
## getReadings()
#### Description
The method returns number of currently registered sensor values in the data buffer.
- The calculation of a statistical value can be provided before registering the full data buffer. In that case the method returns the number of sensor values, which a statistic is calculated from.
- Usually the returned value should be the same as [length of the data buffer](#getBufferLen) at the end of a measurement cycle when method [registerData()](#registerData) returns *false*.
- Before end of a measurement cycle the method [registerData](#registerData()) returns *true* and number of readings is less than [length of the data buffer](#getBufferLen).

#### Syntax
    uint8_t getReadings();

#### Parameters
None

#### Returns
Actual number of registered sensor values in the data buffer.

#### See also
[registerData()](#registerData)

[Back to interface](#interface)
