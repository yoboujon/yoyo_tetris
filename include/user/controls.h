#ifndef HEADER_CONTROLS
#define HEADER_CONTROLS

#include "lib.h"
#include <map>

enum class keyState {
    UNSET = -1,
    NOT_PRESSED,
    PRESSED
};

class tetrisControls {
    public:
        tetrisControls();
        tetrisControls(float* elapsedPtr);
        ~tetrisControls();
        bool IsKeyDownTiming(int key, float timing);
        bool TempoKey(bool keyDown, float timing);
        bool IsKeyDown(int key);
    private:
        float* _elapsedPtr;
        float _timingCommand;
        keyState _keyPressed;
};

#endif // HEADER_CONTROLS