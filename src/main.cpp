#include "raylib.h"

int main() {
    InitWindow(800, 600, "Hello Tetris C++!");
    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        PollInputEvents();
        BeginDrawing();
        ClearBackground(DARKBLUE);
        DrawText("Hello, Tetris C++!", 220, 270, 30, WHITE);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
