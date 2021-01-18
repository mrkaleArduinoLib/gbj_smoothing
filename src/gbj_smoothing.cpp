#include "gbj_smoothing.h"
const String gbj_smoothing::VERSION = "GBJ_SMOOTHING 1.2.0";

// Constructor
gbj_smoothing::gbj_smoothing(
  uint16_t valueMax,
  uint16_t valueMin,
  uint8_t bufferLen,
  uint8_t sensorDelay)
{
  setFilter(valueMax, valueMin);
  setBufferLen(bufferLen);
  setDelay(sensorDelay);
  init();
}


// Register data item
bool gbj_smoothing::registerData(uint16_t sensorValue)
{
  if (sensorValue < getValueMin())
    return true;
  if (sensorValue > getValueMax())
    return true;
  // Register value
  if (getReadings() >= getBufferLen())
    init(); // Start new batch
  _buffer[_bufferCnt++] = sensorValue;
  delay(getDelay());
  return getReadings() < getBufferLen();
}


// Sensor data statistics
uint16_t gbj_smoothing::getMedian()
{
  if (getReadings() == 0)
    return 0;
  gbj_apphelpers::sort_buble_asc(_buffer, getReadings());
  // Round down median index
  return _buffer[(getReadings() - 1) / 2];
}


uint16_t gbj_smoothing::getAverage()
{
  if (getReadings() == 0)
    return 0;
  uint16_t statistic = 0;
  for (uint8_t i = 0; i < getReadings(); i++)
    statistic += _buffer[i];
  // Round up arithmetic mean
  return divide(statistic, getReadings());
}


uint16_t gbj_smoothing::getMidAverage()
{
  if (getReadings() == 0)
    return 0;
  gbj_apphelpers::sort_buble_asc(_buffer, getReadings());
  uint16_t statistic = 0;
  for (uint8_t i = 1; i < getReadings() - 1; i++)
    statistic += _buffer[i];
  // Round up arithmetic mean
  return divide(statistic, getReadings() - 2);
}


uint16_t gbj_smoothing::getMinimum()
{
  if (getReadings() == 0)
    return 0;
  uint16_t statistic = _buffer[0];
  for (uint8_t i = 1; i < getReadings(); i++)
  {
    if (_buffer[i] < statistic) statistic = _buffer[i];
  }
  return statistic;
}


uint16_t gbj_smoothing::getMaximum()
{
  if (getReadings() == 0)
    return 0;
  uint16_t statistic = _buffer[0];
  for (uint8_t i = 1; i < getReadings(); i++)
  {
    if (_buffer[i] > statistic) statistic = _buffer[i];
  }
  return statistic;
}