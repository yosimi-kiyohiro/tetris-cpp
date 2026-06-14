#include "GameState.hpp"
#include "raylib.h"
#include <algorithm>

static constexpr float LOCK_DELAY    = 0.5f;
static constexpr int   MAX_RESETS    = 15;

GameState::GameState()
    : board_(),
      current_(Tetromino::spawn(board_.nextType())),
      hasHeldThisTurn_(false),
      fallTimer_(0.0f),
      fallInterval_(1.0f),
      lockDelayTimer_(0.0f),
      lockDelayResets_(0)
{}

bool GameState::onGround() const {
    Tetromino below = current_;
    below.y++;
    return !board_.isValid(below);
}

bool GameState::tryMove(int dx, int dy) {
    Tetromino moved = current_;
    moved.x += dx;
    moved.y += dy;
    if (!board_.isValid(moved)) return false;

    bool wasOnGround = onGround();
    current_ = moved;

    // 着地中に移動成功: ロック遅延タイマーをリセット（上限15回）
    if (wasOnGround && lockDelayResets_ < MAX_RESETS) {
        lockDelayTimer_ = 0.0f;
        lockDelayResets_++;
    }
    return true;
}

bool GameState::tryRotate() {
    Tetromino rotated = current_;
    rotated.rotate();

    bool wasOnGround = onGround();
    int kicks[] = {0, -1, 1, -2, 2};
    for (int k : kicks) {
        rotated.x = current_.x + k;
        if (board_.isValid(rotated)) {
            current_ = rotated;
            if (wasOnGround && lockDelayResets_ < MAX_RESETS) {
                lockDelayTimer_ = 0.0f;
                lockDelayResets_++;
            }
            return true;
        }
    }
    return false;
}

void GameState::tryHold() {
    if (hasHeldThisTurn_) return;
    hasHeldThisTurn_ = true;

    TetrominoType heldType = current_.type;
    if (hold_.has_value()) {
        current_ = Tetromino::spawn(hold_->type);
    } else {
        current_ = Tetromino::spawn(board_.nextType());
    }
    hold_ = Tetromino::spawn(heldType);

    // ロック遅延・落下タイマーをリセット
    lockDelayTimer_  = 0.0f;
    lockDelayResets_ = 0;
    fallTimer_       = 0.0f;
}

void GameState::hardDrop() {
    while (tryMove(0, 1)) {}
    lockAndSpawn();  // ロック遅延をスキップして即固定
}

void GameState::lockAndSpawn() {
    board_.lock(current_);
    current_         = Tetromino::spawn(board_.nextType());
    fallTimer_       = 0.0f;
    lockDelayTimer_  = 0.0f;
    lockDelayResets_ = 0;
    hasHeldThisTurn_ = false;
}

Tetromino GameState::calcGhost() const {
    Tetromino ghost = current_;
    while (board_.isValid(ghost)) ghost.y++;
    ghost.y--;
    return ghost;
}

TetrominoType GameState::nextPeek() {
    return board_.peekNext();
}

void GameState::handleInput() {
    if (IsKeyPressed(KEY_LEFT))  tryMove(-1,  0);
    if (IsKeyPressed(KEY_RIGHT)) tryMove( 1,  0);
    if (IsKeyPressed(KEY_UP))    tryRotate();
    if (IsKeyPressed(KEY_SPACE)) hardDrop();
    if (IsKeyPressed(KEY_C))     tryHold();
}

void GameState::update(float dt) {
    dt = std::min(dt, 0.05f);

    if (onGround()) {
        // 着地中: ロック遅延タイマーを進める
        lockDelayTimer_ += dt;
        if (lockDelayTimer_ >= LOCK_DELAY || lockDelayResets_ >= MAX_RESETS) {
            lockAndSpawn();
        }
    } else {
        // 空中: 通常落下
        float interval = IsKeyDown(KEY_DOWN) ? fallInterval_ / 20.0f : fallInterval_;
        fallTimer_ += dt;
        if (fallTimer_ >= interval) {
            fallTimer_ -= interval;
            tryMove(0, 1);
        }
    }
}
