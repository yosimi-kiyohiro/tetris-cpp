#include "Renderer.hpp"
#include <algorithm>

// 色定数（JS版 #1a1a2e 踏襲）
static const Color COL_BG       = {26, 26, 46, 255};
static const Color COL_BOARD_BG = {20, 20, 40, 255};
static const Color COL_GRID     = {40, 40, 80, 255};
static const Color COL_PANEL_BG = {15, 15, 35, 255};
static const Color COL_LABEL    = {180, 180, 200, 255};

static constexpr int MINI_CELL  = 20;
static constexpr int PANEL_H    = 100;  // ラベル+ピース を収めるパネル高さ

Renderer::Renderer() : fontOwned_(false) {
    InitWindow(WIN_W, WIN_H, "Block Drop C++");
    SetTargetFPS(TARGET_FPS);

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
    DrawRectangle(BOARD_X, BOARD_Y, BOARD_W, BOARD_H, COL_BOARD_BG);

    for (int col = 1; col < BOARD_COLS; col++) {
        int x = BOARD_X + col * CELL_SIZE;
        DrawLine(x, BOARD_Y, x, BOARD_Y + BOARD_H, COL_GRID);
    }
    for (int row = 1; row < BOARD_ROWS; row++) {
        int y = BOARD_Y + row * CELL_SIZE;
        DrawLine(BOARD_X, y, BOARD_X + BOARD_W, y, COL_GRID);
    }

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

void Renderer::drawGhost(const Tetromino& t) {
    Color ghostFill    = typeToColor(t.type);
    Color ghostOutline = typeToColor(t.type);
    ghostFill.a = 60;

    for (int r = 0; r < (int)t.shape.size(); r++) {
        for (int c = 0; c < (int)t.shape[r].size(); c++) {
            if (!t.shape[r][c] || t.y + r < 0) continue;
            int x = BOARD_X + (t.x + c) * CELL_SIZE;
            int y = BOARD_Y + (t.y + r) * CELL_SIZE;
            DrawRectangle(x + 1, y + 1, CELL_SIZE - 2, CELL_SIZE - 2, ghostFill);
            DrawRectangleLines(x + 1, y + 1, CELL_SIZE - 2, CELL_SIZE - 2, ghostOutline);
        }
    }
}

void Renderer::drawPiece(const Tetromino& t) {
    for (int r = 0; r < (int)t.shape.size(); r++)
        for (int c = 0; c < (int)t.shape[r].size(); c++)
            if (t.shape[r][c] && t.y + r >= 0)
                drawCell(t.x + c, t.y + r, typeToColor(t.type));
}

// ピースのシェイプを cx, cy を中心に MINI_CELL サイズで描く
void Renderer::drawMiniPiece(TetrominoType type, int cx, int cy) {
    if (type == TetrominoType::None) return;
    Shape shape = Tetromino::spawn(type).shape;
    int rows = (int)shape.size();
    int cols = rows > 0 ? (int)shape[0].size() : 0;

    // 実際に埋まっているセルの範囲を計算して中央揃え
    int minR = rows, maxR = -1, minC = cols, maxC = -1;
    for (int r = 0; r < rows; r++)
        for (int c = 0; c < cols; c++)
            if (shape[r][c]) {
                minR = std::min(minR, r); maxR = std::max(maxR, r);
                minC = std::min(minC, c); maxC = std::max(maxC, c);
            }
    if (maxR < 0) return;

    int spanW  = (maxC - minC + 1) * MINI_CELL;
    int spanH  = (maxR - minR + 1) * MINI_CELL;
    int startX = cx - spanW / 2 - minC * MINI_CELL;
    int startY = cy - spanH / 2 - minR * MINI_CELL;

    Color c = typeToColor(type);
    for (int r = 0; r < rows; r++)
        for (int col2 = 0; col2 < (int)shape[r].size(); col2++)
            if (shape[r][col2])
                DrawRectangle(startX + col2 * MINI_CELL + 1,
                              startY + r    * MINI_CELL + 1,
                              MINI_CELL - 2, MINI_CELL - 2, c);
}

void Renderer::drawHoldPanel(TetrominoType holdType) {
    int panelX = 0;
    int panelY = BOARD_Y;
    int cx     = PANEL_W / 2;          // 60
    int cy     = panelY + PANEL_H / 2; // 垂直中央

    // パネル背景
    DrawRectangle(panelX, panelY, PANEL_W, PANEL_H, COL_PANEL_BG);
    DrawRectangleLines(panelX, panelY, PANEL_W, PANEL_H, COL_GRID);

    // ラベル
    const char* label = "ホールド";
    Vector2 sz = MeasureTextEx(jpFont_, label, 16, 1);
    DrawTextEx(jpFont_, label,
               {(float)cx - sz.x / 2.0f, (float)(panelY + 6)},
               16, 1, COL_LABEL);

    // ピース（未保持なら何も描かない）
    drawMiniPiece(holdType, cx, cy + 12);
}

void Renderer::drawNextPanel(TetrominoType nextType) {
    int panelX = BOARD_X + BOARD_W;   // 440
    int panelY = BOARD_Y;
    int cx     = panelX + PANEL_W / 2; // 500
    int cy     = panelY + PANEL_H / 2;

    // パネル背景
    DrawRectangle(panelX, panelY, PANEL_W, PANEL_H, COL_PANEL_BG);
    DrawRectangleLines(panelX, panelY, PANEL_W, PANEL_H, COL_GRID);

    // ラベル
    const char* label = "ネクスト";
    Vector2 sz = MeasureTextEx(jpFont_, label, 16, 1);
    DrawTextEx(jpFont_, label,
               {(float)cx - sz.x / 2.0f, (float)(panelY + 6)},
               16, 1, COL_LABEL);

    // ピース
    drawMiniPiece(nextType, cx, cy + 12);
}
