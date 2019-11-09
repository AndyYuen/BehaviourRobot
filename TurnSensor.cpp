#include "TurnSensor.h"

//#define _ENABLE_LCD 1

#define READ_DELAY  15

TurnSensor* TurnSensor::instance = NULL;
L3G* TurnSensor::gyro = NULL;

// debug only: let them be initialised to NULL, we just don't create the objects if _ENABLE_LCD is not defined
Zumo32U4ButtonA * TurnSensor::buttonA = NULL;
Zumo32U4LCD * TurnSensor::lcd = NULL;


TurnSensor* TurnSensor::getInstance() {
  if (instance == NULL) {
    gyro = new L3G();
    instance = new TurnSensor();
  }
  return instance;
}


void TurnSensor::setupTurnSensor() {
  Wire.begin();
  gyro->init();

  // 800 Hz output data rate,
  // low-pass filter cutoff 100 Hz
  gyro->writeReg(L3G::CTRL1, 0b11111111);

  // 2000 dps full scale
  gyro->writeReg(L3G::CTRL4, 0b00100000);

  // High-pass filter disabled
  gyro->writeReg(L3G::CTRL5, 0b00000000);

  // Turn on the yellow LED 
  ledYellow(1);

  // Delay to give the user time to remove their finger.
  delay(500);

  // Calibrate the gyro->
  int32_t total = 0;
  for (uint16_t i = 0; i < 1024; i++)
  {
    // Wait for new data to be available, then read it.
    while(!gyro->readReg(L3G::STATUS_REG) & 0x08);
    gyro->read();

    // Add the Z axis reading to the total.
    total += gyro->g.z;
  }

  // Turn off the yellow LED
  ledYellow(0);
  gyroOffset = total / 1024;

  // Display the angle (in degrees from -180 to 180) until the
  // user presses A.
#ifdef _ENABLE_LCD
  lcd->clear();
  resetTurnSensor();
  while (!buttonA->getSingleDebouncedRelease())
  {
    updateTurnSensor();
    displayAngle(getTurnAngle());
  }
  lcd->clear();
#endif
}

void TurnSensor::resetTurnSensor() {
  gyroLastUpdate = micros();
  turnAngle = 0;
}

void TurnSensor::updateTurnSensor() {
  // Read the measurements from the gyro->
  gyro->read();
  turnRate = gyro->g.z - gyroOffset;

  // Figure out how much time has passed since the last update (dt)
  uint16_t m = micros();
  uint16_t dt = m - gyroLastUpdate;
  gyroLastUpdate = m;

  // Multiply dt by turnRate in order to get an estimation of how
  // much the robot has turned since the last update.
  // (angular change = angular velocity * time)
  int32_t d = (int32_t)turnRate * dt;

  // The units of d are gyro digits times microseconds.  We need
  // to convert those to the units of turnAngle, where 2^29 units
  // represents 45 degrees.  The conversion from gyro digits to
  // degrees per second (dps) is determined by the sensitivity of
  // the gyro: 0.07 degrees per second per digit.
  //
  // (0.07 dps/digit) * (1/1000000 s/us) * (2^29/45 unit/degree)
  // = 14680064/17578125 unit/(digit*us)
  turnAngle += (int64_t)d * 14680064 / 17578125;

}
  
TurnSensor::TurnSensor() {
  //Wire.begin();
  gyro = new L3G();
  gyroOffset = 0;
  gyroLastUpdate = 0;
  turnAngle = 0;
  turnRate = 0;

  // debug only
#ifdef _ENABLE_LCD
  buttonA = new Zumo32U4ButtonA();
  lcd = new Zumo32U4LCD();
#endif

  setupTurnSensor();
  delay(500);
  resetTurnSensor();
}

uint32_t TurnSensor::getTurnAngleRaw() {
  return turnAngle;
}

int32_t TurnSensor::getTurnAngle() {
  return (((int32_t)turnAngle >> 16) * 360) >> 16;
}

int16_t TurnSensor::getTurnRate() {
  return turnRate;
}

void TurnSensor::waitTillTurnedNumberOfDegreesLeft(int32_t degrees) {

  resetTurnSensor();

  updateTurnSensor();
  int32_t startAngle = getTurnAngle();
  int32_t target = degrees - startAngle;

#ifdef _ENABLE_LCD
  displayAngle(startAngle);
#endif
  
  if (target < 180) {
     do {
      delay(READ_DELAY);
      updateTurnSensor();
    } while (getTurnAngle() < target);
  }
  else {
    if (startAngle < 0) {
       do {
        delay(READ_DELAY); 
        updateTurnSensor();
      } while (getTurnAngle() < 0);
    }
    do {
      delay(READ_DELAY); 
      updateTurnSensor();
    } while (getTurnAngle() > 0);

    target -= 180 -startAngle; 
    target = target % 180;
    do {
      delay(READ_DELAY);
      updateTurnSensor();
    } while (getTurnAngle() < target);
  }

}

void TurnSensor::waitTillTurnedNumberOfDegreesRight(int32_t degrees) {

  resetTurnSensor();

  updateTurnSensor();
  int32_t startAngle = getTurnAngle();
  int32_t target = -degrees + startAngle;

#ifdef _ENABLE_LCD
  displayAngle(startAngle);
#endif

  if (abs(target) < 180) {
     do {
      delay(READ_DELAY);
      updateTurnSensor();
    } while (getTurnAngle() > target);
  }
  else {
    if (startAngle > 0) {
       do {
        delay(READ_DELAY); 
        updateTurnSensor();
      } while (getTurnAngle() > 0);
    }
    do {
      delay(READ_DELAY); 
      updateTurnSensor();
    } while (getTurnAngle() < 0);

    target -= -180  + startAngle; 
    target = target % 180;
    do {
      delay(READ_DELAY);
      updateTurnSensor();
    } while (getTurnAngle() > abs(target));
  }

}

void TurnSensor::displayAngle(int16_t angle) {
#ifdef _ENABLE_LCD
  //lcd->clear();

  lcd->gotoXY(0, 0);
  lcd->print(angle);
  lcd->print(F("   "));

#endif
}
