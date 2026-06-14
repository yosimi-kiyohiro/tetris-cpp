#pragma once
#include "raylib.h"
#include "constants.hpp"

class Renderer {
public:
    Renderer();
    ~Renderer();

    bool shouldClose() const;
    void beginFrame();
    void endFrame();
    void drawBoard();

private:
    Font jpFont_;
    bool fontOwned_;  // UnloadFont を呼ぶべきか
};
