#pragma once
#include "Tetromino.hpp"
#include "constants.hpp"
#include <vector>
#include <random>

class Board {
public:
    Board();

    bool              isValid(const Tetromino& t) const;
    void              lock(const Tetromino& t);
    int               countFullLines() const;     // 消去せずに揃ったライン数だけ返す
    std::vector<int>  getFullLineRows() const;    // 揃った行番号リストを返す（消去前に呼ぶ）
    int               clearLines();               // 揃ったラインを消去して消去数を返す
    TetrominoType     cell(int row, int col) const;
    TetrominoType nextType();
    TetrominoType peekNext();  // 消費せず次のタイプを覗く

private:
    TetrominoType grid_[BOARD_ROWS][BOARD_COLS];

    std::vector<TetrominoType> bag_;
    size_t                     bagIdx_;
    std::mt19937               rng_;

    void refillBag();
};
