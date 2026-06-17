#include "ParticleSystem.hpp"
#include "constants.hpp"
#include <algorithm>
#include <cmath>
#include <cstdlib>

static constexpr int   PARTICLES_PER_CELL = 5;
static constexpr float GRAVITY            = 250.0f;   // px/秒²
static constexpr float SPEED_MIN          = 60.0f;
static constexpr float SPEED_MAX          = 180.0f;
static constexpr float LIFE_MIN           = 0.35f;
static constexpr float LIFE_MAX           = 0.65f;
static constexpr float SIZE_BASE          = 6.0f;

static float randF(float lo, float hi) {
    return lo + (hi - lo) * ((float)rand() / (float)RAND_MAX);
}

void ParticleSystem::emit(int col, int row, Color color) {
    float cx = BOARD_X + col * CELL_SIZE + CELL_SIZE * 0.5f;
    float cy = BOARD_Y + row * CELL_SIZE + CELL_SIZE * 0.5f;

    for (int i = 0; i < PARTICLES_PER_CELL; i++) {
        float angle = randF(0.0f, 6.2832f);  // 0〜2π
        float speed = randF(SPEED_MIN, SPEED_MAX);
        float life  = randF(LIFE_MIN, LIFE_MAX);

        Particle p;
        p.x       = cx + randF(-4.0f, 4.0f);
        p.y       = cy + randF(-4.0f, 4.0f);
        p.vx      = cosf(angle) * speed;
        p.vy      = sinf(angle) * speed - 40.0f;  // 少し上向きの初速
        p.life    = life;
        p.maxLife = life;
        p.color   = color;
        p.size    = SIZE_BASE + randF(-2.0f, 2.0f);
        particles_.push_back(p);
    }
}

void ParticleSystem::update(float dt) {
    for (auto& p : particles_) {
        p.vy   += GRAVITY * dt;
        p.x    += p.vx * dt;
        p.y    += p.vy * dt;
        p.life -= dt;
    }
    // 寿命が尽きたパーティクルを削除
    particles_.erase(
        std::remove_if(particles_.begin(), particles_.end(),
                       [](const Particle& p) { return p.life <= 0.0f; }),
        particles_.end()
    );
}

void ParticleSystem::draw() const {
    for (const auto& p : particles_) {
        float ratio = p.life / p.maxLife;           // 1.0→0.0
        float sz    = p.size * ratio;
        if (sz < 1.0f) continue;

        Color c = p.color;
        c.a = (unsigned char)(255 * ratio);          // フェードアウト

        DrawRectangle(
            (int)(p.x - sz * 0.5f),
            (int)(p.y - sz * 0.5f),
            (int)sz, (int)sz, c
        );
    }
}
