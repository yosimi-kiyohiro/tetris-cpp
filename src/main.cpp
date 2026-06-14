#include "Renderer.hpp"
#include "GameState.hpp"

int main() {
    Renderer  renderer;
    GameState game;

    while (!renderer.shouldClose()) {
        game.handleInput();
        game.update(GetFrameTime());

        Tetromino ghost = game.calcGhost();

        renderer.beginFrame();
        renderer.drawBoard();
        renderer.drawLockedCells(game.board());
        renderer.drawGhost(ghost);
        renderer.drawPiece(game.current());
        renderer.endFrame();
    }

    return 0;
}
