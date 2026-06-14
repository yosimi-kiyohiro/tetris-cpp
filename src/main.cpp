#include "Renderer.hpp"

int main() {
    Renderer renderer;

    while (!renderer.shouldClose()) {
        renderer.beginFrame();
        renderer.drawBoard();
        renderer.endFrame();
    }

    return 0;
}
