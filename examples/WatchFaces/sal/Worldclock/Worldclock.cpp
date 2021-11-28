#include <string>
#include "Worldclock.h"

Worldclock::Worldclock() {
    x_offset = 30;
    y_offset = 30;
}

// display is 200x200
// cursor starts at top left
void Worldclock::drawWatchFace() {
    display.setFont(&FreeMono12pt7b);
    display.fillScreen(GxEPD_WHITE);
    display.setTextColor(GxEPD_BLACK);
    display.setTextWrap(false);
    
    writeLine("ZRH " + buildTime(0));
    writeLine("BOG " + buildTime(-6));
    writeLine("NYC " + buildTime(-6));
    writeLine("MTV " + buildTime(-9));
    
    drawMoonsInCorners(25);
}

// x,y is top left corner
void Worldclock::drawMoonsInCorners(int icon_size) {
    MoonPhase moon_phase = getMoonPhase();
    // top-left
    display.drawBitmap(0, 0, moon_waning_crescent, icon_size, icon_size, moon_phase == WANING_CRESCENT ? GxEPD_BLACK : GxEPD_WHITE, moon_phase == WANING_CRESCENT ? GxEPD_WHITE : GxEPD_BLACK);
    // top-middle                      
    display.drawBitmap(100 - icon_size/2, 0, moon_new, icon_size, icon_size, moon_phase == NEW ? GxEPD_BLACK : GxEPD_WHITE, moon_phase == NEW ? GxEPD_WHITE : GxEPD_BLACK);
    // top-right
    display.drawBitmap(200 - icon_size, 0, moon_waxing_crescent, icon_size, icon_size, moon_phase == WAXING_CRESCENT ? GxEPD_BLACK : GxEPD_WHITE, moon_phase == WAXING_CRESCENT ? GxEPD_WHITE : GxEPD_BLACK);
    // middle-left   
    display.drawBitmap(0, 100 - icon_size/2, moon_third_quarter, icon_size, icon_size, moon_phase == THIRD_QUARTER ? GxEPD_BLACK : GxEPD_WHITE, moon_phase == THIRD_QUARTER ? GxEPD_WHITE : GxEPD_BLACK);
    // middle-right
    display.drawBitmap(200 - icon_size, 100 - icon_size/2, moon_first_quarter, icon_size, icon_size, moon_phase == FIRST_QUARTER ? GxEPD_BLACK : GxEPD_WHITE, moon_phase == FIRST_QUARTER ? GxEPD_WHITE : GxEPD_BLACK);  
    // bottom-left        
    display.drawBitmap(0, 200 - icon_size, moon_waning_gibbous, icon_size, icon_size, moon_phase == WANING_GIBBOUS ? GxEPD_BLACK : GxEPD_WHITE, moon_phase == WANING_GIBBOUS ? GxEPD_WHITE : GxEPD_BLACK);
    // bottom-middle            
    display.drawBitmap(100 - icon_size/2, 200 - icon_size, moon_full, icon_size, icon_size, moon_phase == FULL ? GxEPD_BLACK : GxEPD_WHITE, moon_phase == FULL ? GxEPD_WHITE : GxEPD_BLACK);
    // bottom-right
    display.drawBitmap(200 - icon_size, 200 - icon_size, moon_waxing_gibbous, icon_size, icon_size, moon_phase == WAXING_GIBBOUS ? GxEPD_BLACK : GxEPD_WHITE, moon_phase == WAXING_GIBBOUS ? GxEPD_WHITE : GxEPD_BLACK);
}

/**
 * Approximates current moon phase. Uses the first new moon in 2020 as point of reference and computes the number of days in the current cycle 
 * to know which phase we're currently in. The length of the lunar cycle varies, so using a constant length will yield errors over time, 
 * which is why 2020 is chosen.
 * 
 * A full moon can last for up to three days and can happen during the day. This table uses 3d (or 1% of the cycle) for full and new moons and
 * splits the rest of the cycle in equal parts (is this correct? Keplers law - equal distances covered in equal time).
 */
Worldclock::MoonPhase Worldclock::getMoonPhase() {
    // number of seconds since reference point
    time_t elapsed_time = RTC.get() - FIRST_NEW_MOON;
    double elapsed_days = elapsed_time / 60 / 60 / 24;
    // number of seconds in the current cycle
    time_t days_in_cycle = std::fmod(elapsed_days, LUNAR_CYCLE);
    double percent_in_cycle = days_in_cycle / LUNAR_CYCLE;
    
    if (percent_in_cycle > 99 && percent_in_cycle <= 1) {
        return MoonPhase::NEW;
    } else if (percent_in_cycle > 1 && percent_in_cycle <= 17.3) {
        return MoonPhase::WAXING_CRESCENT;
    } else if (percent_in_cycle > 17.3 && percent_in_cycle <= 33.6) {
        return MoonPhase::FIRST_QUARTER;
    } else if (percent_in_cycle > 33.6 && percent_in_cycle <= 49.9) {
        return MoonPhase::WAXING_GIBBOUS;
    } else if (percent_in_cycle > 49.9 && percent_in_cycle <= 51) {
        return MoonPhase::FULL;
    } else if (percent_in_cycle > 51 && percent_in_cycle <= 67.3) {
        return MoonPhase::WANING_GIBBOUS;
    } else if (percent_in_cycle > 67.3 && percent_in_cycle <= 84.6) {
        return MoonPhase::THIRD_QUARTER;
    } 
    
    // percent_in_cycle > 84.6 && percent_in_cycle <= 99)
    return MoonPhase::WANING_CRESCENT;
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
    if (currentTime.Hour + offset < 10) {
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