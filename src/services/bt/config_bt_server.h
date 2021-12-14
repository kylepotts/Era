#include "Watchy.h"

typedef GxEPD2_BW<GxEPD2_154_D67, GxEPD2_154_D67::HEIGHT> EDisplay;
class ConfigBtServer {
    public:
        BLEServer* bleServer = NULL;
        BLEService* bleService = NULL;
        BLECharacteristic* timeCharacteristic = NULL;
        EDisplay display;
        ConfigBtServer(EDisplay disp);
        void drawConnectedNotification(String status);
        void init();
        char* parseDateTime();
};

extern RTC_DATA_ATTR int ble_status;
extern RTC_DATA_ATTR int IN_BT_SETUP;