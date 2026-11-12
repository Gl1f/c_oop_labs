#pragma once
#include <string>
#include <memory>
#include <vector>
#include "Line.h"

// Получает параметры и координирует линии без дополнительных потоков.
class Application {
private:
    int frequency = 1;
    int speed = 1;
    int length = 1;
    bool colorMode = false;
    int probability = 1;
    int minRadius = 1;
    int maxRadius = 10;
    static bool parseNumber(const std::string& text, int& value, int minValue = 1, int maxValue = 30);
    static bool readNumber(const std::string& prompt, int& value, int minValue, int maxValue);
    static bool parseMode(const std::string& text, bool& value);
    bool readParameters(); // Диалог с повторным вводом при ошибке.
    static void printHelp();
    ULONGLONG secondStart = 0;
    int spawnTimes[30]{}; // Случайные смещения от начала секунды.
    int nextSpawn = 0;
    std::vector<std::unique_ptr<Line>> lines; // Владеем активными линиями.
    std::vector<Explosion> explosions;
    void drawFigure(const Figure& figure, Console& console) const;
    void planSecond();
    void spawnLines(ULONGLONG now, const Console& console);

public:
    int run(int argumentCount, char* arguments[]);
};
