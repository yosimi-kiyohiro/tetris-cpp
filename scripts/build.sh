#!/bin/sh
# ビルド＆起動スクリプト
# w64devkit で実行：sh scripts/build.sh

# w64devkit の PATH を通す
export PATH="/c/w64devkit/w64devkit/bin:$PATH"

# プロジェクトルートに移動
cd "$(dirname "$0")/.." || exit 1

echo "ビルド中..."
g++ src/main.cpp src/Renderer.cpp src/Tetromino.cpp src/Board.cpp src/GameState.cpp src/ScoreFile.cpp src/AudioManager.cpp \
    -o tetris.exe \
    -I C:/raylib/raylib-6.0_win64_mingw-w64/include \
    -L C:/raylib/raylib-6.0_win64_mingw-w64/lib \
    -lraylib -lopengl32 -lgdi32 -lwinmm \
    -std=c++17 -Wall -Wextra

if [ $? -eq 0 ]; then
    echo "ビルド成功！起動します..."
    ./tetris.exe
else
    echo "ビルド失敗。エラーを確認してください。"
fi
