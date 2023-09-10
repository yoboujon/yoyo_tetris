#ifndef HEADER_TETRIS_INPUT_MANAGER
#define HEADER_TETRIS_INPUT_MANAGER

#include "event/component.h"
#include "lib.h"

#include <vector>

class TetrisInputManager : public BaseComponent {
public:
    // Singleton
    static TetrisInputManager& GetInstance()
    {
        static TetrisInputManager instance;
        return instance;
    }

    void Update();
    std::vector<int> GetKeys();
    int GetKey();
    void ClearKeys();

private:
    // Singleton
    TetrisInputManager();
    ~TetrisInputManager();
    TetrisInputManager(const TetrisInputManager&) = delete;
    TetrisInputManager& operator=(const TetrisInputManager&) = delete;

    std::vector<int> _keysEntered; // deque of KeyboardKey
};

#endif // HEADER_TETRIS_INPUT_MANAGER