#include <Arduino.h>

extern int8_t _shutterStatus;

void shutterStatus();
void slewing();
void closeShutter();
void openShutter();
void abortSlew();
