#include "Worldclock.h"
#include "Fonts/FreeMono24pt7b.h"
#include "Fonts/FreeMono9pt7b.h"

RTC_DATA_ATTR int8_t current_day;
// TODO: try this with Preferences.h 
// RTC_DATA_ATTR const char* timezone;
RTC_DATA_ATTR int timezone_offset;
RTC_DATA_ATTR bool has_fetched;

Worldclock::Worldclock() {}

// display is 200x200
void Worldclock::drawWatchFace() {
    display.fillScreen(GxEPD_WHITE);
    display.setTextColor(GxEPD_BLACK);
    display.setTextWrap(false);

    if (currentTime.Hour == 0 && currentTime.Minute == 0) {
        sensor.resetStepCounter();
    }

    updateFromWifi();
    
    drawMoons(25/* icon_size */);
    drawClock();

    // body
    display.setFont(&FreeMono9pt7b);
    drawBody();
    
    drawBattery();
    drawDate();
}

void Worldclock::updateFromWifi() {
    // update only once per day
    if (has_fetched && current_day == currentTime.Wday) {
        return;
    }

    current_day = currentTime.Wday;
    if (!connectWiFi()) {
        Serial.println("[-] Failed connecting to WiFi. Not updating.");
        return;
    }
    
    Serial.println("[*] Starting Wifi update.");
    HTTPClient http;
    http.setConnectTimeout(3000);
    http.setReuse(false);
    http.begin(String("https://api.ipgeolocation.io/timezone?apiKey=") + String(IPGEO_API_KEY));
    int responseCode = http.GET();
    if (responseCode == 200) {
        String body = http.getString();
        JSONVar responseObj = JSON.parse(body);
        timezone_offset = int(responseObj["timezone_offset"]);
        has_fetched = true;
        Serial.println("[+] Timezone set.");
    } else {
        Serial.printf("[-] Failed fetching timezone: %s\n", http.errorToString(responseCode).c_str());
    }

    http.end();
}

/**
 * Draws the battery level by drawing a filled rectangle at the bottom left corner of the battery icon and then passing a negative length to draw upwards.
 * The max battery voltage is 4.25 and the height of the battery icon is icon_height - 2*top_margin, so the current voltage is translated into 'icon height'
 * by: batteryVoltage * iconHeight / 4.2 
 * _________
 * |   _    |
 * | _|_|_  |
 * | |    | |
 * | |    | |
 * | |    | |
 * | |____| |
 * |________|
 */
void Worldclock::drawBattery() {
    int8_t icon_width = 28, icon_height = 30, side_margin = 8, top_margin = 4;
    display.drawBitmap(200 - icon_width, 200 - icon_height, battery, icon_width, icon_height, GxEPD_WHITE, GxEPD_BLACK);
    int16_t battery_level = (getBatteryVoltage() * (icon_height - 2*top_margin)) / 4.25;
    display.fillRect(200 - icon_width + side_margin, 200 - top_margin, icon_width - side_margin*2, -battery_level, GxEPD_BLACK);
}

void Worldclock::drawBody() {
    int16_t x, y;
    uint16_t width, height;
    display.getTextBounds(timezone, 
        display.getCursorX(), display.getCursorY(), 
        &x, &y, 
        &width, &height);
    display.setCursor(100 - width/2, 60);
    display.print(timezone);

    int x_offset = 45, y_offset = 70, icon_size = 50;
    display.drawBitmap(x_offset, y_offset, colombia, icon_size, icon_size, GxEPD_WHITE, GxEPD_BLACK);
    display.setCursor(x_offset + icon_size, y_offset + icon_size/2);
    display.print(buildTime(-5));

    display.drawBitmap(x_offset, y_offset + icon_size, steps, 40, 40, GxEPD_WHITE, GxEPD_BLACK);
    display.setCursor(x_offset + icon_size, y_offset + icon_size + 40/2);
    display.println(String(sensor.getCounter()));
}

