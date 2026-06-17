#pragma once
#include "raylib.h"
#include <vector>

struct Particle {
    float x, y;       // 現在位置（ピクセル）
    float vx, vy;     // 速度（px/秒）
    float life;       // 残り寿命（秒）
    float maxLife;    // 初期寿命
    Color color;
    float size;       // 初期サイズ（px）
};

class ParticleSystem {
public:
    void emit(int col, int row, Color color);   // グリッド座標とセル色を受け取る
    void update(float dt);
    void draw() const;

private:
    std::vector<Particle> particles_;
};
