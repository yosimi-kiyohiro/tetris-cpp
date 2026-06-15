#pragma once
#include "Board.hpp"
#include "Tetromino.hpp"
#include <optional>

class GameState {
public:
    GameState();

    void update(float dt);
    void handleInput();
    void resetContinue();   // 前回レベルから継続
    void resetNewGame();    // Lv.1 からニューゲーム
    void clearAll();        // ハイスコア＋レベルをリセット
    void clearLevel();      // レベルだけリセット

    const Board&     board()        const { return board_; }
    const Tetromino& current()      const { return current_; }
    Tetromino        calcGhost()    const;

    TetrominoType    holdType()     const {
        return hold_.has_value() ? hold_->type : TetrominoType::None;
    }
    TetrominoType    nextPeek();

    int  score()        const { return score_; }
    int  highScore()    const { return highScore_; }
    int  level()        const { return level_; }
    int  savedLevel()   const { return savedLevel_; }
    int  linesCleared() const { return linesCleared_; }
    bool isStarted()    const { return started_; }
    bool isPaused()     const { return paused_; }
    bool isGameOver()   const { return gameOver_; }

private:
    Board    board_;
    Tetromino current_;

    std::optional<Tetromino> hold_;
    bool  hasHeldThisTurn_;

    float fallTimer_;
    float fallInterval_;

    float lockDelayTimer_;
    int   lockDelayResets_;

    int  score_;
    int  highScore_;
    int  level_;
    int  savedLevel_;
    int  linesCleared_;

    bool started_;
    bool paused_;
    bool gameOver_;

    bool onGround() const;
    bool tryMove(int dx, int dy);
    bool tryRotate();
    void tryHold();
    void hardDrop();
    void lockAndSpawn();
    void resetState(int startLevel);
};
