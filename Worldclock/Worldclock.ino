#include "Worldclock.h"

Worldclock watchy;

void setup() {
    Serial.begin(115200);
    watchy.init();
}

void loop(){}