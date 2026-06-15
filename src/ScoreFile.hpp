#pragma once

struct SaveData {
    int highScore  = 0;
    int savedLevel = 1;
};

class ScoreFile {
public:
    static SaveData load();
    static void     save(int highScore, int savedLevel);
    static void     clearAll();
    static void     clearLevel();
};
