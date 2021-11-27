#ifndef WORLDCLOCK_WATCHY_H
#define WORLDCLOCK_WATCHY_H

#include <Watchy.h>

class Worldclock : public Watchy {
    public:
        Worldclock();
        void drawWatchFace();
};

#endif