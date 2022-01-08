#include "time_characteristic.h"
#include "Watchy.h"

void BLETimeCharacteristicCallbacks::onWrite(BLECharacteristic* c) {
    log_d("Read String %s", (c->getValue()).c_str());
    Watchy::RTC.config((c->getValue()).c_str());
};

BLETimeCharacteristic::BLETimeCharacteristic(uint32_t properties) : BLECharacteristic(TIME_CHAR_UUID, properties) {};

void BLETimeCharacteristic::setTimeStr(std::string value) {
    setValue(value);
};

std::string BLETimeCharacteristic::getTimeStr() {
    return getValue();
}

void BLETimeCharacteristic::init() {
    this->setTimeStr(parseDateTime());
    this->setCallbacks(new BLETimeCharacteristicCallbacks());
};

char* BLETimeCharacteristic::parseDateTime() {
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
    snprintf(t, 100, "%hd:%hd:%hd:%s:%s", tmYearToY2k(currentTime.Year) + 2000 ,currentTime.Month, currentTime.Day,hour, min);
    return t;
};