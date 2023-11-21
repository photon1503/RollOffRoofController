#include <ArduinoJson.h>
#include <WebServer.h>
#include "AsyncUDP.h"
#include "control.h"

const char *version = "1.0.0.0";
const char *name = "RolloffRoofController";

unsigned const int localPort = 32227; // The Alpaca Discovery test port
unsigned const int alpacaPort = 4567; // The (fake) port that the Alpaca API would be available on

String apiBase = "/api/v1/dome/0/";

WebServer server(alpacaPort);

AsyncUDP udp;

DynamicJsonDocument doc(2048);
char buffer[2048];

void response400()
{
    server.send(400, "application/json");
}

void respondNotImplemented()
{
    doc.clear();
    doc["ErrorNumber"] = 1024;
    doc["ErrorMessage"] = "Not implemented";

    serializeJson(doc, buffer);

    server.send(200, "application/json", buffer);
}

void responseCommon()
{
    doc["ErrorNumber"] = 0;
    doc["ErrorMessage"] = "";

    serializeJson(doc, buffer);

    server.send(200, "application/json", buffer);
}

void response()
{
    doc.clear();
    responseCommon();
}

void responseEmpty()
{
    response();
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
    server.on(apiBase + "slaved", HTTP_GET, respondFalse);
    server.on(apiBase + "slaved", HTTP_PUT, respondNotImplemented);
    server.on(apiBase + "azimuth", respondNaN);
    server.on(apiBase + "altitude", respondNaN);
    server.on(apiBase + "park", respondNotImplemented);
    server.on(apiBase + "slewtoaltitude", respondNotImplemented);
    server.on(apiBase + "slewtoazimuth", respondNotImplemented);
    server.on(apiBase + "synctoazimuth", respondNotImplemented);
    server.on(apiBase + "findhome", respondNotImplemented);
    server.on(apiBase + "setpark", respondNotImplemented);

    // start server
    server.begin();
}

void autodiscovery()
{
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
void handle()
{
    server.handleClient();
}