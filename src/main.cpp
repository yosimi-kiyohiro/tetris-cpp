#include "Renderer.hpp"
#include "GameState.hpp"

int main() {
    Renderer  renderer;
    GameState game;

    while (!renderer.shouldClose()) {
        game.handleInput();
        game.update(GetFrameTime());

        renderer.beginFrame();
        renderer.drawBoard();
        renderer.drawLockedCells(game.board());

        // スタート済みでゲームオーバーでない間だけピース・ゴーストを描く
        if (game.isStarted() && !game.isGameOver()) {
            renderer.drawGhost(game.calcGhost());
            renderer.drawPiece(game.current());
        }

        renderer.drawHoldPanel(game.holdType());
        renderer.drawNextPanel(game.nextPeek());
        renderer.drawScorePanel(game.score(), game.highScore(),
                                game.level(), game.linesCleared());

        if (!game.isStarted())      renderer.drawStartOverlay(game.savedLevel());
        else if (game.isPaused())   renderer.drawPauseOverlay();
        else if (game.isGameOver()) renderer.drawGameOverOverlay(game.savedLevel());

        renderer.endFrame();
    }

    return 0;
}
