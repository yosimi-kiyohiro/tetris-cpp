#pragma once
#include "Board.hpp"
#include "Tetromino.hpp"

class GameState {
public:
    GameState();

    void update(float dt);
    void handleInput();

    const Board&     board()      const { return board_; }
    const Tetromino& current()    const { return current_; }
    Tetromino        calcGhost()  const;

private:
    Board     board_;
    Tetromino current_;
    float     fallTimer_;
    float     fallInterval_;  // 秒/段（Lv1=1.0f）

    bool tryMove(int dx, int dy);
    bool tryRotate();
    void hardDrop();
    void lockAndSpawn();
};
