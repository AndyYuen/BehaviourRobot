#include <Wire.h>
#include <Zumo32U4.h>
#include <SerialCommand.h>
#include <Queue.h>

#include "BehaviourBasedRobot.h"
#include "RemoteCommandBehaviour.h"
#include "AvoidObstacleBehaviour.h"
#include "AvoidCliffBehaviour.h"
#include "SendTelemetryBehaviour.h"
#include "SpinBehaviour.h"
#include "CruiseBehaviour.h"
#include "TurnSensor.h"

// uncomment #define below to use the Serial Monitor to issue serial commands for debugging
//#define _DEBUG 1
//#define _RHDM
#define SPEED 19200
#define PERIOD 500

//#define _DISCARD_COMMANDS

BehaviourBasedRobot robot;
RemoteCommandBehaviour *remoteCmd;
SendTelemetryBehaviour *telemetry;
SerialCommand sCmd;
unsigned long now , timeToSend = millis();
char buffer[200];
DataQueue<RemoteCommandBehaviour::COMMANDS> cmdQueue;
DataQueue<int> speedQueue;

Zumo32U4ProximitySensors proxSensors;
Zumo32U4LineSensors lineSensors;



void setup() {
  
//Wire.begin();

#ifdef _DEBUG
  // use it for debugging only
  Serial.begin(SPEED);
#else
  // use it for serial communication
  Serial1.begin(SPEED);
#endif

// initialise compass
TurnSensor::getInstance();

  // setup robot behaviours
#ifdef _RHDM
  telemetry = new SendTelemetryBehaviour(&proxSensors, 't');
  robot.addBehaviour(telemetry);
#endif
  //***************add this for demo, commented out for testing only*********************************************
  // behaviour #1
  //robot.addBehaviour(new AvoidCliffBehaviour(&lineSensors, 'c'));

  // behaviour #2

//#ifndef _RHDM
//  robot.addBehaviour(new AvoidObstacleBehaviour(&proxSensors, 'a'));
//#endif
  // behaviour #3
  remoteCmd = new RemoteCommandBehaviour('r');
  robot.addBehaviour(remoteCmd);

//  robot.addBehaviour(new SpinBehaviour('s'));
//  robot.addBehaviour(new CruiseBehaviour('z'));

  lineSensors.initThreeSensors();
  proxSensors.initFrontSensor();


  // Setup handlers for SerialCommands (case-sensitive)
  sCmd.addCommand("speed", speed);        // set speed
  sCmd.addCommand("stop", stop);          // stop
  sCmd.addCommand("forward", forward);    // move forward
  sCmd.addCommand("ram", ram);            // ram
  sCmd.addCommand("backward", backward);  // move backward
  sCmd.addCommand("left", left);          // turn left
  sCmd.addCommand("right", right);        // turn right
  sCmd.addCommand("spinLeft", spinLeft);  // spin left
  sCmd.addCommand("spinRight", spinRight);// spin right
  sCmd.addCommand("timidLeft", timidLeft);// timid left
  sCmd.addCommand("timidRight", timidRight);// timid right
  sCmd.addCommand("speedLeft", speedLeft);// speed left
  sCmd.addCommand("speedRight", speedRight);// speed right
  sCmd.addCommand("turnAround", turnAround);// turn around
//  sCmd.addCommand("disable", disable);    // disable behaviour
//  sCmd.addCommand("enable", enable);      // enable behaviour
  sCmd.setDefaultHandler(unrecognized);   // Handler for command that isn't matched

  //  Serial.println("Ready to accept commands...");
  // let robot stop for 5 seconds before starting
  delay(300);
}

void loop() {
  // process serial commands
  // - Use Serial for debugging using Serial Monitor
  // - when connected to TX/RX pins, use Serial1 instead
#ifdef _DEBUG
  sCmd.readSerial(Serial);
#else
  sCmd.readSerial(Serial1);
#endif

#ifdef _DISCARD_COMMANDS

  while (!cmdQueue.isEmpty()) {
    cmdQueue.dequeue();
  }
  while (!speedQueue.isEmpty()) {
    speedQueue.dequeue();
  }

#else
  if (!remoteCmd->isCommandPending()) {
    if (!cmdQueue.isEmpty()) {
      ExecuteNextCommand();
    }
  }
 #endif

  // let robot do its stuff
  robot.run();
  
#ifdef _RHDM  
  // send telemetry to Esp32 here
  now = millis();
  if (timeToSend <= now) {
    sprintf(buffer, "{ \"leftObstacle\": %u, \"rightObstacle\": %u};", telemetry->getLeftValue(), telemetry->getRightValue());
    Serial1.print(buffer);
    //Serial1.print("Hello there;");
    timeToSend = now + PERIOD;
  }
#endif
  
}
// ********************************************************************************
// *
// * The following procedures handle commands received from the serial port
// *
// ********************************************************************************
int getInt() {
  int aNumber;
  char *arg;
  arg = sCmd.next();
  if (arg != NULL) {
    aNumber = atoi(arg);    // Converts a char string to an integer
  }
  else {
    aNumber = 0;
  }

  return aNumber;
}

