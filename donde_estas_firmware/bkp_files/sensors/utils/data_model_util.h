#ifndef _DATA_MODEL_UTIL_H_
#define _DATA_MODEL_UTIL_H_

#include <cmath>

typedef struct {
  float voltage;
  float valueCalibration1;
  float valueCalibration2;
  float valueCalibration3;
  int16_t adsSignal;
} SensorDataModel;

float roundTwoDecimalPlaces(float value) {
  return roundf(value * 100.0) / 100.0;
}

#endif /* _DATA_MODEL_UTIL_H_ */
