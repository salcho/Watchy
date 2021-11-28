#ifndef WORLDCLOCK_WATCHY_H
#define WORLDCLOCK_WATCHY_H

#include <Watchy.h>
#include "WString.h"
#include "FreeMono12pt7b.h"

class Worldclock : public Watchy {
    public:
        Worldclock();
        void drawWatchFace();

    private:
        uint16_t x_offset, y_offset;
        
        String buildTime(int offset);
        void writeLine(String txt);
};

#endif