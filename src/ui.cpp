#include <ESPUI.h>
#include "control.h"

int statusLabelId;
int panel1;

void openButtonCallback(Control *sender, int type)
{
    ESPUI.print(statusLabelId, "Opening...");
    openShutter();
    ESPUI.print(statusLabelId, "Open");
}

void stopButtonCallback(Control *sender, int type)
{
    ESPUI.print(statusLabelId, "Stopped!");
    abortSlew();
}

void closeButtonCallback(Control *sender, int type)
{
    ESPUI.print(statusLabelId, "Closing...");
    closeShutter();
    ESPUI.print(statusLabelId, "Closed");
}

void setupUI()
{
    auto ctrlTab = ESPUI.addControl(Tab, "", "Control");
    auto setupTab = ESPUI.addControl(Tab, "", "Setup");

    statusLabelId = ESPUI.addControl(ControlType::Label, "Status", "Status", ControlColor::None, ctrlTab);

    panel1 = ESPUI.addControl(ControlType::Button, "Control", "Open", ControlColor::Turquoise, ctrlTab, openButtonCallback);
    ESPUI.addControl(ControlType::Button, "", "Stop", ControlColor::Carrot, panel1, stopButtonCallback);
    ESPUI.addControl(ControlType::Button, "", "Close", ControlColor::None, panel1, closeButtonCallback);

    ESPUI.begin("Roll off Roof Controller");
}