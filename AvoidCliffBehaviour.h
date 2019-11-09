#ifndef AvoidCliffBehaviour_h
#define AvoidCliffBehaviour_h

#include <Zumo32U4.h>
#include "LocalMotion.h"

//#define _BLACK_RING
#define _STOP_ON_CLIFF

// Behaviour for the robot when it encounters a cliff: stop, reverse
// - cliff detected at left, left-and-centre: turn right
// - cliff detected at right, right-and-centre: turn left
// - cliff detected at centre, left-and-right, left-centre-right: turn around
class AvoidCliffBehaviour: public LocalMotion
{
public:
	// A sensors reading must be greater than or equal to this
	// threshold in order for the program to consider that sensor as
	// seeing an object.
#ifdef _BLACK_RING
	static const uint16_t sensorThreshold = 200;
#else	
	static const uint16_t sensorThreshold = 900;
#endif
	static const uint8_t NUM_SENSORS = 3;

	AvoidCliffBehaviour(Zumo32U4LineSensors* lineSensors, char cmd) : LocalMotion(cmd) {
		this->lineSensors = lineSensors;
    randomSeed(millis());
	}


	bool requestAction() {
		// Read the line sensors.
		lineSensors->read(lineSensorValues, QTR_EMITTERS_ON_AND_OFF);
		cliffAhead = 0;
		for (int i = 0; i < NUM_SENSORS; i++)
		{
			// encode the line sensor values
#ifdef _BLACK_RING
			if (lineSensorValues[i] < sensorThreshold)
#else
			if (lineSensorValues[i] >= sensorThreshold)
#endif
			{
				cliffAhead += 1 << i;
			}
		}
		//printReadingsToSerial();
		return cliffAhead > 0;
	}

	void abortAction() {
		stop();
	}

	unsigned long executeActionAndInhibit() {
    stop();
    delay(120);
    reverse();
    delay(200);
    stop();
#ifndef _STOP_ON_CLIFF
    delay(120);
//    forward();
// In order not to get stuck, introduce a random factor to turnAround
//    if (random(10) < 3) {
//      turnAround();
//    }
//    else {
  		switch (cliffAhead)
  		{
  		case 1:
  		case 3:
  			// cliff detected at left, left-and-centre: turn right
  			turnRight();
  			break;
  
  		case 4:
  		case 6:
  			// cliff detected at right, right-and-centre: turn left
  			turnLeft();
  			break;
  		case 2:
  		case 7:
  		case 5:
  			// cliff detected at centre, left-and-right, left-centre-right: turn around
  			turnAround();
  			break;
  		default:
  			break;
  		}
//    }
#endif
		return 0;
	}

private:

	Zumo32U4LineSensors* lineSensors;
	uint16_t lineSensorValues[NUM_SENSORS];
	uint8_t cliffAhead;

};

#endif
