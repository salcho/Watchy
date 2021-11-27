#ifndef WORLDCLOCK_WATCHY_H
#define WORLDCLOCK_WATCHY_H

#include <Watchy.h>
#include "WString.h"
#include "GoogleSans_Regular20pt7b.h"

class Worldclock : public Watchy {
    public:
        Worldclock();
        void drawWatchFace();

    private:
        std::string buildTime();
};

#endif