void Worldclock::drawDate() {
    display.setCursor(30, 195);
    String txt = String(currentTime.Day) + "/" + String(currentTime.Month);
    display.print(txt);

    int16_t x, y;
    uint16_t width, height;
    display.getTextBounds(txt, 
        display.getCursorX(), display.getCursorY(), 
        &x, &y, 
        &width, &height);

    display.setCursor(30, 195 - height);
    uint8_t wday = currentTime.Wday;
    if (wday == 1) {
        txt = "DOMINGO";
    } else if (wday == 2) {
        txt = "LUNES";
    } else if (wday == 3) {
        txt = "MARTES";
    } else if (wday == 4) {
        txt = "MIERCOLES";
    } else if (wday == 5) {
        txt = "JUEVES";
    } else if (wday == 6) {
        txt = "VIERNES";
    } else if (wday == 7) {
        txt = "SABADO";
    }
    display.print(txt);
}

void Worldclock::drawClock() {
    display.setFont(&FreeMono24pt7b);
    int16_t x_boundary, y_boundary;
    uint16_t width, height;
    
    String time = buildTime(timezone_offset);
    display.getTextBounds(time, 
        display.getCursorX(), display.getCursorY(), 
        &x_boundary, &y_boundary, 
        &width, &height);
    display.setCursor(100 - width/2, height + 10);
    display.print(time);
}

// x,y is top left corner
void Worldclock::drawMoons(int icon_size) {
    display.drawBitmap(0, icon_size*0, moon_new, icon_size, icon_size, GxEPD_WHITE, GxEPD_BLACK);
    display.drawBitmap(0, icon_size*1, moon_waxing_crescent, icon_size, icon_size, GxEPD_WHITE, GxEPD_BLACK);
    display.drawBitmap(0, icon_size*2, moon_first_quarter, icon_size, icon_size, GxEPD_WHITE, GxEPD_BLACK);  
    display.drawBitmap(0, icon_size*3, moon_waxing_gibbous, icon_size, icon_size, GxEPD_WHITE, GxEPD_BLACK);
    display.drawBitmap(0, icon_size*4, moon_full, icon_size, icon_size, GxEPD_WHITE, GxEPD_BLACK);
    display.drawBitmap(0, icon_size*5, moon_waning_gibbous, icon_size, icon_size, GxEPD_WHITE, GxEPD_BLACK);
    display.drawBitmap(0, icon_size*6, moon_third_quarter, icon_size, icon_size, GxEPD_WHITE, GxEPD_BLACK);
    display.drawBitmap(0, icon_size*7, moon_waning_crescent, icon_size, icon_size, GxEPD_WHITE, GxEPD_BLACK);

    switch (getMoonPhase()) {
        case MoonPhase::NEW:
            display.drawRect(0, icon_size*0, icon_size, icon_size, GxEPD_BLACK);
        break;
        case MoonPhase::WAXING_CRESCENT:
            display.drawRect(0, icon_size*1, icon_size, icon_size, GxEPD_BLACK);
        break;
        case MoonPhase::FIRST_QUARTER:
            display.drawRect(0, icon_size*2, icon_size, icon_size, GxEPD_BLACK);
        break;
        case MoonPhase::WAXING_GIBBOUS:
            display.drawRect(0, icon_size*3, icon_size, icon_size, GxEPD_BLACK);
        break;
        case MoonPhase::FULL:
            display.drawRect(0, icon_size*4, icon_size, icon_size, GxEPD_BLACK);
        break;
        case MoonPhase::WANING_GIBBOUS:
            display.drawRect(0, icon_size*5, icon_size, icon_size, GxEPD_BLACK);
        break;
        case MoonPhase::THIRD_QUARTER:
            display.drawRect(0, icon_size*6, icon_size, icon_size, GxEPD_BLACK);
        break;
        case MoonPhase::WANING_CRESCENT:
            display.drawRect(0, icon_size*7, icon_size, icon_size, GxEPD_BLACK);
        break;
    }
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
    time_t elapsed_time = RTC.rtc_ds.get() - FIRST_NEW_MOON;
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

String Worldclock::buildTime(int offset) {
    String time = "";
    if (currentTime.Hour + offset < 10) {
        time += "0";
    }
    time += String(currentTime.Hour + offset);
    time += ":";
    if (currentTime.Minute < 10){
        time += "0";
    }  
    time += currentTime.Minute;
    return time;   
}