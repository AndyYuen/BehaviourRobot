#ifndef CruiseBehaviour_h
#define CruiseBehaviour_h

#include <Zumo32U4.h>
#include "LocalMotion.h"

// Behaviour for the robot:
// - move forward, let the other behaviours take over
class CruiseBehaviour: public LocalMotion
{
public:

	CruiseBehaviour(char cmd) : LocalMotion(cmd) {
	}

	bool requestAction() {
		return true;
	}

	void abortAction() {
		stop();
	}

	unsigned long executeActionAndInhibit() {
    forward();
		return 500;
	}

};

#endif
