#ifndef HEADER_CONTROLS
#define HEADER_CONTROLS

#include "lib.h"
#include <map>

class controlsTetris {
    public:
        controlsTetris();
        ~controlsTetris();
        bool IsKeyDownTiming(int key, float timing);
        bool IsKeyDown(int key);
    private:
        float _timingCommand;
        float _elapsed;
};

#endif // HEADER_CONTROLS