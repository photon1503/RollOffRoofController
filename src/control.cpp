#include "alpaca.h"

bool isShutterOpen = false;
bool isSlewing = false;

void shutterStatus()
{
  if (isShutterOpen)
    response(0);
  if (!isShutterOpen)
    response(1);
  // TODO: Opening, Closing, Error
}
void slewing()
{
  response(isSlewing);
}

void closeShutter()
{

  isSlewing = true;
  // TODO Action
  //delay(2000);
  isSlewing = false;
  isShutterOpen = false;
  response();
}

void openShutter()
{

  isSlewing = true;
  // TODO Action
  //delay(2000);
  isSlewing = false;
  isShutterOpen = true;
  response();
}

void abortSlew()
{
  // TODO Action
  isSlewing = false;
  response();
}
