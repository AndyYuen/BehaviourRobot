#ifndef BehaviourBasedRobot_h
#define BehaviourBasedRobot_h

#include "Behaviour.h"

class BehaviourBasedRobot {

public:
	static const int MAX_BEHAVIOURS_SUPPORTED = 16;


	BehaviourBasedRobot() {
	}
	
	void reset() {
		behavioursDefined = 0;
		executingBehaviour = -1;
	}

	bool disableBehaviour(char cmd) {
    for (int i = 0; i < behavioursDefined; i++) {
  		if (behaviours[i]->getCmd() == cmd)
  		{
  			enabledState[i] = false;
        return true;
  		}
    }
    return false;
	}

	bool enableBehaviour(char cmd) {
    for (int i = 0; i < behavioursDefined; i++) {
      if (behaviours[i]->getCmd() == cmd)
      {
        enabledState[i] = true;
        return true;
      }
    }
    return false;
  }

  void disableBehaviour(int index) {
    if (index < MAX_BEHAVIOURS_SUPPORTED)
    {
      enabledState[index] = false;
    }
  }

  void enableBehaviour(int index) {
    if (index < MAX_BEHAVIOURS_SUPPORTED)
    {
      enabledState[index] = true;
    }
  }
	void addBehaviour(Behaviour* behaviour) {
		if (behavioursDefined < MAX_BEHAVIOURS_SUPPORTED)
		{
			behaviours[behavioursDefined] = behaviour;
			enableBehaviour(behavioursDefined);
			inhibitUntil[behavioursDefined] = 0;
			behavioursDefined++;
		}
	}
	
	// scan the configured behaviours and execute the first one (highest priority) that requested control
	void run() {
		unsigned long now = millis();
		for (int i = 0; i < behavioursDefined; i++) {
			if (enabledState[i] && (inhibitUntil[i] <= now) &&behaviours[i]->requestAction())
			{
       
				inhibitUntil[i] = 0;
				if (executingBehaviour >= 0)
				{
//          Serial.print("Behaviour executing: "); Serial.println( behaviours[executingBehaviour]->getCmd());
					behaviours[executingBehaviour]->abortAction();
				}
//        Serial.print("Behaviour executing next: "); Serial.println( behaviours[i]->getCmd());
				unsigned long period = behaviours[i]->executeActionAndInhibit();
				if (period != 0)
				{
					inhibitUntil[i] = millis() + period;
				}
				executingBehaviour = i;
				break;
			}
		}

	}


private:
	Behaviour* behaviours[MAX_BEHAVIOURS_SUPPORTED];
	bool enabledState[MAX_BEHAVIOURS_SUPPORTED];
	unsigned long inhibitUntil[MAX_BEHAVIOURS_SUPPORTED];
	int behavioursDefined = 0;
	int executingBehaviour = -1;


};


#endif
