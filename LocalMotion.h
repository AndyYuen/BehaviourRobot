#ifndef LocalMotion_h
#define LocalMotion_h

#include <Zumo32U4.h>
#include "Behaviour.h"

//Behaviour for the robot when it encounters an obstacle either on the left or the right:
//it backs up and spins either left or right depending on the obstacle is located
//or the right or left respectively
class LocalMotion: public Behaviour
{
public:

	LocalMotion(char cmd);

	void forward();
  void reverse();
	void turnLeft();
	void turnRight();
	void timidLeft();
	void timidRight();
	void speedLeft();
	void speedRight();
	void turnAround();
	void spinLeft();
	void spinRight();
	void ram();
	void stop();
	void setSpeed(int16_t speed);
	int16_t getSpeed();

	virtual bool requestAction();
	virtual void abortAction();
	virtual unsigned long executeActionAndInhibit();

protected:


	static Zumo32U4Motors motors;
  //static Compass compass;
  static int16_t straightLineSpeed;
  static int16_t turnSpeed;
  static int16_t spinSpeed;
  static int16_t ramSpeed;
  static unsigned long turnDelay;
  static unsigned long turnaroundDelay;
  static unsigned long spinDelay;


};


#endif
