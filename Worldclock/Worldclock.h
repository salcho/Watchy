#ifndef WORLDCLOCK_WATCHY_H
#define WORLDCLOCK_WATCHY_H

#include <Watchy.h>
#include <cmath>
#include "WString.h"
#include "icons.h"
#include "local_config.h"

class Worldclock : public Watchy {
    public:
        Worldclock();
        void drawWatchFace();

    private:
        enum MoonPhase {
            NEW, WAXING_CRESCENT, FIRST_QUARTER, WAXING_GIBBOUS,
            FULL, WANING_GIBBOUS, THIRD_QUARTER, WANING_CRESCENT
        };
        static constexpr double LUNAR_CYCLE = 29.53058770576;  // http://individual.utoronto.ca/kalendis/lunar/#FALC
        static constexpr time_t FIRST_NEW_MOON = 1579902240;   // the first new moon of 2020 was on Jan. 24, Fri 22:44

        String buildTime(int offset);
        void drawMoons(int icon_size);
        void drawBody();
        void drawClock();
        void drawDate();
        void drawBattery();
        void updateFromWifi();
        Worldclock::MoonPhase getMoonPhase();
};

#endif