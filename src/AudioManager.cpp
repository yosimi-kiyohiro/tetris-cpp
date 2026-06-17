#include "AudioManager.hpp"

AudioManager::AudioManager()
    : mutedBgm_(true), mutedSe_(false)
{
    InitAudioDevice();

    seMove_     = LoadSound("assets/sounds/se_move.ogg");
    seRotate_   = LoadSound("assets/sounds/se_rotate.ogg");
    seHardDrop_ = LoadSound("assets/sounds/se_harddrop.ogg");
    seClear_    = LoadSound("assets/sounds/se_clear.ogg");
    seLevelUp_  = LoadSound("assets/sounds/se_levelup.ogg");
    seGameOver_ = LoadSound("assets/sounds/se_gameover.ogg");

    SetSoundVolume(seMove_,     0.9f);
    SetSoundVolume(seRotate_,   0.4f);
    SetSoundVolume(seHardDrop_, 0.6f);
    SetSoundVolume(seClear_,    0.7f);
    SetSoundVolume(seLevelUp_,  0.7f);
    SetSoundVolume(seGameOver_, 0.7f);

    bgm_ = LoadMusicStream("assets/sounds/bgm.ogg");
    SetMusicVolume(bgm_, 0.5f);
    // BGM はデフォルト OFF。Mキーで ONにする
}

AudioManager::~AudioManager() {
    UnloadSound(seMove_);
    UnloadSound(seRotate_);
    UnloadSound(seHardDrop_);
    UnloadSound(seClear_);
    UnloadSound(seLevelUp_);
    UnloadSound(seGameOver_);
    UnloadMusicStream(bgm_);
    CloseAudioDevice();
}

void AudioManager::handleInput() {
    if (IsKeyPressed(KEY_M)) {
        mutedBgm_ = !mutedBgm_;
        if (mutedBgm_) {
            StopMusicStream(bgm_);
        } else {
            PlayMusicStream(bgm_);
        }
    }
    if (IsKeyPressed(KEY_S)) {
        mutedSe_ = !mutedSe_;
    }
}

void AudioManager::update(float dt) {
    (void)dt;
    if (!mutedBgm_) {
        UpdateMusicStream(bgm_);
    }
}

void AudioManager::play(const AudioFlags& af) {
    if (mutedSe_) return;
    if (af.move)     PlaySound(seMove_);
    if (af.rotate)   PlaySound(seRotate_);
    if (af.hardDrop) PlaySound(seHardDrop_);
    if (af.clear)    PlaySound(seClear_);
    if (af.levelUp)  PlaySound(seLevelUp_);
    if (af.gameOver) PlaySound(seGameOver_);
}