// This gets set as the default handler, and gets called when no other command matches.
void unrecognized(const char *command) {
  // ignore it, do not send it over serial unless debugging
#ifdef _DEBUG
  Serial.print("Unrecognised command: "); Serial.println(command);
#endif
}

// serial commands are passed on to RemoteCommandBehaviour and activated during robot.arbitrate();
void speed() {
  int percent = getInt();
  if (percent < 0) {
    percent = 0;
  }
  else if (percent > 100) {
    percent = 100;
  }

  //absolute speed is between 0 and 400 hence times 4
  speedQueue.enqueue(percent * 4);
  cmdQueue.enqueue(RemoteCommandBehaviour::COMMANDS::CMD_SPEED);
  //remoteCmd->setCommand(RemoteCommandBehaviour::COMMANDS::CMD_SPEED, percent * 4, 0);
}

void stop() {
  cmdQueue.enqueue(RemoteCommandBehaviour::COMMANDS::CMD_STOP);
  //remoteCmd->setCommand(RemoteCommandBehaviour::COMMANDS::CMD_STOP, 0, 0);
}

void forward() {
  cmdQueue.enqueue(RemoteCommandBehaviour::COMMANDS::CMD_FORWARD);
  //remoteCmd->setCommand(RemoteCommandBehaviour::COMMANDS::CMD_FORWARD, 0, 0);
}

void ram() {
  cmdQueue.enqueue(RemoteCommandBehaviour::COMMANDS::CMD_RAM);
  //remoteCmd->setCommand(RemoteCommandBehaviour::COMMANDS::CMD_RAM, 0, 0);
}

void backward() {
  cmdQueue.enqueue(RemoteCommandBehaviour::COMMANDS::CMD_BACKWARD);
  //remoteCmd->setCommand(RemoteCommandBehaviour::COMMANDS::CMD_BACKWARD, 0, 0);
}

void left() {
  cmdQueue.enqueue(RemoteCommandBehaviour::COMMANDS::CMD_LEFT);
  //remoteCmd->setCommand(RemoteCommandBehaviour::COMMANDS::CMD_LEFT, 0, 0);
}

void right() {
  cmdQueue.enqueue(RemoteCommandBehaviour::COMMANDS::CMD_RIGHT);
  //remoteCmd->setCommand(RemoteCommandBehaviour::COMMANDS::CMD_RIGHT, 0, 0);
}

void spinLeft() {
  cmdQueue.enqueue(RemoteCommandBehaviour::COMMANDS::CMD_SPINLEFT);
  //remoteCmd->setCommand(RemoteCommandBehaviour::COMMANDS::CMD_SPINLEFT, 0, 0);
}

void spinRight() {
  cmdQueue.enqueue(RemoteCommandBehaviour::COMMANDS::CMD_SPINRIGHT);
  //remoteCmd->setCommand(RemoteCommandBehaviour::COMMANDS::CMD_SPINRIGHT, 0, 0);
}

void timidLeft() {
  cmdQueue.enqueue(RemoteCommandBehaviour::COMMANDS::CMD_TIMIDLEFT);
  //remoteCmd->setCommand(RemoteCommandBehaviour::COMMANDS::CMD_TIMIDLEFT, 0, 0);
}

void timidRight() {
  cmdQueue.enqueue(RemoteCommandBehaviour::COMMANDS::CMD_TIMIDRIGHT);
  //remoteCmd->setCommand(RemoteCommandBehaviour::COMMANDS::CMD_TIMIDRIGHT, 0, 0);
}
void speedLeft() {
  cmdQueue.enqueue(RemoteCommandBehaviour::COMMANDS::CMD_SPEEDLEFT);
  //remoteCmd->setCommand(RemoteCommandBehaviour::COMMANDS::CMD_SPEEDLEFT, 0, 0);
}

void speedRight() {
  cmdQueue.enqueue(RemoteCommandBehaviour::COMMANDS::CMD_SPEEDRIGHT);
  //remoteCmd->setCommand(RemoteCommandBehaviour::COMMANDS::CMD_SPEEDRIGHT, 0, 0);
}

void turnAround() {
  cmdQueue.enqueue(RemoteCommandBehaviour::COMMANDS::CMD_TURNAROUND);
  //remoteCmd->setCommand(RemoteCommandBehaviour::COMMANDS::CMD_TURNAROUND, 0, 0);
}
//void disable() {
//  cmdQueue.enqueue(RemoteCommandBehaviour::COMMANDS::CMD_STOP);
//  robot.disableBehaviour(getInt());
//}
//
//void enable() {
//  cmdQueue.enqueue(RemoteCommandBehaviour::COMMANDS::CMD_STOP);
//  robot.enableBehaviour(getInt());
//}

void ExecuteNextCommand() {
  RemoteCommandBehaviour::COMMANDS cmd = cmdQueue.dequeue();
  switch (cmd) {
    case RemoteCommandBehaviour::COMMANDS::CMD_SPEED:
      remoteCmd->setCommand(cmd, speedQueue.dequeue(), 0);
      break;
    default:
      remoteCmd->setCommand(cmd, 0, 0);
      break;
  }
#ifdef _DM
    Serial1.print("Done;");
#endif

}
