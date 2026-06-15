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
    void drawGhost(const Tetromino& t);
    void drawPiece(const Tetromino& t);
    void drawHoldPanel(TetrominoType holdType);
    void drawNextPanel(TetrominoType nextType);
    void drawScorePanel(int score, int highScore, int level, int lines);
    void drawStartOverlay(int savedLevel);
    void drawPauseOverlay();
    void drawGameOverOverlay(int savedLevel);

private:
    Font jpFont_;
    bool fontOwned_;

    void drawCell(int col, int row, Color c);
    void drawMiniPiece(TetrominoType type, int cx, int cy);
};
