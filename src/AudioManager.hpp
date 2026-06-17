#pragma once
#include "raylib.h"

struct AudioFlags {
    bool move     = false;
    bool rotate   = false;
    bool hardDrop = false;
    bool clear    = false;
    bool levelUp  = false;
    bool gameOver = false;
};

class AudioManager {
public:
    AudioManager();
    ~AudioManager();

    void handleInput();          // Mキー=BGM切替, Sキー=SE切替
    void update(float dt);       // UpdateMusicStream を毎フレーム呼ぶ
    void play(const AudioFlags& af);

private:
    Sound seMove_;
    Sound seRotate_;
    Sound seHardDrop_;
    Sound seClear_;
    Sound seLevelUp_;
    Sound seGameOver_;
    Music bgm_;

    bool mutedBgm_;
    bool mutedSe_;
};
