#include "Renderer.hpp"

// 色定数（JS版 #1a1a2e 踏襲）
static const Color COL_BG       = {26, 26, 46, 255};
static const Color COL_BOARD_BG = {20, 20, 40, 255};
static const Color COL_GRID     = {40, 40, 80, 255};

Renderer::Renderer() : fontOwned_(false) {
    InitWindow(WIN_W, WIN_H, "Block Drop C++");
    SetTargetFPS(TARGET_FPS);

    // ゲームで使う文字を列挙してフォントアトラスに焼き込む
    const char* gameChars =
        "ブロックドロップスコアレベルライン消去"
        "ゲームオーバースタート一時停止再開"
        "ホールドネクストハイ"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz"
        "0123456789 :/-.!";
    int cpCount = 0;
    int* cp = LoadCodepoints(gameChars, &cpCount);
    jpFont_ = LoadFontEx("assets/fonts/NotoSansJP.ttf", 20, cp, cpCount);
    UnloadCodepoints(cp);

    if (jpFont_.glyphCount > 0) {
        fontOwned_ = true;
    } else {
        jpFont_ = GetFontDefault();
    }
}

Renderer::~Renderer() {
    if (fontOwned_) UnloadFont(jpFont_);
    CloseWindow();
}

bool Renderer::shouldClose() const {
    return WindowShouldClose();
}

void Renderer::beginFrame() {
    BeginDrawing();
    ClearBackground(COL_BG);
}

void Renderer::endFrame() {
    EndDrawing();
}

void Renderer::drawBoard() {
    // 盤面背景
    DrawRectangle(BOARD_X, BOARD_Y, BOARD_W, BOARD_H, COL_BOARD_BG);

    // グリッド線（縦）
    for (int col = 1; col < BOARD_COLS; col++) {
        int x = BOARD_X + col * CELL_SIZE;
        DrawLine(x, BOARD_Y, x, BOARD_Y + BOARD_H, COL_GRID);
    }
    // グリッド線（横）
    for (int row = 1; row < BOARD_ROWS; row++) {
        int y = BOARD_Y + row * CELL_SIZE;
        DrawLine(BOARD_X, y, BOARD_X + BOARD_W, y, COL_GRID);
    }

    // 盤面の外枠
    DrawRectangleLines(BOARD_X, BOARD_Y, BOARD_W, BOARD_H, WHITE);
}

void Renderer::drawCell(int col, int row, Color c) {
    int x = BOARD_X + col * CELL_SIZE;
    int y = BOARD_Y + row * CELL_SIZE;
    DrawRectangle(x + 1, y + 1, CELL_SIZE - 2, CELL_SIZE - 2, c);
}

void Renderer::drawLockedCells(const Board& board) {
    for (int r = 0; r < BOARD_ROWS; r++)
        for (int c = 0; c < BOARD_COLS; c++)
            if (board.cell(r, c) != TetrominoType::None)
                drawCell(c, r, typeToColor(board.cell(r, c)));
}

void Renderer::drawPiece(const Tetromino& t) {
    for (int r = 0; r < (int)t.shape.size(); r++)
        for (int c = 0; c < (int)t.shape[r].size(); c++)
            if (t.shape[r][c] && t.y + r >= 0)
                drawCell(t.x + c, t.y + r, typeToColor(t.type));
}
