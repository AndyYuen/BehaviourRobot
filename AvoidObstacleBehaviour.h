#ifndef AvoidObstacleBehaviour_h
#define AvoidObstacleBehaviour_h

#include <Zumo32U4.h>
#include "LocalMotion.h"

// Behaviour for the robot when it encounters an obstacle either on the left or the right:
// - Turn right or left respectively
// - Turn around if both left and right are blocked
class AvoidObstacleBehaviour: public LocalMotion
{
public:
	// A sensors reading must be greater than or equal to this
	// threshold in order for the program to consider that sensor as
	// seeing an object.
	static const uint8_t sensorThreshold = 6;

	AvoidObstacleBehaviour(Zumo32U4ProximitySensors* proxSensors, char cmd) : LocalMotion(cmd) {
		this->proxSensors = proxSensors;
	}

	bool requestAction() {
		proxSensors->read();
		leftValue = proxSensors->countsFrontWithLeftLeds();
		rightValue = proxSensors->countsFrontWithRightLeds();
		return leftValue >= sensorThreshold || rightValue >= sensorThreshold;
	}

	void abortAction() {
		stop();
	}

	unsigned long executeActionAndInhibit() {
		if (leftValue < rightValue)
		{
		  // The right value is greater, so the object is probably
		  // closer to the robot's right LEDs, which means the robot
		  // is not facing it directly.  Turn to the right to try to
		  // make it more even.
		  turnRight();

		}
		else if (leftValue > rightValue)
		{
		  // The left value is greater, so turn to the left.
		  turnLeft();
		}
		else
		{
		  // The values are equal, so stop the motors.
		  turnAround();
		}
		return 0;
	}

private:
	Zumo32U4ProximitySensors* proxSensors;
	uint8_t leftValue = 0;
	uint8_t rightValue = 0;


};

#endif
