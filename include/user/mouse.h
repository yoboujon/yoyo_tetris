#ifndef HEADER_TETRIS_MOUSE
#define HEADER_TETRIS_MOUSE

#include "lib.h"

class TetrisMouse {
public:
    // Singleton
    static TetrisMouse& GetInstance()
    {
        static TetrisMouse instance;
        return instance;
    }
    void SetMouse(MouseCursor cursor);
    void ResetMouse();
    void UpdateMouse();
private:
    // Singleton
    TetrisMouse();
    ~TetrisMouse();
    TetrisMouse(const TetrisMouse&) = delete;
    TetrisMouse& operator=(const TetrisMouse&) = delete;

    // Status
    MouseCursor _mouseStatus;
    bool _mouseChanged;
    bool _specialCursor;
};

#endif // HEADER_TETRIS_MOUSE