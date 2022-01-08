#include "Era.h"

void Era::init(String datetime){
    esp_sleep_wakeup_cause_t wakeup_reason;
    wakeup_reason = esp_sleep_get_wakeup_cause(); //get wake up reason
    Wire.begin(SDA, SCL); //init i2c
    Watchy::RTC.init();

    configBtServer = new ConfigBtServer(display);

    display.init(0, displayFullInit, 10, true); // 10ms by spec, and fast pulldown reset
    display.epd2.setBusyCallback(displayBusyCallback);
    
    switch (wakeup_reason)
    {
        case ESP_SLEEP_WAKEUP_EXT0: //RTC Alarm
            if(guiState == WATCHFACE_STATE){
                if(IN_BT_SETUP != 1) {
                  RTC.read(currentTime);
                  showWatchFace(true); //partial updates on tick
                }
            }
            break;
        case ESP_SLEEP_WAKEUP_EXT1: //button Press
            handleButtonPress();
            break;
        default: //reset
            Watchy::RTC.config(datetime);
            //_bmaConfig();
            Watchy::RTC.read(currentTime);
            showWatchFace(false); //full update on reset
            break;
    }
    deepSleep();
}

void Era::handleButtonPress() {
  uint64_t wakeupBit = esp_sleep_get_ext1_wakeup_status();
  if(IN_BT_SETUP == 1) {
    if (wakeupBit & BACK_BTN_MASK){
      btStop();
     showWatchFace(true);
    }
  } else {
    if(wakeupBit & MENU_BTN_MASK) {
        showMenu('0', true);
        configBtServer->init();
        btStop();
        Watchy::RTC.config("");
        Watchy::RTC.read(currentTime);
        Watchy::RTC.clearAlarm();
        showWatchFace(false);
    }
  }
}

void Era::showMenu(byte menuIndex, bool partialRefresh) {
    display.init(0,false);
    display.setFullWindow();
    display.fillScreen(GxEPD_WHITE);
    display.setTextColor(GxEPD_BLACK);
    display.setCursor(30,100);
    display.setFont(&FreeMonoBold9pt7b);
    display.print("Waiting for BT connection");
    display.println(" ");
    display.println("Press back to exit");
    display.display(true);

    guiState = MAIN_MENU_STATE; 
}

void Era::drawWatchFace(){
    display.fillScreen(GxEPD_WHITE);
    display.setTextColor(GxEPD_BLACK);
    display.setFont(&DSEG7_Classic_Bold_53);
    display.setCursor(0, 53+60);
    String ampm = currentTime.Hour < 12 ? " AM": " PM";
    if(currentTime.Hour < 10 ){
        display.print("0");
    } else if( currentTime.Hour > 12 && (currentTime.Hour - 12) < 10) {
      display.print("0");
    }
    display.print(currentTime.Hour > 12 ? currentTime.Hour - 12 : currentTime.Hour);
    display.print(":");
    if(currentTime.Minute < 10){
        display.print("0");
    }  
    display.print(currentTime.Minute);
    display.setFont(&FreeMonoBold9pt7b);
    display.setCursor(160, 140);
    display.println(ampm); 

    display.setCursor(20, 160);
    
    display.setFont(&FreeMonoBold9pt7b);
    display.print(tmYearToY2k(currentTime.Year) + 2000);
    display.print("/");
    display.print(currentTime.Month);
    display.print("/");
    display.print(currentTime.Day);

    char battery[100];
    sprintf(battery, "Battery %.2f V", getBatteryVoltage());

    display.setCursor(20, 180);
    display.println(battery);
}