#include "Era.h"
#include "settings.h"

Era watchy(settings);

void setup(){
    Serial.begin(115200);
    watchy.init();
}

void loop(){}