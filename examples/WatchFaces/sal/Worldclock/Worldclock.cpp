#include <string>
#include "Worldclock.h"

Worldclock::Worldclock() {

}

// display is 200x200
// cursor starts at top left
// 20pt font has 20px height
void Worldclock::drawWatchFace() {
    display.setFont(&GoogleSans_Regular20pt7b);

    display.setCursor(10, 20);
    display.print("ZRH");
    
    int16_t x_boundary, y_boundary;
    uint16_t width, height;
    display.getTextBounds("ZRH", display.getCursorX(), display.getCursorY(), &x_boundary, &y_boundary, &width, &height);

    display.setCursor(0, 180);
    display.print(x_boundary, 10);

    display.setCursor(0, 160);
    display.print(y_boundary, 10);

    display.setCursor(0, 140);
    display.print(width, 10);

    display.setCursor(0, 120);
    display.print(height, 10);

    display.setCursor(100, 100);
    display.print(buildTime().c_str());
}

std::string Worldclock::buildTime() {
    std::string time = "";
    if (currentTime.Hour < 10) {
        time += "0";
    }
    time += currentTime.Hour;
    time += ":";
    if (currentTime.Minute < 10){
        time += "0";
    }  
    time += currentTime.Minute;
    return time;   
}