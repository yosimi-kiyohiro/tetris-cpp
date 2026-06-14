#pragma once
#include "raylib.h"
#include <vector>

enum class TetrominoType { None, I, O, T, S, Z, J, L };

using Shape = std::vector<std::vector<int>>;

Color typeToColor(TetrominoType t);
Shape rotateMatrix(const Shape& s);

struct Tetromino {
    TetrominoType type  = TetrominoType::None;
    Shape         shape = {};
    int           x     = 0;
    int           y     = 0;

    static Tetromino spawn(TetrominoType t);
    void rotate();
};
