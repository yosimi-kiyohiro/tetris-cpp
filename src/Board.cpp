#include "Board.hpp"
#include <algorithm>

Board::Board() : bagIdx_(0), rng_(std::random_device{}()) {
    for (int r = 0; r < BOARD_ROWS; r++)
        for (int c = 0; c < BOARD_COLS; c++)
            grid_[r][c] = TetrominoType::None;
    refillBag();
}

TetrominoType Board::cell(int row, int col) const {
    return grid_[row][col];
}

bool Board::isValid(const Tetromino& t) const {
    for (int r = 0; r < (int)t.shape.size(); r++) {
        for (int c = 0; c < (int)t.shape[r].size(); c++) {
            if (!t.shape[r][c]) continue;
            int br = t.y + r;
            int bc = t.x + c;
            if (bc < 0 || bc >= BOARD_COLS || br >= BOARD_ROWS) return false;
            // br < 0 は盤面上の見えない領域（スポーン直後）なので許容する
            if (br >= 0 && grid_[br][bc] != TetrominoType::None) return false;
        }
    }
    return true;
}

void Board::lock(const Tetromino& t) {
    for (int r = 0; r < (int)t.shape.size(); r++) {
        for (int c = 0; c < (int)t.shape[r].size(); c++) {
            if (!t.shape[r][c]) continue;
            int br = t.y + r;
            int bc = t.x + c;
            if (br >= 0 && br < BOARD_ROWS && bc >= 0 && bc < BOARD_COLS)
                grid_[br][bc] = t.type;
        }
    }
}

int Board::clearLines() {
    int cleared = 0;
    // 下から上へスキャンする。上から消すと削除後のシフトで未チェック行が生まれるため、
    // 下から消すことで「詰めた後の同じ行インデックス」を正しく再チェックできる。
    for (int r = BOARD_ROWS - 1; r >= 0; ) {
        bool full = true;
        for (int c = 0; c < BOARD_COLS; c++) {
            if (grid_[r][c] == TetrominoType::None) { full = false; break; }
        }
        if (full) {
            // r より上の行を1段ずつ下へずらす
            for (int rr = r; rr > 0; rr--)
                for (int c = 0; c < BOARD_COLS; c++)
                    grid_[rr][c] = grid_[rr - 1][c];
            // 最上行を空にする
            for (int c = 0; c < BOARD_COLS; c++)
                grid_[0][c] = TetrominoType::None;
            cleared++;
            // r を進めない（シフト後、同じインデックスに新しい行が降りてくる）
        } else {
            r--;
        }
    }
    return cleared;
}

void Board::refillBag() {
    bag_ = { TetrominoType::I, TetrominoType::O, TetrominoType::T,
             TetrominoType::S, TetrominoType::Z, TetrominoType::J, TetrominoType::L };
    std::shuffle(bag_.begin(), bag_.end(), rng_);
    bagIdx_ = 0;
}

TetrominoType Board::nextType() {
    if (bagIdx_ >= bag_.size()) refillBag();
    return bag_[bagIdx_++];
}

TetrominoType Board::peekNext() {
    if (bagIdx_ >= bag_.size()) refillBag();
    return bag_[bagIdx_];  // bagIdx_ を進めない
}
