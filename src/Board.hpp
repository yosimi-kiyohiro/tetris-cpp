#pragma once
#include "Tetromino.hpp"
#include "constants.hpp"
#include <vector>
#include <random>

class Board {
public:
    Board();

    bool          isValid(const Tetromino& t) const;
    void          lock(const Tetromino& t);
    TetrominoType cell(int row, int col) const;
    TetrominoType nextType();
    TetrominoType peekNext();  // 消費せず次のタイプを覗く

private:
    TetrominoType grid_[BOARD_ROWS][BOARD_COLS];

    std::vector<TetrominoType> bag_;
    size_t                     bagIdx_;
    std::mt19937               rng_;

    void refillBag();
};
