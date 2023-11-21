#include <Arduino.h>
#include <Update.h>
#include <WebServer.h>
#include <ESPUI.h>
#include <DNSServer.h>
#include <WiFi.h>
#include "ui.h"
#include "control.h"
#include "prefs.h"
#include "alpaca.h"
#include "secrets.h"


const char *SSID = "x3";
const char *PWD = pass;


void connectToWiFi()
{
  Serial.print("Connecting to ");
  Serial.println(SSID);

  WiFi.begin(SSID, PWD);

  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(500);
    // we can even make the ESP32 to sleep
  }

  Serial.print("Connected. IP: ");
  Serial.println(WiFi.localIP());
}

void setup()
{
  Serial.begin(9600);
  delay(250);
  startupPreferences();
  connectToWiFi();
  setup_routing();
  setupUI();
  autodiscovery(); 
}

void loop()
{
  handle();
}
