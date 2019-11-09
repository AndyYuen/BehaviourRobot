#ifndef SendTelemetryBehaviour_h
#define SendTelemetryBehaviour_h

#include <Zumo32U4.h>
#include "LocalMotion.h"
//#include "TurnSensor.h"

// only send telemetry every 100 milliseconds
#define PERIOD 2000

// Behaviour for sending the left and right sensor values over the serial port:
// - left followed by right values 0..255
// This behaviour should be set up as the higestst priority behaviour
class SendTelemetryBehaviour: public LocalMotion
{
public:
	// Sensor values are sent over Serial1
	// This behviour never return true in requestAction ie, it never takes control




	SendTelemetryBehaviour(Zumo32U4ProximitySensors* proxSensors, char cmd) : LocalMotion(cmd) {
    //this->turnSensor = TurnSensor::getInstance();
		this->proxSensors = proxSensors;
    now = timeToSend = millis();
	}

	bool requestAction() {
    sendTelemetry();
		return false;
	}

	void abortAction() {
		stop();
	}

	unsigned long executeActionAndInhibit() {
		return 0;
	}

  uint8_t getLeftValue() {
    return leftValue;  
  }
    
  uint8_t getRightValue() {
    return rightValue;  
  }

private:
	Zumo32U4ProximitySensors* proxSensors;
  //TurnSensor *turnSensor;
	uint8_t leftValue = 0;
	uint8_t rightValue = 0;
  //char buffer[200];
  unsigned long now , timeToSend;


  // only send telemetry if it itime to send (every PERIOD millisonds)
  void sendTelemetry() {
    //char buffer[64];
    now = millis();
    if (timeToSend <= now) {
      proxSensors->read();
      leftValue = proxSensors->countsFrontWithLeftLeds();
      rightValue = proxSensors->countsFrontWithRightLeds();
      timeToSend = now + PERIOD;

      //sprintf(buffer, "%05d %05d %05d %06.2f \n", turnSensor->x(), turnSensor->y(), turnSensor->z(), turnSensor->getHeading());
      //Serial.print(buffer);
      //Serial.println(turnSensor->getHeading());
    }
  }

};

#endif
