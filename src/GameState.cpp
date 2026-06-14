#include "GameState.hpp"
#include "raylib.h"
#include <algorithm>

GameState::GameState()
    : board_(),
      current_(Tetromino::spawn(board_.nextType())),
      fallTimer_(0.0f),
      fallInterval_(1.0f)
{}

bool GameState::tryMove(int dx, int dy) {
    Tetromino moved = current_;
    moved.x += dx;
    moved.y += dy;
    if (!board_.isValid(moved)) return false;
    current_ = moved;
    return true;
}

bool GameState::tryRotate() {
    Tetromino rotated = current_;
    rotated.rotate();

    // 簡易ウォールキック：±1、±2 の順に試す（Iミノは±2まで必要）
    int kicks[] = {0, -1, 1, -2, 2};
    for (int k : kicks) {
        rotated.x = current_.x + k;
        if (board_.isValid(rotated)) {
            current_ = rotated;
            return true;
        }
    }
    return false;
}

void GameState::hardDrop() {
    while (tryMove(0, 1)) {}
    lockAndSpawn();
}

void GameState::lockAndSpawn() {
    board_.lock(current_);
    current_   = Tetromino::spawn(board_.nextType());
    fallTimer_ = 0.0f;
}

Tetromino GameState::calcGhost() const {
    Tetromino ghost = current_;
    while (board_.isValid(ghost)) ghost.y++;
    ghost.y--;
    return ghost;
}

void GameState::handleInput() {
    if (IsKeyPressed(KEY_LEFT))  tryMove(-1,  0);
    if (IsKeyPressed(KEY_RIGHT)) tryMove( 1,  0);
    if (IsKeyPressed(KEY_UP))    tryRotate();
    if (IsKeyPressed(KEY_SPACE)) hardDrop();
    // ↓ キーはソフトドロップ（update内で速度を上げる）
}

void GameState::update(float dt) {
    dt = std::min(dt, 0.05f);

    // ↓ 押しっぱなしでソフトドロップ（20倍速）
    float interval = IsKeyDown(KEY_DOWN) ? fallInterval_ / 20.0f : fallInterval_;
    fallTimer_ += dt;

    if (fallTimer_ >= interval) {
        fallTimer_ -= interval;
        if (!tryMove(0, 1)) lockAndSpawn();
    }
}
