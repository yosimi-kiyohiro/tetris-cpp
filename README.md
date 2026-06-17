# Block Drop C++

JavaScript版「Block Drop」の C++ 移植版。raylib を使ったネイティブアプリ。

- JS版（遊べる）: https://yosimi-kiyohiro.github.io/block-drop-js/

## 環境

| 項目 | 内容 |
|---|---|
| コンパイラ | w64devkit（MinGW-GCC 16.1.0） |
| ライブラリ | raylib 6.0（MinGW版） |
| 言語 | C++17 |
| 対象 | Windows PC専用（スマホ非対応） |

## ビルド＆実行

w64devkit のターミナルで実行：

```bash
cd /path/to/tetris-cpp   # プロジェクトフォルダに移動
sh scripts/build.sh
```

手動ビルド（src に新ファイルを追加した時は build.sh も更新すること）：

```bash
g++ src/main.cpp src/Renderer.cpp src/Tetromino.cpp src/Board.cpp \
    src/GameState.cpp src/ScoreFile.cpp src/AudioManager.cpp src/ParticleSystem.cpp \
    -o tetris.exe \
    -I C:/raylib/raylib-6.0_win64_mingw-w64/include \
    -L C:/raylib/raylib-6.0_win64_mingw-w64/lib \
    -lraylib -lopengl32 -lgdi32 -lwinmm \
    -std=c++17 -Wall -Wextra
./tetris.exe
```

> ※ 実行は `tetris-cpp/` フォルダ内から行うこと（フォント・音源のパスが相対パスのため）

## 操作方法

| キー | 動作 |
|---|---|
| ← → | 左右移動 |
| ↑ | 回転（簡易ウォールキック・SRS非準拠） |
| ↓ | ソフトドロップ |
| スペース | ハードドロップ |
| C | ホールド（1ターン1回） |
| P / Esc | 一時停止 ⇔ 再開 |
| M | BGM ON/OFF（デフォルト OFF） |
| S | 効果音 ON/OFF（デフォルト ON） |
| N | ニューゲーム（Lv.1 から） |
| R / スペース | リスタート（前回レベルから） |
| A | 全クリア（ハイスコア＋レベルをリセット） |
| L | レベルクリア（レベルだけリセット） |

## スコア

| ライン消去 | 点数 |
|---|---|
| 1列 | 100点 |
| 2列 | 300点 |
| 3列 | 500点 |
| 4列（テトリス） | 800点 |

10ラインごとにレベルアップ・落下が加速（最速100ms間隔）。

## 注意事項

- 回転は簡易ウォールキック（SRS 非準拠）
- スマホ・タッチ操作は非対応（JS版を利用してください）
- ハイスコアと最高レベルは `highscore.txt` に自動保存（.gitignore で除外済み）

## ファイル構成

```
tetris-cpp/
├── src/
│   ├── main.cpp               # エントリーポイント・ゲームループ
│   ├── constants.hpp          # 定数定義（セルサイズ・盤面サイズ等）
│   ├── Tetromino.hpp/cpp      # テトリミノ（形・色・生成・回転）
│   ├── Board.hpp/cpp          # 盤面クラス（衝突・消去・7-bag）
│   ├── GameState.hpp/cpp      # ゲーム状態・入力・演出フェーズ管理
│   ├── Renderer.hpp/cpp       # 画面描画クラス（raylib ラッパー）
│   ├── ScoreFile.hpp/cpp      # ハイスコア・レベル保存（ファイルI/O）
│   ├── AudioManager.hpp/cpp   # 効果音・BGM（RAII パターン）
│   └── ParticleSystem.hpp/cpp # パーティクル破片演出
├── assets/
│   ├── fonts/                 # 日本語フォント（NotoSansJP）
│   └── sounds/                # 効果音・BGM（CC0フリー素材）
├── scripts/
│   └── build.sh               # ビルド＆実行スクリプト
├── .gitignore
└── README.md

```

## 音源クレジット（CC0）

- 効果音：[Kenney Interface Sounds](https://kenney.nl/assets/interface-sounds)（CC0 Public Domain）
- BGM：[Tallbeard Studios Music Loop Bundle](https://tallbeard.itch.io/music-loop-bundle)（CC0 Public Domain）

## JS版との比較

| 機能 | JavaScript版（Block Drop） | C++版（本リポジトリ） |
|---|---|---|
| 動く場所 | ブラウザ | Windows ネイティブアプリ |
| 実行方式 | インタプリタ（その場で実行） | コンパイル済み（高速） |
| 描画 | Canvas API | raylib |
| スコア保存 | なし | ファイル保存（highscore.txt） |
| 効果音・BGM | なし | あり（raylib AudioDevice） |
| パーティクル演出 | なし（消去フラッシュのみ） | あり（破片が飛び散る） |
| スマホ対応 | あり（タッチボタン） | なし（PC専用） |
| メモリ管理 | 自動（GC） | 手動（RAII パターン） |

## 関連リンク

- JS版リポジトリ：https://github.com/yosimi-kiyohiro/block-drop-js
