#pragma once
#include "Board.hpp"
#include "Tetromino.hpp"
#include <optional>

// 演出フェーズ
enum class AnimPhase { None, Flash };

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

    // 演出
    bool  isAnimating()        const { return animPhase_ != AnimPhase::None; }
    float flashAlpha()         const;           // 0.0〜1.0（Rendererがフラッシュ強度に使う）
    bool  consumeShakeTrigger();                // ハードドロップ時に true を1回返して消費

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

    // 演出フィールド
    AnimPhase animPhase_;
    float     animTimer_;
    bool      shakeRequested_;

    bool onGround() const;
    bool tryMove(int dx, int dy);
    bool tryRotate();
    void tryHold();
    void hardDrop();
    void lockAndSpawn();
    void finalizeClear();   // フラッシュ後にライン消去・スポーンを実行
    void resetState(int startLevel);
};
