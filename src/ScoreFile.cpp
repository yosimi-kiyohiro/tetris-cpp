#include "ScoreFile.hpp"
#include "raylib.h"
#include <fstream>
#include <string>

static std::string scorePath() {
    return std::string(GetApplicationDirectory()) + "highscore.txt";
}

SaveData ScoreFile::load() {
    SaveData d;
    std::ifstream f(scorePath());
    if (f) f >> d.highScore >> d.savedLevel;
    return d;
}

void ScoreFile::save(int highScore, int savedLevel) {
    std::ofstream f(scorePath());
    if (f) f << highScore << " " << savedLevel;
}

void ScoreFile::clearAll() {
    // ファイルを空（0 1）で上書き
    std::ofstream f(scorePath());
    if (f) f << "0 1";
}

void ScoreFile::clearLevel() {
    // ハイスコアを保持したままレベルだけ 1 に戻す
    SaveData d = load();
    save(d.highScore, 1);
}
