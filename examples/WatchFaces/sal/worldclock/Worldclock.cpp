#include "Worldclock.h"

Worldclock::Worldclock() {

}

void Worldclock::drawWatchFace() {
    display.setCursor(25, 110);
    if(currentTime.Hour < 10){ //use the currentTime struct to print latest time
        display.print("0");
    }
    display.print(currentTime.Hour);
    display.print(":");
    if(currentTime.Minute < 10){
        display.print("0");
    }  
    display.println(currentTime.Minute);   
}