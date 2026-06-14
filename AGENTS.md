# Block Drop C++ — Claude への説明書

## プロジェクト概要

JavaScript版テトリス「Block Drop」の C++ 移植学習プロジェクト。
清広さんが C++ のクラス・型・メモリ管理を JS版と対比しながら体験することが目的。

## ビルド環境（重要）

- **コンパイラ**：w64devkit（MinGW-GCC）— `C:\w64devkit\w64devkit\bin\g++.exe`
- **ライブラリ**：raylib 6.0 MinGW版 — `C:\raylib\raylib-6.0_win64_mingw-w64\`
- **CMake・vcpkg は使用しない**（OpenGL 黒画面問題で断念・w64devkit に切替済み）

ビルドコマンド：
```bash
g++ src/main.cpp src/Renderer.cpp src/Tetromino.cpp src/Board.cpp src/GameState.cpp -o tetris.exe \
  -I C:/raylib/raylib-6.0_win64_mingw-w64/include \
  -L C:/raylib/raylib-6.0_win64_mingw-w64/lib \
  -lraylib -lopengl32 -lgdi32 -lwinmm -Wall -Wextra
```

新しい .cpp を追加したら、ビルドコマンドの `src/Renderer.cpp` の隣に追記すること。

## 現在の進捗

| ステップ | 内容 | 状態 |
|---|---|---|
| 0 | 環境構築・Hello窓 | ✅ 完了 |
| 1 | Renderer・日本語フォント・盤面グリッド | ✅ 完了 |
| 2 | Tetromino/Board・7bag・落下 | ✅ 完了 |
| 3a | キー操作・衝突・回転・ゴースト | ✅ 完了 |
| 3b | NEXT/HOLD・ロック遅延 | ⬜ |
| 4 | ライン消去・スコア・ゲームオーバー | ⬜ |
| 5〜9 | 演出・音・パーティクル・Web版 | ⬜ |

## ファイル構成と役割

```
src/
├── constants.hpp      # セルサイズ・ウィンドウサイズ・盤面定数
├── Tetromino.hpp/cpp  # TetrominoType enum・Shape・rotateMatrix・Tetromino構造体
├── Board.hpp/cpp      # int[20][10] グリッド・7bag・isValid・lock
├── Renderer.hpp/cpp   # InitWindow・描画・フォント管理
├── main.cpp           # ゲームループ（落下タイマー）
```

## 設計の決定事項（変更不可）

| 項目 | 決定内容 |
|---|---|
| メモリ管理 | 盤面は `int[20][10]` 生配列・破片は `vector` |
| 回転方式 | 毎回計算（`rotateMatrix`）・SRS非準拠 |
| ピース保持 | `std::optional<Tetromino>` |
| フォント | NotoSansJP.ttf（`assets/fonts/`） |
| BGM | OFF 開始・M キーで切替 |
| 効果音 | ON 低音量・S キーで切替 |
| スマホ | 非対応（PC専用） |

## よく使うコマンド

```bash
# 【一番簡単】ビルド＆起動を一発で（w64devkit から）
sh scripts/build.sh

# ビルドのみ（新ファイルを追加した場合は src/XXX.cpp を追記）
g++ src/main.cpp src/Renderer.cpp src/Tetromino.cpp src/Board.cpp src/GameState.cpp -o tetris.exe \
  -I C:/raylib/raylib-6.0_win64_mingw-w64/include \
  -L C:/raylib/raylib-6.0_win64_mingw-w64/lib \
  -lraylib -lopengl32 -lgdi32 -lwinmm -Wall -Wextra

# 実行（tetris-cpp フォルダ内から）
./tetris.exe
```

## 注意事項

- `tetris.exe` は `.gitignore` で除外済み（コミットしない）
- `assets/fonts/NotoSansJP.ttf` はコミット済み（9.2MB）
- プロジェクトノート：`Obsidian safe/permanent-note/テトリスゲームプロジェクト.md`
