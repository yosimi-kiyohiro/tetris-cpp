#pragma once
#include "raylib.h"
#include "constants.hpp"
#include "Board.hpp"
#include "Tetromino.hpp"

class Renderer {
public:
    Renderer();
    ~Renderer();

    bool shouldClose() const;
    void beginFrame();
    void endFrame();
    void drawBoard();
    void drawLockedCells(const Board& board);
    void drawPiece(const Tetromino& t);

private:
    Font jpFont_;
    bool fontOwned_;

    void drawCell(int col, int row, Color c);
};
