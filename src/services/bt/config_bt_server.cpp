#include "config_bt_server.h"
#include "time_characteristic.h"

#define SERVICE_UUID "86b12865-4b70-4893-8ce6-9864fc00374d"
#define TIME_CHAR_UUID "38c15f3d-7b83-42b1-9275-0b10ea4baeaf"

RTC_DATA_ATTR int ble_status = -1;
RTC_DATA_ATTR int IN_BT_SETUP = -1;

class BLECallBacks: public BLEServerCallbacks {
    void onConnect(BLEServer* pServer) {
      log_d("Connected BLE");
      ble_status = 0;
    };

    void onDisconnect(BLEServer* pServer) {
      log_d("Disconnected BLE");
      ble_status = 1;
    }
};

ConfigBtServer::ConfigBtServer(EDisplay disp): display(disp){};

void ConfigBtServer::drawConnectedNotification(String status) {
    display.init(0,false);
    display.setFullWindow();
    display.fillScreen(GxEPD_WHITE);
    display.setTextColor(GxEPD_BLACK);
    display.setCursor(30,100);
    display.setFont(&FreeMonoBold9pt7b);
    display.print(status);
    display.display(true);
}

void ConfigBtServer::init() {
    BLEDevice::init("Fetty BLE");
    IN_BT_SETUP = 1;
    ble_status = -1;
    bleServer = BLEDevice::createServer();
    bleServer->setCallbacks(new BLECallBacks());

    bleService = bleServer->createService(SERVICE_UUID);

    BLETimeCharacteristic* timeChar = new BLETimeCharacteristic( BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE);
    timeChar->init();
    bleService->addCharacteristic(timeChar); 

    bleService->start();

    bleServer->getAdvertising()->addServiceUUID(SERVICE_UUID);
    bleServer->getAdvertising()->start();

    int prevStatus = -1;
    int currentStatus;
    while(1) {
        currentStatus = ble_status;
        int backButtonPressed = digitalRead(BACK_BTN_PIN);
        if(backButtonPressed) {
            log_d("back button pressed");
            IN_BT_SETUP = -1;
            break;
        }
        if(prevStatus != currentStatus || prevStatus == 1) {
            if(ble_status == 0) {
                log_d("Connected");
                drawConnectedNotification("BT Connected");
        } else if (ble_status == 1) {
                log_d("Disconnected exiting");
                IN_BT_SETUP = -1;
                drawConnectedNotification("BT Disconnected");
                break;
            } 
        }
        prevStatus = currentStatus; 
    }
}

char* ConfigBtServer::parseDateTime() {
    Watchy::RTC.config("");
    tmElements_t currentTime;
    Watchy::RTC.read(currentTime);
    char* t = (char*) malloc(100);
    char hour[10];
    char min[10];
    if(currentTime.Hour < 10) {
        sprintf(hour, "0%hd", currentTime.Hour);
    } else {
        sprintf(hour, "%hd", currentTime.Hour);
    }
    if(currentTime.Minute < 10) {
        sprintf(min, "0%hd", currentTime.Minute);
    } else {
        sprintf(min, "%hd", currentTime.Minute);
    }
    snprintf(t, 100, "%hd:%hd:%hd:%s:%s", currentTime.Year + 2000 ,currentTime.Month, currentTime.Day,hour, min);
    return t;
}