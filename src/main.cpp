#include "Renderer.hpp"
#include "AudioManager.hpp"
#include "GameState.hpp"

int main() {
    Renderer     renderer;
    AudioManager audio;
    GameState    game;

    while (!renderer.shouldClose()) {
        float dt = GetFrameTime();

        game.handleInput();
        audio.handleInput();
        game.update(dt);
        renderer.update(dt);
        audio.update(dt);

        // ハードドロップのシェイクトリガーを Renderer に転送
        if (game.consumeShakeTrigger()) renderer.triggerShake();

        // 音声イベントを AudioManager に転送
        audio.play(game.consumeAudioFlags());

        renderer.beginFrame();
        renderer.drawBoard();
        renderer.drawLockedCells(game.board());

        // 演出中（フラッシュ）はピース・ゴーストを非表示にして白く光らせる
        if (game.isStarted() && !game.isGameOver()) {
            if (!game.isAnimating()) {
                renderer.drawGhost(game.calcGhost());
                renderer.drawPiece(game.current());
            }
            if (game.flashAlpha() > 0.0f) {
                renderer.drawFlashOverlay(game.flashAlpha());
            }
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
