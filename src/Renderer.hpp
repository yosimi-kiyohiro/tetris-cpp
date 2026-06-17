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
    void update(float dt);          // シェイクタイマーを進める
    void triggerShake();            // ハードドロップ時に呼ぶ
    void drawBoard();
    void drawLockedCells(const Board& board);
    void drawGhost(const Tetromino& t);
    void drawPiece(const Tetromino& t);
    void drawFlashOverlay(float alpha);  // alpha: 1.0（最大）→0.0
    void drawHoldPanel(TetrominoType holdType);
    void drawNextPanel(TetrominoType nextType);
    void drawScorePanel(int score, int highScore, int level, int lines);
    void drawStartOverlay(int savedLevel);
    void drawPauseOverlay();
    void drawGameOverOverlay(int savedLevel);

private:
    Font  jpFont_;
    bool  fontOwned_;

    // シェイク演出
    float shakeTimer_;   // 残り時間（0 = 非アクティブ）
    float shakeOffset_;  // 現在のX方向オフセット（px）

    void drawCell(int col, int row, Color c);
    void drawMiniPiece(TetrominoType type, int cx, int cy);
};
