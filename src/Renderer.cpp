#include "Renderer.hpp"
#include <algorithm>

// 色定数
static const Color COL_BG       = {15, 15, 30,  255};  // 背景
static const Color COL_BOARD_BG = {10, 10, 22,  255};  // 盤面
static const Color COL_GRID     = {40, 55, 100, 255};  // グリッド線
static const Color COL_PANEL_BG = {22, 38, 80,  255};  // スコアパネル背景（ダークブルー）
static const Color COL_SIDE_BG  = {18, 52, 52,  255};  // ホールド・ネクスト背景（ダークティール）
static const Color COL_BORDER   = {80, 130, 200, 255}; // パネル枠
static const Color COL_LABEL    = {255, 225, 50,  255}; // ラベル（黄色・輝度最高・色覚特性を問わず視認可）
static const Color COL_SUBLABEL = {210, 210, 210, 255}; // 補助テキスト（薄いグレー）

static constexpr int MINI_CELL  = 20;
static constexpr int PANEL_H    = 100;  // ラベル+ピース を収めるパネル高さ

Renderer::Renderer() : fontOwned_(false) {
    InitWindow(WIN_W, WIN_H, "Block Drop C++");
    SetTargetFPS(TARGET_FPS);

    const char* gameChars =
        "ブロックドロップスコアレベルライン消去"
        "ゲームオーバースタートニューゲーム一時停止再開リ"
        "ホールドネクストハイから継続全クリア"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz"
        "0123456789 :/-.![].";
    int cpCount = 0;
    int* cp = LoadCodepoints(gameChars, &cpCount);
    jpFont_ = LoadFontEx("assets/fonts/NotoSansJP.ttf", 40, cp, cpCount);
    UnloadCodepoints(cp);
    SetTextureFilter(jpFont_.texture, TEXTURE_FILTER_BILINEAR);

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

// 数値を安全に文字列化（TextFormat の静的バッファ問題を回避）
static void intToStr(char* buf, int size, int val) {
    snprintf(buf, size, "%d", val);
}

// 任意座標に縁取り付きで描く
static void drawTextOutlined(Font font, const char* text, float x, float y, float size, Color col) {
    Color ol = {0, 0, 0, 255};
    float d = 2.0f;
    DrawTextEx(font, text, {x-d, y-d}, size, 1, ol);
    DrawTextEx(font, text, {x+d, y-d}, size, 1, ol);
    DrawTextEx(font, text, {x-d, y+d}, size, 1, ol);
    DrawTextEx(font, text, {x+d, y+d}, size, 1, ol);
    DrawTextEx(font, text, {x,   y  }, size, 1, col);
}

// タイトル用：1文字ずつテトリスカラーで縁取り描画
static void drawRainbowTitleOutlined(Font font, const char* text, float y, float size) {
    static const Color COLORS[] = {
        {240, 50,  50,  255},  // 赤 (Z)
        {240, 140, 0,   255},  // オレンジ (L)
        {240, 220, 0,   255},  // 黄 (O)
        {50,  210, 50,  255},  // 緑 (S)
        {0,   200, 240, 255},  // シアン (I)
        {80,  80,  240, 255},  // 青 (J)
        {200, 0,   240, 255},  // 紫 (T)
    };
    Color ol = {0, 0, 0, 255};
    float d  = 2.0f;

    Vector2 totalSz = MeasureTextEx(font, text, size, 1);
    float x = BOARD_X + (BOARD_W - totalSz.x) / 2.0f;

    int byteCount = 0;
    const char* ptr = text;
    int colorIdx = 0;
    while (*ptr != '\0') {
        GetCodepoint(ptr, &byteCount);
        char buf[8] = {};
        for (int b = 0; b < byteCount && b < 7; b++) buf[b] = ptr[b];

        Color col = COLORS[colorIdx % 7];
        DrawTextEx(font, buf, {x-d, y-d}, size, 1, ol);
        DrawTextEx(font, buf, {x+d, y-d}, size, 1, ol);
        DrawTextEx(font, buf, {x-d, y+d}, size, 1, ol);
        DrawTextEx(font, buf, {x+d, y+d}, size, 1, ol);
        DrawTextEx(font, buf, {x,   y  }, size, 1, col);

        x += MeasureTextEx(font, buf, size, 1).x;
        ptr += byteCount;
        colorIdx++;
    }
}

// 縁取り付きで中央揃えに描く
static void drawCenteredTextOutlined(Font font, const char* text, float y, float size, Color col) {
    Vector2 sz = MeasureTextEx(font, text, size, 1);
    float x = BOARD_X + (BOARD_W - sz.x) / 2.0f;
    drawTextOutlined(font, text, x, y, size, col);
}

void Renderer::drawScorePanel(int score, int highScore, int level, int lines) {
    DrawRectangle(0, 0, WIN_W, TOP_H, COL_PANEL_BG);
    DrawLine(0, TOP_H - 1, WIN_W, TOP_H - 1, COL_BORDER);
    // 列の仕切り線
    int colW0 = WIN_W / 4;
    for (int i = 1; i < 4; i++)
        DrawLine(colW0 * i, 4, colW0 * i, TOP_H - 4, COL_BORDER);

    const char* labels[4] = {"スコア", "ハイ", "レベル", "ライン"};
    int         values[4] = {score, highScore, level, lines};

    int colW = WIN_W / 4;  // 140px
    char valBuf[16];
    for (int i = 0; i < 4; i++) {
        float cx = (float)(colW * i + colW / 2);

        // ラベル（縁取り付き・シアン）
        Vector2 lsz = MeasureTextEx(jpFont_, labels[i], 20, 1);
        drawTextOutlined(jpFont_, labels[i], cx - lsz.x / 2.0f, 6.0f, 20, COL_LABEL);

        // 数値（縁取り付き）
        intToStr(valBuf, sizeof(valBuf), values[i]);
        Vector2 vsz = MeasureTextEx(jpFont_, valBuf, 34, 1);
        drawTextOutlined(jpFont_, valBuf, cx - vsz.x / 2.0f, 32.0f, 34, WHITE);
    }
}


void Renderer::drawStartOverlay(int savedLevel) {
    DrawRectangle(BOARD_X, BOARD_Y, BOARD_W, BOARD_H, {0, 0, 0, 180});

    float midY = BOARD_Y + BOARD_H / 2.0f;
    Color white = WHITE;
    Color sub   = COL_SUBLABEL;

    drawRainbowTitleOutlined(jpFont_, "ブロックドロップ", midY - 85, 40);

    if (savedLevel > 1) {
        char contBuf[32];
        snprintf(contBuf, sizeof(contBuf), "SPACE: Lv.%d から継続", savedLevel);
        drawCenteredTextOutlined(jpFont_, contBuf,        midY - 10, 26, white);
        drawCenteredTextOutlined(jpFont_, "N: ニューゲーム", midY + 26, 24, sub);
    } else {
        drawCenteredTextOutlined(jpFont_, "SPACE: スタート", midY - 10, 28, white);
    }

    drawCenteredTextOutlined(jpFont_, "A: 全クリア   L: レベルクリア", midY + 60, 24, sub);
}

void Renderer::drawPauseOverlay() {
    DrawRectangle(BOARD_X, BOARD_Y, BOARD_W, BOARD_H, {0, 0, 0, 200});

    float midY = BOARD_Y + BOARD_H / 2.0f;
    drawCenteredTextOutlined(jpFont_, "一時停止", midY - 22, 34, WHITE);
    drawCenteredTextOutlined(jpFont_, "P: 再開",  midY + 22, 26, WHITE);
}

void Renderer::drawGameOverOverlay(int savedLevel) {
    DrawRectangle(BOARD_X, BOARD_Y, BOARD_W, BOARD_H, {0, 0, 0, 220});

    float midY = BOARD_Y + BOARD_H / 2.0f;

    Color gold = {255, 220, 80, 255};
    Color sub  = {200, 200, 200, 255};

    drawCenteredTextOutlined(jpFont_, "ゲームオーバー", midY - 80, 36, gold);

    if (savedLevel > 1) {
        char contBuf[32];
        snprintf(contBuf, sizeof(contBuf), "R: Lv.%d から継続", savedLevel);
        drawCenteredTextOutlined(jpFont_, contBuf,            midY - 10, 26, WHITE);
        drawCenteredTextOutlined(jpFont_, "N: ニューゲーム",   midY + 26, 24, sub);
    } else {
        drawCenteredTextOutlined(jpFont_, "R: リスタート",    midY - 10, 28, WHITE);
        drawCenteredTextOutlined(jpFont_, "N: ニューゲーム",   midY + 26, 24, sub);
    }

    drawCenteredTextOutlined(jpFont_, "A: 全クリア   L: レベルクリア", midY + 60, 24, sub);
}

void Renderer::drawHoldPanel(TetrominoType holdType) {
    int panelX = 0;
    int panelY = BOARD_Y;
    int cx     = PANEL_W / 2;          // 60
    int cy     = panelY + PANEL_H / 2; // 垂直中央

    // パネル背景
    DrawRectangle(panelX, panelY, PANEL_W, PANEL_H, COL_SIDE_BG);
    DrawRectangleLines(panelX, panelY, PANEL_W, PANEL_H, COL_BORDER);

    // ラベル
    const char* label = "ホールド";
    Vector2 sz = MeasureTextEx(jpFont_, label, 19, 1);
    drawTextOutlined(jpFont_, label,
                     (float)cx - sz.x / 2.0f, (float)(panelY + 8),
                     19, COL_LABEL);

    // ピース（未保持なら何も描かない）
    drawMiniPiece(holdType, cx, cy + 12);
}

void Renderer::drawNextPanel(TetrominoType nextType) {
    int panelX = BOARD_X + BOARD_W;   // 440
    int panelY = BOARD_Y;
    int cx     = panelX + PANEL_W / 2; // 500
    int cy     = panelY + PANEL_H / 2;

    // パネル背景
    DrawRectangle(panelX, panelY, PANEL_W, PANEL_H, COL_SIDE_BG);
    DrawRectangleLines(panelX, panelY, PANEL_W, PANEL_H, COL_BORDER);

    // ラベル
    const char* label = "ネクスト";
    Vector2 sz = MeasureTextEx(jpFont_, label, 19, 1);
    drawTextOutlined(jpFont_, label,
                     (float)cx - sz.x / 2.0f, (float)(panelY + 8),
                     19, COL_LABEL);

    // ピース
    drawMiniPiece(nextType, cx, cy + 12);
}
