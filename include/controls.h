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
        float _elapsed;
        std::map<int,float> _timingCommand;
        std::map<int,bool> _keyPressed;
};

#endif // HEADER_CONTROLS