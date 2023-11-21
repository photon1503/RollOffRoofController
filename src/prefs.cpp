#include <Preferences.h>
Preferences preferences;

int getAlpacaPort()
{
    int p = preferences.getInt("AlpacaPort", 4568);
    Serial.print("Get AlpacaPort: ");
    Serial.println(p);

    return p;
}

void setAlpacaPort(int port)
{
    preferences.putInt("AlpacaPort", port);
    Serial.print("Set AlpacaPort: ");
    Serial.println(port);
}

int getDeviceID()
{
    int p = preferences.getInt("DeviceID", 0);
    Serial.print("Get DeviceID: ");
    Serial.println(p);

    return p;
}

void setDeviceID(int id)
{
    preferences.putInt("DeviceID", id);
    Serial.print("Set DeviceID: ");
    Serial.println(id);
}

void startupPreferences()
{
    preferences.begin("my-app", false);
    Serial.println("Preferences initialized");
}
