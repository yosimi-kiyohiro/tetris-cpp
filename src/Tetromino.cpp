#include "Tetromino.hpp"
#include "constants.hpp"

// JS版 TETROMINOES と同じ形・色
static const Shape SHAPES[] = {
    {},                                                      // 0: None
    {{0,0,0,0}, {1,1,1,1}, {0,0,0,0}, {0,0,0,0}},         // 1: I
    {{1,1}, {1,1}},                                         // 2: O
    {{0,1,0}, {1,1,1}, {0,0,0}},                           // 3: T
    {{0,1,1}, {1,1,0}, {0,0,0}},                           // 4: S
    {{1,1,0}, {0,1,1}, {0,0,0}},                           // 5: Z
    {{1,0,0}, {1,1,1}, {0,0,0}},                           // 6: J
    {{0,0,1}, {1,1,1}, {0,0,0}},                           // 7: L
};

Color typeToColor(TetrominoType t) {
    switch (t) {
        case TetrominoType::I: return Color{  0, 188, 212, 255};
        case TetrominoType::O: return Color{255, 235,  59, 255};
        case TetrominoType::T: return Color{156,  39, 176, 255};
        case TetrominoType::S: return Color{ 76, 175,  80, 255};
        case TetrominoType::Z: return Color{244,  67,  54, 255};
        case TetrominoType::J: return Color{ 33, 150, 243, 255};
        case TetrominoType::L: return Color{255, 152,   0, 255};
        default:               return BLACK;
    }
}

// 90度時計回り回転（JS版 rotateMatrix と同じロジック）
Shape rotateMatrix(const Shape& s) {
    int n = (int)s.size();
    Shape r(n, std::vector<int>(n, 0));
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            r[i][j] = s[n - 1 - j][i];
    return r;
}

Tetromino Tetromino::spawn(TetrominoType t) {
    Tetromino p;
    p.type  = t;
    p.shape = SHAPES[(int)t];
    int w   = (int)p.shape[0].size();
    p.x     = BOARD_COLS / 2 - w / 2;
    p.y     = 0;
    return p;
}

void Tetromino::rotate() {
    shape = rotateMatrix(shape);
}
