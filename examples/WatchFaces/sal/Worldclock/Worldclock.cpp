#include <string>
#include "Worldclock.h"

Worldclock::Worldclock() {
    x_offset = 30;
    y_offset = 30;
}

// display is 200x200
// cursor starts at top left
// 20pt font has 20px height
void Worldclock::drawWatchFace() {
    display.setFont(&FreeMono12pt7b);
    display.fillScreen(GxEPD_WHITE);
    display.setTextColor(GxEPD_BLACK);
    display.setTextWrap(false);
    
    String txt = "ZRH " + buildTime(0);
    writeLine(txt);

    txt = "NYC " + buildTime(-6);
    writeLine(txt);

    txt = "MTV " + buildTime(-9);
    writeLine(txt);
}

void Worldclock::writeLine(String txt) {
    int16_t x_boundary, y_boundary;
    uint16_t width, height;
    display.getTextBounds(txt, 
        display.getCursorX(), display.getCursorY(), 
        &x_boundary, &y_boundary, 
        &width, &height);

    display.setCursor(this->x_offset, this->y_offset + height);
    display.print(txt);
    // move offset to next line
    this->y_offset += height + 5;
}

String Worldclock::buildTime(int offset) {
    String time = "";
    if (currentTime.Hour < 10) {
        time += "0";
    }
    time += currentTime.Hour + offset;
    time += ":";
    if (currentTime.Minute < 10){
        time += "0";
    }  
    time += currentTime.Minute;
    return time;   
}