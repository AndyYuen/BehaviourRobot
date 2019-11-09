#ifndef TurnSensor_h
#define TurnSensor_h

#include <Wire.h>
#include <Zumo32U4.h>

class TurnSensor
{
public:
  
  static TurnSensor *getInstance();

  void resetTurnSensor();
  void updateTurnSensor();
  uint32_t getTurnAngleRaw();
  int32_t getTurnAngle();
  int16_t getTurnRate();

  void waitTillTurnedNumberOfDegreesLeft(int32_t degrees);
  void waitTillTurnedNumberOfDegreesRight(int32_t degrees);

private:
  TurnSensor();
  
  static L3G *gyro;
  static TurnSensor *instance;
  uint32_t turnAngle;
  int16_t turnRate;
  int16_t gyroOffset;
  uint16_t gyroLastUpdate;

  void setupTurnSensor();

  // debug only
  static Zumo32U4ButtonA *buttonA;
  static Zumo32U4LCD *lcd;

  void displayAngle(int16_t angle);



};
#endif
