#pragma once
#include <string>

// Получает параметры и запускает линии по очереди.
class Application {
private:
    int speed = 1;
    int length = 1;
    bool colorMode = false;
    static bool parseNumber(const std::string& text, int& value);
    static bool parseMode(const std::string& text, bool& value);
    bool readParameters(); // Диалог с повторным вводом при ошибке.
    static void printHelp();

public:
    int run(int argumentCount, char* arguments[]);
};
