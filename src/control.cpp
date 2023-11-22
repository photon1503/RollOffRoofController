#include "alpaca.h"
#include "ui.h"

bool isShutterOpen = false;
bool isSlewing = false;
int8_t _shutterStatus = 4;

void PushRelay()
{
  digitalWrite(16, LOW);
  delay(500);
  digitalWrite(16, HIGH);
}

//  0 = Open, 1 = Closed, 2 = Opening, 3 = Closing, 4 = Shutter status error
void shutterStatus()
{
  response(_shutterStatus);
}

void slewing()
{
  response(isSlewing);
}

void closeShutter()
{
  Serial.println("closeShutter");
  isSlewing = true;
  _shutterStatus = 3;
  updateRoofStatus();
  // TODO Action
  PushRelay();
  delay(2000);
  isSlewing = false;
  isShutterOpen = false;
  response();
  _shutterStatus = 1;
  updateRoofStatus();
}

void openShutter()
{
  Serial.println("openShutter");
  _shutterStatus = 2;
  updateRoofStatus();
  isSlewing = true;
  // TODO Action
  PushRelay();
  delay(2000);
  isSlewing = false;
  isShutterOpen = true;
  response();
  _shutterStatus = 0;
  updateRoofStatus();
}

void abortSlew()
{
  Serial.println("abortSlew");
  // TODO Action
  PushRelay();
  isSlewing = false;
  response();
  updateText("Stopped!");
}
