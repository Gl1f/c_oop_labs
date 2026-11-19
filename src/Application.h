#pragma once
#include <string>
#include <memory>
#include "ArrayList.h"
#include "LinkedList.h"
#include "Line.h"

// Получает параметры и координирует линии без дополнительных потоков.
class Application {
private:
    int frequency = 1;
    int speed = 1;
    int length = 1;
    bool colorMode = false;
    static bool parseNumber(const std::string& text, int& value);
    static bool parseMode(const std::string& text, bool& value);
    bool readParameters(); // Диалог с повторным вводом при ошибке.
    static void printHelp();
    ULONGLONG secondStart = 0;
    ArrayList<ULONGLONG> spawnTimes; // Время запуска каждой линии в миллисекундах.
    int nextSpawn = 0;
    LinkedList<std::unique_ptr<Line>> lines; // Владеем активными линиями.
    void planSecond();
    void spawnLines(ULONGLONG now, const Console& console);

public:
    int run(int argumentCount, char* arguments[]);
};
