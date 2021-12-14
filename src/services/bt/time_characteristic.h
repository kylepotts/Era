#include "BLE.h"

#define TIME_CHAR_UUID "38c15f3d-7b83-42b1-9275-0b10ea4baeaf"

class BLETimeCharacteristicCallbacks : public BLECharacteristicCallbacks {
    public:
        void onWrite(BLECharacteristic* c);
};

class BLETimeCharacteristic : public BLECharacteristic {
    using BLECharacteristic::BLECharacteristic;
    public:
        BLETimeCharacteristic(uint32_t properties);
        void setTimeStr(std::string value);
        void init();
        char* parseDateTime();
        std::string getTimeStr();
};