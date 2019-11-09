#ifndef Behaviour_h
#define Behaviour_h

#include <Wire.h>
#include <Zumo32U4.h>

// behaviour base class
class Behaviour {

public:
	Behaviour(char cmd) {
		this->cmd = cmd;
	}

	char getCmd() {
		return cmd;
	}
	
	// returns true if this behaviour want to request action
  // returns false otherwise
	virtual bool requestAction();

  // abort the current action
	virtual void abortAction();

  // returns 0 if not to inhibit further action
  // otherwise returns number of milliseconds to inbibit further actn
	virtual unsigned long executeActionAndInhibit();

		
protected:	
	// command to activate this behaviour (for remote testing only)
	char cmd;

};

#endif
