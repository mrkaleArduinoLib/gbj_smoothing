#include "gbj_filter_smoothing.h"


gbj_filter_smoothing::gbj_filter_smoothing(uint16_t valueMax, uint16_t valueMin, uint8_t bufferLen, uint8_t sensorDelay)
{
  setFilter(valueMax, valueMin);
  setDelay(sensorDelay);
  // Adjust buffer length to odd number
  _bufferLen = constrain(bufferLen | 1, GBJ_FILTER_SMOOTHING_BUFFER_MIN, GBJ_FILTER_SMOOTHING_BUFFER_MAX);
  init();
}


// Initialize all status variables
void gbj_filter_smoothing::init()
{
  _bufferCnt = 0;
}


// Register data item
bool gbj_filter_smoothing::registerData(uint16_t sensorValue)
{
  if (sensorValue < _valueMin) return true;
  if (sensorValue > _valueMax) return true;
  // Register value
  if (_bufferCnt >= _bufferLen) init(); // Start new batch
  _buffer[_bufferCnt++] = sensorValue;
  if (_sensorDelay > 0) wait(_sensorDelay);
  return _bufferCnt < _bufferLen;
}


// Sensor data statistics
uint16_t gbj_filter_smoothing::getMedian()
{
  if (_bufferCnt == 0) return 0;
  sort();
  // Round down median index
  return _buffer[(_bufferCnt - 1)/2];
}


uint16_t gbj_filter_smoothing::getAverage()
{
  if (_bufferCnt == 0) return 0;
  uint16_t statistic = 0;
  for (uint8_t i = 0; i < _bufferCnt; i++) statistic += _buffer[i];
  // Round up arithmetic mean
  return divide(statistic, _bufferCnt);
}


uint16_t gbj_filter_smoothing::getMidAverage()
{
  if (_bufferCnt == 0) return 0;
  sort();
  uint16_t statistic = 0;
  for (uint8_t i = 1; i < _bufferCnt - 1; i++) statistic += _buffer[i];
  // Round up arithmetic mean
  return divide(statistic, _bufferCnt - 2);
}


uint16_t gbj_filter_smoothing::getMinimum()
{
  if (_bufferCnt == 0) return 0;
  uint16_t statistic = _buffer[0];
  for (uint8_t i = 1; i < _bufferCnt; i++)
  {
    if (_buffer[i] < statistic) statistic = _buffer[i];
  }
  return statistic;
}


uint16_t gbj_filter_smoothing::getMaximum()
{
  if (_bufferCnt == 0) return 0;
  uint16_t statistic = _buffer[0];
  for (uint8_t i = 1; i < _bufferCnt; i++)
  {
    if (_buffer[i] > statistic) statistic = _buffer[i];
  }
  return statistic;
}


//------------------------------------------------------------------------------
// Setters
//------------------------------------------------------------------------------

// Set filter for valid registered values
void gbj_filter_smoothing::setFilterMin(uint16_t valueMin)
{
  _valueMin = constrain(valueMin, GBJ_FILTER_SMOOTHING_MIN, GBJ_FILTER_SMOOTHING_MAX);
  swapdata(_valueMin, _valueMax); // Sort range values
}


void gbj_filter_smoothing::setFilterMax(uint16_t valueMax)
{
  _valueMax = constrain(valueMax, GBJ_FILTER_SMOOTHING_MIN, GBJ_FILTER_SMOOTHING_MAX);
  swapdata(_valueMin, _valueMax); // Sort range values
}


void gbj_filter_smoothing::setFilter(uint16_t valueMax, uint16_t valueMin)
{
  setFilterMax(valueMax);
  setFilterMin(valueMin);
}


// Set delay between registering data values
void gbj_filter_smoothing::setDelay(uint8_t sensorDelay)
{
  _sensorDelay = constrain(sensorDelay, GBJ_FILTER_SMOOTHING_DELAY_MIN, GBJ_FILTER_SMOOTHING_DELAY_MAX);
}


//------------------------------------------------------------------------------
// Getters
//------------------------------------------------------------------------------
uint8_t  gbj_filter_smoothing::getBufferLen() { return _bufferLen; };
uint8_t  gbj_filter_smoothing::getReadings()  { return _bufferCnt; };
uint16_t gbj_filter_smoothing::getValueMin()  { return _valueMin; };
uint16_t gbj_filter_smoothing::getValueMax()  { return _valueMax; };
uint8_t  gbj_filter_smoothing::getDelay()     { return _sensorDelay; };


//------------------------------------------------------------------------------
// Private methods
//------------------------------------------------------------------------------

// Sort array by bubblesort algorithm in ascending order
void gbj_filter_smoothing::sort()
{
  bool again = true;
  for(byte i = 0; i < (_bufferCnt - 1) && again; i++)
  {
    again = false;
    for(byte j = _bufferCnt - 1; j > i; --j)
    {
      if(_buffer[j] < _buffer[j-1])
      {
        uint16_t t = _buffer[j];
        _buffer[j] = _buffer[j-1];
        _buffer[j-1] = t;
        again = true;
      }
    }
  }
}


// Wait for delay period expiry
void gbj_filter_smoothing::wait(uint32_t delay)
{
  uint32_t timestamp = millis();
  while (millis() - timestamp < delay);
}
