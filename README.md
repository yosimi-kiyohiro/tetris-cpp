# Block Drop C++

JavaScript版「Block Drop」の C++ 移植版。raylib を使ったネイティブアプリ。

## 環境

| 項目 | 内容 |
|---|---|
| コンパイラ | w64devkit（MinGW-GCC 16.1.0） |
| ライブラリ | raylib 6.0（MinGW版）|
| 言語 | C++17 |
| 対象 | Windows PC専用（スマホ非対応） |

## ビルド方法

w64devkit のターミナルで実行：

```bash
cd ~/Projects/tetris-cpp
g++ src/main.cpp src/Renderer.cpp -o tetris.exe \
  -I C:/raylib/raylib-6.0_win64_mingw-w64/include \
  -L C:/raylib/raylib-6.0_win64_mingw-w64/lib \
  -lraylib -lopengl32 -lgdi32 -lwinmm \
  -Wall -Wextra
```

## 実行方法

```bash
./tetris.exe
```

> ※ 実行は `tetris-cpp/` フォルダ内から行うこと（フォント読込のパスが相対パスのため）

## 操作方法

| キー | 動作 |
|---|---|
| ← → | 左右移動 |
| ↑ | 回転 |
| ↓ | ソフトドロップ |
| スペース | ハードドロップ |
| C | ホールド |
| P / Esc | 一時停止 ⇔ 再開 |
| M | BGM ON/OFF |
| S | 効果音 ON/OFF |

## ファイル構成

```
tetris-cpp/
├── src/
│   ├── main.cpp          # エントリーポイント
│   ├── constants.hpp     # 定数定義
│   ├── Renderer.hpp/cpp  # 画面描画クラス
│   ├── Tetromino.hpp/cpp # テトリミノクラス（ステップ2〜）
│   ├── Board.hpp/cpp     # 盤面クラス（ステップ2〜）
│   └── GameState.hpp/cpp # ゲーム状態クラス（ステップ3〜）
├── assets/
│   ├── fonts/            # 日本語フォント（NotoSansJP）
│   └── sounds/           # 効果音・BGM（ステップ6〜）
├── .gitignore
└── README.md
```

## 注意事項

- 回転は簡易ウォールキック（SRS 非準拠）
- スマホ・タッチ操作は非対応（JS版を使用してください）

## 関連リンク

- JS版（遊べる）: https://yosimi-kiyohiro.github.io/block-drop-js/
- JS版リポジトリ: https://github.com/yosimi-kiyohiro/block-drop-js
