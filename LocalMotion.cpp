#include <Zumo32U4.h>
#include "Behaviour.h"
#include "LocalMotion.h"
#include "TurnSensor.h"

//Behaviour for the robot when it encounters an obstacle either on the left or the right:
//it backs up and spins either left or right depending on the obstacle is located
//or the right or left respectively

//speed between -400 and 400
static int16_t LocalMotion::straightLineSpeed = 200;
static int16_t LocalMotion::turnSpeed = 150;
static int16_t LocalMotion::spinSpeed = 400;
static int16_t LocalMotion::ramSpeed = 400;
static unsigned long LocalMotion::turnDelay = 120;
static unsigned long LocalMotion::turnaroundDelay = 1200;
static unsigned long LocalMotion::spinDelay = 810;

LocalMotion::LocalMotion(char cmd) : Behaviour(cmd) {
}

void LocalMotion::forward()
{
  stop();
  motors.setSpeeds(straightLineSpeed, straightLineSpeed);
}

void LocalMotion::reverse()
{
  stop();
  motors.setSpeeds(-straightLineSpeed, -straightLineSpeed);
}

void LocalMotion::turnLeft()
{
  stop();
  motors.setSpeeds(-turnSpeed, turnSpeed);
  //delay(turnDelay);
  (TurnSensor::getInstance())->waitTillTurnedNumberOfDegreesLeft(30);
  stop();
}

void LocalMotion::turnRight()
{
  stop();
  motors.setSpeeds(turnSpeed, -turnSpeed);
  //delay(turnDelay);
  (TurnSensor::getInstance())->waitTillTurnedNumberOfDegreesRight(30);
  stop();
}

void LocalMotion::timidLeft()
{
  stop();
  motors.setSpeeds(-ramSpeed, -ramSpeed);
  delay(turnDelay);
  motors.setSpeeds(-ramSpeed, ramSpeed);
  //delay(turnDelay);
  (TurnSensor::getInstance())->waitTillTurnedNumberOfDegreesLeft(30);
  stop();
}

void LocalMotion::timidRight()
{
  stop();
  motors.setSpeeds(-ramSpeed, -ramSpeed);
  delay(turnDelay);
  motors.setSpeeds(ramSpeed, -ramSpeed);
  //delay(turnDelay);
  (TurnSensor::getInstance())->waitTillTurnedNumberOfDegreesRight(30);
  stop();
}

void LocalMotion::speedLeft()
{
  stop();
  motors.setSpeeds(-straightLineSpeed, straightLineSpeed);
  //delay(turnDelay);
  (TurnSensor::getInstance())->waitTillTurnedNumberOfDegreesLeft(30);
  stop();
}

void LocalMotion::speedRight()
{
  stop();
  motors.setSpeeds(straightLineSpeed, -straightLineSpeed);
  //delay(turnDelay);
  (TurnSensor::getInstance())->waitTillTurnedNumberOfDegreesRight(30);
  stop();
}
void LocalMotion::turnAround()
{
  stop();
  motors.setSpeeds(turnSpeed, -turnSpeed);
  //delay(turnaroundDelay);
  (TurnSensor::getInstance())->waitTillTurnedNumberOfDegreesLeft(180);
  stop();
}

	void LocalMotion::spinLeft()
	{
	stop();
    motors.setSpeeds(-spinSpeed, spinSpeed);
    //delay(spinDelay);
    (TurnSensor::getInstance())->waitTillTurnedNumberOfDegreesLeft(360);
    stop();
	}

	void LocalMotion::spinRight()
	{
    //spinLeft();

	stop();
    motors.setSpeeds(spinSpeed, -spinSpeed);
    //delay(spinDelay);
    (TurnSensor::getInstance())->waitTillTurnedNumberOfDegreesRight(360);
    stop();

	}
	
	void LocalMotion::ram()
	{
	stop();
    motors.setSpeeds(ramSpeed, ramSpeed);
	}

void LocalMotion::stop()
{
  motors.setSpeeds(0, 0);
}

void LocalMotion::setSpeed(int16_t speed) {
  straightLineSpeed = speed;
}

int16_t LocalMotion::getSpeed() {
  return straightLineSpeed;
}
