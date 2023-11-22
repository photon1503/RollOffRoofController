#include <ESPUI.h>
#include "control.h"
#include "prefs.h"

int statusLabelId;
int panel1;
uint16_t UIalpacaPort, UIdeviceID;

void openButtonCallback(Control *sender, int type)
{
    if (type == B_DOWN)
    {
        Serial.println("openButtonCallback");
        openShutter();
    }
}

void stopButtonCallback(Control *sender, int type)
{
    if (type == B_DOWN)
    {
        Serial.println("stopButtonCallback");
        abortSlew();
    }
}

void closeButtonCallback(Control *sender, int type)
{
    if (type == B_DOWN)
    {
        Serial.println("closeButtonCallback");
        closeShutter();
    }
}

void updateText(String status)
{
    ESPUI.updateText(statusLabelId, status);
}

//  0 = Open, 1 = Closed, 2 = Opening, 3 = Closing, 4 = Shutter status error
String getRoofStatus()
{
    switch (_shutterStatus)
    {
    case 0:
        return "Open";
        break;
    case 1:
        return "Closed";
        break;
    case 2:
        return "Opening ...";
        break;
    case 3:
        return "Closing ...";
        break;
    case 4:
        return "Shutter status error";
        break;
    default:
        return "Unkown error!";
    }
}

void updateRoofStatus()
{
    ESPUI.updateText(statusLabelId, getRoofStatus());
}

void callbackAlpacaPort(Control *sender, int type)
{

    String s = sender->value;
    setAlpacaPort(s.toInt());
}

void callbackDeviceID(Control *sender, int type)
{

    String s = sender->value;
    setDeviceID(s.toInt());
}

void rebootButtonCallback(Control *sender, int type)
{
    ESP.restart();
}

void setupUIControl()
{
    auto ctrlTab = ESPUI.addControl(Tab, "", "Control");

    statusLabelId = ESPUI.addControl(ControlType::Label, "Status", "Status", ControlColor::None, ctrlTab);

    panel1 = ESPUI.addControl(ControlType::Button, "Control", "Open", ControlColor::Turquoise, ctrlTab, openButtonCallback);
    ESPUI.addControl(ControlType::Button, "", "Stop", ControlColor::Carrot, panel1, stopButtonCallback);
    ESPUI.addControl(ControlType::Button, "", "Close", ControlColor::None, panel1, closeButtonCallback);
}

void setupUISetup()
{
    auto setupTab = ESPUI.addControl(Tab, "", "Setup");
    UIalpacaPort = ESPUI.addControl(ControlType::Number, "Alpaca Port", String(getAlpacaPort()), ControlColor::None, setupTab, callbackAlpacaPort);
    UIdeviceID = ESPUI.addControl(ControlType::Number, "Device ID", String(getDeviceID()), ControlColor::None, setupTab, callbackDeviceID);

    ESPUI.addControl(ControlType::Button, "", "Reboot", ControlColor::Carrot, setupTab, rebootButtonCallback);
}

void setupUI()
{
    setupUIControl();
    setupUISetup();

    ESPUI.begin("Roll off Roof Controller");
}