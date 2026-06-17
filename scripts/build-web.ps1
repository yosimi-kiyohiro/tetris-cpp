$empp = "C:\Users\nimam\Projects\tetris-cpp\emsdk\upstream\emscripten\em++.bat"

Set-Location "C:\Users\nimam\Projects\tetris-cpp"

& $empp src/main.cpp src/Renderer.cpp src/Tetromino.cpp src/Board.cpp src/GameState.cpp src/ScoreFile.cpp src/AudioManager.cpp src/ParticleSystem.cpp -o docs/index.html -I C:/Users/nimam/Projects/raylib-src/src C:/Users/nimam/Projects/raylib-src/src/libraylib.web.a -s USE_GLFW=3 -s TOTAL_MEMORY=67108864 -s ALLOW_MEMORY_GROWTH=1 -std=c++17 --preload-file assets --shell-file scripts/shell.html -s WASM=1 -DPLATFORM_WEB -Os

if ($LASTEXITCODE -eq 0) {
    Write-Host "Build succeeded!"
} else {
    Write-Host "Build failed."
}
