#include <Arduino.h>
#include <Update.h>
#include <WebServer.h>
#include <DNSServer.h>
#include <ArduinoJson.h>
// #include "ESPAsyncUDP.h"
#include "AsyncUDP.h"
#include <WiFi.h>

const char *SSID = "x3";
const char *PWD = "apfe1komp0tt";

const char *version = "1.0.0.0";
const char *name = "RolloffRoofController";

String apiBase = "/api/v1/dome/0/";
unsigned const int localPort = 32227; // The Alpaca Discovery test port
unsigned const int alpacaPort = 4567; // The (fake) port that the Alpaca API would be available on

bool isShutterOpen = false;
bool isSlewing = false;

AsyncUDP udp;
WebServer server(alpacaPort);

// JSON data buffer
// StaticJsonDocument<250> jsonDocument;
DynamicJsonDocument doc(2048);
char buffer[2048];

void responseCommon()
{
  doc["ErrorNumber"] = 0;
  doc["ErrorMessage"] = "";

  serializeJson(doc, buffer);

  server.send(200, "application/json", buffer);
}

void response(char *value)
{
  doc.clear();
  doc["Value"] = value;
  responseCommon();
}

void response(boolean value)
{
  doc.clear();
  doc["Value"] = value;
  responseCommon();
}

void response(int32_t value)
{
  doc.clear();
  doc["Value"] = value;
  responseCommon();
}

void getName()
{
  Serial.println("Get name");
  response("RolloffRoofController");
}

void getDescription()
{
  response("RolloffRoofController");
}

void getDriverInfo()
{
  response("RolloffRoofController");
}

void getDriverVersion()
{
  response("1.0.0.0");
}

void getInterfaceVersion()
{
  response(1);
}

void respondTrue()
{
  response(true);
}
void respondFalse()
{
  response(false);
}

void respondNaN()
{
  response(0);
}

void getsupportedactions()
{
  doc.clear();

  doc["Value"] = doc.createNestedArray("");
  responseCommon();
}

void apiversions()
{
  doc.clear();

  JsonArray array = doc.createNestedArray("Value");
  array.add(1);

  responseCommon();
}

void mgmtDescription()
{
  doc.clear();

  JsonObject Value = doc.createNestedObject("Value");
  Value["ServerName"] = name;
  Value["Manufacturer"] = "Photon";
  Value["ManufacturerVersion"] = version;
  Value["Location"] = "n/a";

  responseCommon();
}

void mgmtConfigureddevices()
{
  doc.clear();
  
  JsonArray array = doc.createNestedArray("Value");
  JsonObject Value = array.createNestedObject();
  Value["DeviceName"] = name;
  Value["DeviceType"] = "Dome";
  Value["DeviceNumber"] = 0;
  Value["UniqueID"] = "3b1ecc17-38b0-4179-9e04-09dd07a285eb";

  responseCommon();
}

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
  // TODO Action
  isShutterOpen = false;
}

void openShutter()
{
  // TODO Action
  isShutterOpen = true;
}

void abortSlew()
{
  // TODO Action
  isSlewing = false;
}

void setup_routing()
{
  server.on("/management/apiversions", apiversions);
  server.on("/management/v1/description", mgmtDescription);
  server.on("/management/v1/configureddevices", mgmtConfigureddevices);

  server.on(apiBase + "name", getName);
  server.on(apiBase + "connected", respondTrue);
  server.on(apiBase + "description", getDescription);
  server.on(apiBase + "driverinfo", getDriverInfo);
  server.on(apiBase + "driverversion", getDriverVersion);
  server.on(apiBase + "interfaceversion", getInterfaceVersion);
  server.on(apiBase + "cansetshutter", respondTrue);
  server.on(apiBase + "cansetpark", respondFalse);
  server.on(apiBase + "canslave", respondFalse);
  server.on(apiBase + "canpark", respondFalse);
  server.on(apiBase + "canfindhome", respondFalse);
  server.on(apiBase + "cansetaltitude", respondFalse);
  server.on(apiBase + "cansetfindhome", respondFalse);
  server.on(apiBase + "cansetazimuth", respondFalse);
  server.on(apiBase + "cansyncazimuth", respondFalse);

  server.on(apiBase + "supportedactions", getsupportedactions);

  server.on(apiBase + "slewing", slewing);
  server.on(apiBase + "shutterstatus", shutterStatus);
  server.on(apiBase + "closeshutter", closeShutter);
  server.on(apiBase + "openshutter", openShutter);
  server.on(apiBase + "abortslew", abortSlew);

  server.on(apiBase + "atpark", respondFalse);
  server.on(apiBase + "athome", respondFalse);
  server.on(apiBase + "slaved", respondFalse);
  server.on(apiBase + "azimuth", respondNaN);
  



  // start server
  server.begin();
}

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
  connectToWiFi();

  setup_routing();
  
  if (udp.listen(localPort))
  {
    Serial.println("Listening for discovery requests...");
    udp.onPacket([](AsyncUDPPacket packet)
                 {
            Serial.print("Received UDP Packet of Type: ");
            Serial.print(packet.isBroadcast() ? "Broadcast" : packet.isMulticast() ? "Multicast" : "Unicast");
            Serial.print(", From: ");
            Serial.print(packet.remoteIP());
            Serial.print(":");
            Serial.print(packet.remotePort());
            Serial.print(", To: ");
            Serial.print(packet.localIP());
            Serial.print(":");
            Serial.print(packet.localPort());
            Serial.print(", Length: ");
            Serial.print(packet.length());
            Serial.print(", Data: ");
            Serial.write(packet.data(), packet.length());
            Serial.println();

            // No undersized packets allowed
            if (packet.length() < 16)
            {
                return;
            }

            //Compare packet to Alpaca Discovery string
            if (strncmp("alpacadiscovery1", (char *)packet.data(), 16) != 0)
            {
                return;
            }

            // send a reply, to the IP address and port that sent us the packet we received
            // on a real system this would be the port the Alpaca API was on
            packet.printf("{\"AlpacaPort\": %d}", alpacaPort); });
  }

 
}

void loop()
{
  server.handleClient();
}
