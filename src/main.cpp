#include "Renderer.hpp"
#include "AudioManager.hpp"
#include "GameState.hpp"
#include "ParticleSystem.hpp"
#include "Tetromino.hpp"

#if defined(PLATFORM_WEB)
#include <emscripten/emscripten.h>
#endif

static Renderer*       g_renderer  = nullptr;
static AudioManager*   g_audio     = nullptr;
static GameState*      g_game      = nullptr;
static ParticleSystem* g_particles = nullptr;

static void UpdateDrawFrame() {
    float dt = GetFrameTime();

    g_game->handleInput();
    g_audio->handleInput();
    g_game->update(dt);
    g_renderer->update(dt);
    g_audio->update(dt);

    if (g_game->consumeShakeTrigger()) g_renderer->triggerShake();
    g_audio->play(g_game->consumeAudioFlags());

    for (auto& e : g_game->consumeParticleEmits())
        g_particles->emit(e.col, e.row, typeToColor(e.type));
    g_particles->update(dt);

    g_renderer->beginFrame();
    g_renderer->drawBoard();
    g_renderer->drawLockedCells(g_game->board());
    g_particles->draw();

    if (g_game->isStarted() && !g_game->isGameOver()) {
        if (!g_game->isAnimating()) {
            g_renderer->drawGhost(g_game->calcGhost());
            g_renderer->drawPiece(g_game->current());
        }
        if (g_game->flashAlpha() > 0.0f) {
            g_renderer->drawFlashOverlay(g_game->flashAlpha());
        }
    }

    g_renderer->drawHoldPanel(g_game->holdType());
    g_renderer->drawNextPanel(g_game->nextPeek());
    g_renderer->drawScorePanel(g_game->score(), g_game->highScore(),
                               g_game->level(), g_game->linesCleared());

    if (!g_game->isStarted())           g_renderer->drawStartOverlay(g_game->savedLevel());
    else if (g_game->isPaused())        g_renderer->drawPauseOverlay();
    else if (g_game->isGameOver())      g_renderer->drawGameOverOverlay(g_game->savedLevel());

    g_renderer->endFrame();
}

int main() {
    g_renderer  = new Renderer();
    g_audio     = new AudioManager();
    g_game      = new GameState();
    g_particles = new ParticleSystem();

#if defined(PLATFORM_WEB)
    emscripten_set_main_loop(UpdateDrawFrame, 0, 1);
#else
    while (!g_renderer->shouldClose()) {
        UpdateDrawFrame();
    }
    delete g_renderer;
    delete g_audio;
    delete g_game;
    delete g_particles;
#endif

    return 0;
}
