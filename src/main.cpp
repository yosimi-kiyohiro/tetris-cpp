#include "Renderer.hpp"
#include "Board.hpp"
#include "Tetromino.hpp"
#include <algorithm>

int main() {
    Renderer renderer;
    Board    board;

    Tetromino current      = Tetromino::spawn(board.nextType());
    float     fallTimer    = 0.0f;
    const float FALL_INTERVAL = 1.0f;  // 1秒ごとに1段落下（Lv1）

    while (!renderer.shouldClose()) {
        float dt = std::min(GetFrameTime(), 0.05f);  // delta上限0.05f
        fallTimer += dt;

        if (fallTimer >= FALL_INTERVAL) {
            fallTimer -= FALL_INTERVAL;
            Tetromino dropped = current;
            dropped.y++;
            if (board.isValid(dropped)) {
                current = dropped;
            } else {
                board.lock(current);
                current    = Tetromino::spawn(board.nextType());
                fallTimer  = 0.0f;
            }
        }

        renderer.beginFrame();
        renderer.drawBoard();
        renderer.drawLockedCells(board);
        renderer.drawPiece(current);
        renderer.endFrame();
    }

    return 0;
}
