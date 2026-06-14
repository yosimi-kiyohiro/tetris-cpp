#pragma once
#include "Board.hpp"
#include "Tetromino.hpp"
#include <optional>

class GameState {
public:
    GameState();

    void update(float dt);
    void handleInput();

    const Board&     board()    const { return board_; }
    const Tetromino& current()  const { return current_; }
    Tetromino        calcGhost() const;

    TetrominoType    holdType() const {
        return hold_.has_value() ? hold_->type : TetrominoType::None;
    }
    TetrominoType    nextPeek();   // non-const: Board::peekNext() が内部で補充しうる

private:
    Board    board_;
    Tetromino current_;

    std::optional<Tetromino> hold_;
    bool  hasHeldThisTurn_;

    float fallTimer_;
    float fallInterval_;   // 秒/段（Lv1=1.0f）

    float lockDelayTimer_;
    int   lockDelayResets_;

    bool onGround() const;
    bool tryMove(int dx, int dy);
    bool tryRotate();
    void tryHold();
    void hardDrop();
    void lockAndSpawn();
};
