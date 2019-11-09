#ifndef SpinBehaviour_h
#define SpinBehaviour_h

#include <Zumo32U4.h>
#include "LocalMotion.h"

// Behaviour for the robot:
// - move forward, let the other behaviours take over
class SpinBehaviour: public LocalMotion
{
public:

	SpinBehaviour(char cmd) : LocalMotion(cmd) {
	}

	bool requestAction() {
		return true;
	}

	void abortAction() {
		stop();
	}

	unsigned long executeActionAndInhibit() {
    long num = random(10);
    if (num < 5) {
		  spinLeft();
    } else {
      spinRight();
    }

		return 10000;
	}

};

#endif
