#pragma once

// セル・盤面
constexpr int CELL_SIZE  = 32;
constexpr int BOARD_COLS = 10;
constexpr int BOARD_ROWS = 20;
constexpr int BOARD_W    = CELL_SIZE * BOARD_COLS;  // 320
constexpr int BOARD_H    = CELL_SIZE * BOARD_ROWS;  // 640

// レイアウト
constexpr int PANEL_W    = 120;  // HOLD / NEXT パネル幅
constexpr int TOP_H      = 70;   // スコア表示エリア
constexpr int BOTTOM_H   = 16;   // 下余白

// ウィンドウ全体
constexpr int WIN_W      = PANEL_W + BOARD_W + PANEL_W;  // 560
constexpr int WIN_H      = TOP_H + BOARD_H + BOTTOM_H;   // 704

// 盤面の左上座標（ウィンドウ内）
constexpr int BOARD_X    = PANEL_W;
constexpr int BOARD_Y    = TOP_H;

// フレームレート
constexpr int TARGET_FPS = 60;
