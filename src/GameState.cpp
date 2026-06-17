#include "GameState.hpp"
#include "ScoreFile.hpp"
#include "raylib.h"
#include <algorithm>

static constexpr float LOCK_DELAY  = 0.5f;
static constexpr int   MAX_RESETS  = 15;
static constexpr float FLASH_DUR   = 0.2f;

GameState::GameState() {
    SaveData d  = ScoreFile::load();
    highScore_  = d.highScore;
    savedLevel_ = d.savedLevel;
    resetState(savedLevel_);
    started_   = false;
}

// ---- 内部リセット（startLevel から開始） ----
void GameState::resetState(int startLevel) {
    board_           = Board();
    current_         = Tetromino::spawn(board_.nextType());
    hold_            = std::nullopt;
    hasHeldThisTurn_ = false;
    fallTimer_       = 0.0f;
    lockDelayTimer_  = 0.0f;
    lockDelayResets_ = 0;
    score_           = 0;
    linesCleared_    = 0;
    gameOver_        = false;
    paused_          = false;
    animPhase_       = AnimPhase::None;
    animTimer_       = 0.0f;
    shakeRequested_  = false;
    audioFlags_      = AudioFlags{};

    level_        = startLevel;
    fallInterval_ = std::max(0.1f, 1.0f - (level_ - 1) * 0.1f);
}

// ---- 公開リセット ----
void GameState::resetContinue() {
    resetState(savedLevel_);
    started_ = true;
}

void GameState::resetNewGame() {
    resetState(1);
    started_ = true;
}

void GameState::clearAll() {
    ScoreFile::clearAll();
    highScore_  = 0;
    savedLevel_ = 1;
}

void GameState::clearLevel() {
    ScoreFile::clearLevel();
    savedLevel_ = 1;
}

// ---- 移動・回転・操作 ----
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

    if (wasOnGround && lockDelayResets_ < MAX_RESETS) {
        lockDelayTimer_ = 0.0f;
        lockDelayResets_++;
    }
    audioFlags_.move = true;
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
            audioFlags_.rotate = true;
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

    lockDelayTimer_  = 0.0f;
    lockDelayResets_ = 0;
    fallTimer_       = 0.0f;
}

void GameState::hardDrop() {
    while (tryMove(0, 1)) {}
    audioFlags_.move     = false;  // 落下中の move 音をキャンセル
    audioFlags_.hardDrop = true;
    shakeRequested_ = true;
    lockAndSpawn();
}

void GameState::lockAndSpawn() {
    board_.lock(current_);

    // 消去できる行があればフラッシュ演出を開始（消去は finalizeClear で行う）
    if (board_.countFullLines() > 0) {
        animPhase_ = AnimPhase::Flash;
        animTimer_ = 0.0f;
        return;
    }
    finalizeClear();
}

void GameState::finalizeClear() {
    // clearLines() の前にフル行の色情報を取得してパーティクルキューに積む
    for (int row : board_.getFullLineRows()) {
        for (int col = 0; col < BOARD_COLS; col++) {
            particleEmits_.push_back({ col, row, board_.cell(row, col) });
        }
    }

    // ライン消去・スコア計算
    int lines = board_.clearLines();
    if (lines > 0) {
        static constexpr int SCORE_TABLE[] = {0, 100, 300, 500, 800};
        score_ += SCORE_TABLE[lines];
        linesCleared_ += lines;

        // 10ラインごとにレベルアップ・落下速度更新（最速100ms）
        int newLevel = linesCleared_ / 10 + 1;
        if (newLevel > level_) {
            level_        = newLevel;
            fallInterval_ = std::max(0.1f, 1.0f - (level_ - 1) * 0.1f);
            audioFlags_.levelUp = true;
        } else {
            audioFlags_.clear = true;
        }
    }

    // 次のピースをスポーン
    current_         = Tetromino::spawn(board_.nextType());
    fallTimer_       = 0.0f;
    lockDelayTimer_  = 0.0f;
    lockDelayResets_ = 0;
    hasHeldThisTurn_ = false;

    animPhase_ = AnimPhase::None;

    // スポーン位置が埋まっていたらゲームオーバー
    if (!board_.isValid(current_)) {
        gameOver_ = true;
        audioFlags_.gameOver = true;
        if (score_ > highScore_) highScore_ = score_;
        if (level_ > savedLevel_) savedLevel_ = level_;
        ScoreFile::save(highScore_, savedLevel_);
    }
}

Tetromino GameState::calcGhost() const {
    Tetromino ghost = current_;
    while (board_.isValid(ghost)) ghost.y++;
    ghost.y--;
    return ghost;
}

float GameState::flashAlpha() const {
    if (animPhase_ != AnimPhase::Flash) return 0.0f;
    // フラッシュ開始直後が最大・時間とともに薄れる
    return 1.0f - (animTimer_ / FLASH_DUR);
}

bool GameState::consumeShakeTrigger() {
    if (!shakeRequested_) return false;
    shakeRequested_ = false;
    return true;
}

AudioFlags GameState::consumeAudioFlags() {
    AudioFlags copy = audioFlags_;
    audioFlags_ = AudioFlags{};
    return copy;
}

std::vector<ParticleEmitData> GameState::consumeParticleEmits() {
    std::vector<ParticleEmitData> copy = std::move(particleEmits_);
    particleEmits_.clear();
    return copy;
}

TetrominoType GameState::nextPeek() {
    return board_.peekNext();
}

void GameState::handleInput() {
    if (!started_) {
        if (IsKeyPressed(KEY_SPACE)) resetContinue();
        if (IsKeyPressed(KEY_N))     resetNewGame();
        if (IsKeyPressed(KEY_A))     clearAll();
        if (IsKeyPressed(KEY_L))     clearLevel();
        return;
    }
    if (gameOver_) {
        if (IsKeyPressed(KEY_R)) resetContinue();
        if (IsKeyPressed(KEY_N)) resetNewGame();
        if (IsKeyPressed(KEY_A)) clearAll();
        if (IsKeyPressed(KEY_L)) clearLevel();
        return;
    }
    if (IsKeyPressed(KEY_P) || IsKeyPressed(KEY_ESCAPE)) {
        paused_ = !paused_;
        return;
    }
    if (paused_) return;
    if (isAnimating()) return;  // 演出中は入力無視

    if (IsKeyPressed(KEY_LEFT))  tryMove(-1, 0);
    if (IsKeyPressed(KEY_RIGHT)) tryMove( 1, 0);
    if (IsKeyPressed(KEY_UP))    tryRotate();
    if (IsKeyPressed(KEY_SPACE)) hardDrop();
    if (IsKeyPressed(KEY_C))     tryHold();
}

void GameState::update(float dt) {
    if (!started_ || paused_ || gameOver_) return;
    dt = std::min(dt, 0.05f);

    // フラッシュ演出の更新
    if (animPhase_ == AnimPhase::Flash) {
        animTimer_ += dt;
        if (animTimer_ >= FLASH_DUR) {
            finalizeClear();
        }
        return;  // 演出中はゲームロジックを止める
    }

    if (onGround()) {
        lockDelayTimer_ += dt;
        if (lockDelayTimer_ >= LOCK_DELAY || lockDelayResets_ >= MAX_RESETS) {
            lockAndSpawn();
        }
    } else {
        float interval = IsKeyDown(KEY_DOWN) ? fallInterval_ / 20.0f : fallInterval_;
        fallTimer_ += dt;
        if (fallTimer_ >= interval) {
            fallTimer_ -= interval;
            tryMove(0, 1);
        }
    }
}
