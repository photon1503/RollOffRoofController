#include <Arduino.h>
#include <WebServer.h>

/* 
extern unsigned const int localPort;
extern unsigned const int alpacaPort;

extern WebServer server();
 */
void responseCommon();
void response();
void response(char *value);
void response(boolean value);
void response(int32_t value);
void getName();
void getDescription();
void getDriverInfo();
void getDriverVersion();
void getInterfaceVersion();
void respondTrue();
void respondFalse();
void respondNaN();
void getsupportedactions();
void apiversions();
void mgmtDescription();
void mgmtConfigureddevices();
void setup_routing();
void handle();
void autodiscovery();
