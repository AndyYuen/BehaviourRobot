#ifndef RemoteCommandBehaviour_h
#define RemoteCommandBehaviour_h

#include <Zumo32U4.h>
#include "LocalMotion.h"


// Behaviour for the robot when it receives a serial command:
// - act on the command
class RemoteCommandBehaviour: public LocalMotion
{
public:

  enum COMMANDS {
    CMD_NO_COMMAND,
    CMD_SPEED,
    CMD_FORWARD,
    CMD_BACKWARD,
    CMD_LEFT,
    CMD_RIGHT,
	  CMD_SPINLEFT,
	  CMD_SPINRIGHT,
	  CMD_TIMIDLEFT,
	  CMD_TIMIDRIGHT,
	  CMD_SPEEDLEFT,
	  CMD_SPEEDRIGHT,
	  CMD_TURNAROUND,
	  CMD_RAM,
    CMD_STOP
  };
  

  
  RemoteCommandBehaviour(char cmd) : LocalMotion(cmd) {
    cmd = CMD_NO_COMMAND;
  }

  bool requestAction() {
//    Serial.println("requestAction executed.");
    if (cmd == CMD_NO_COMMAND) return false;
    return true;
  }

  void abortAction() {
    stop();
  }

  unsigned long executeActionAndInhibit() {
//    Serial.println("executeActionAndInhibit executed.");
    switch (cmd) {
      case CMD_SPEED:
        setSpeed(p1);
        break;
      case CMD_FORWARD:
//        Serial.println("forward executed.");
        forward();
        break;
      case CMD_BACKWARD:
        reverse();
        break;
      case CMD_LEFT:
        turnLeft();
        break;
      case CMD_RIGHT:
        turnRight();
        break;
      case CMD_SPINLEFT:
        spinLeft();
        break;
      case CMD_SPINRIGHT:
        spinRight();
        break;
      case CMD_TIMIDLEFT:
        timidLeft();
        break;
      case CMD_TIMIDRIGHT:
        timidRight();
        break;
      case CMD_SPEEDLEFT:
        speedLeft();
        break;
      case CMD_SPEEDRIGHT:
        speedRight();
        break;
      case CMD_TURNAROUND:
        turnAround();
        break;
	  case CMD_RAM:
        ram();
        break;
      case CMD_STOP:
        stop();
        break;
      default:
        break;
    }
    cmd = CMD_NO_COMMAND;
    return 0;
  }

  void setCommand(COMMANDS cmd, int p1, int p2) {
//    Serial.println("setCommand executed.");
    this->cmd = cmd;
    this->p1 = p1;
    this->p2 = p2;
  }
  
  bool isCommandPending() {
	  return (cmd != CMD_NO_COMMAND);
  }

private:
  int p1;
  int p2;
  COMMANDS cmd;

};

#endif
