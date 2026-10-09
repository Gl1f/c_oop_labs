#include "Application.h"
#include "Line.h"
#include <cstdlib>
#include <ctime>
#include <iostream>

bool Application::parseNumber(const std::string& text, int& value) {
    if (text.empty()) return false;
    int number = 0;
    for (char digit : text) {
        if (digit < '0' || digit > '9') return false;
        number = number * 10 + digit - '0';
        if (number > 30) return false; // Заодно исключаем переполнение.
    }
    if (number < 1) return false;
    value = number;
    return true;
}

bool Application::parseMode(const std::string& text, bool& value) {
    if (text == "Y" || text == "y") { value = true; return true; }
    if (text == "N" || text == "n") { value = false; return true; }
    return false;
}

bool Application::readParameters() {
    std::string text;
    do {
        if (!Console::readText("Speed (1-30 symbols/sec): ", text)) return false;
        if (parseNumber(text, speed)) break;
        std::cout << "Enter an integer from 1 to 30.\n";
    } while (true);
    do {
        if (!Console::readText("Length (1-30 symbols): ", text)) return false;
        if (parseNumber(text, length)) break;
        std::cout << "Enter an integer from 1 to 30.\n";
    } while (true);
    do {
        if (!Console::readText("Epilepsy mode (Y/N): ", text)) return false;
        if (parseMode(text, colorMode)) break;
        std::cout << "Enter Y or N.\n";
    } while (true);
    return true;
}

void Application::printHelp() {
    std::cout << "Matrix - lab 1, student number 11\n"
        "Usage: matrix.exe [speed length Y/N]\n"
        "Speed: 1-30 symbols per second. Length: 1-30 symbols.\n"
        "Y: random color for each symbol. N: green.\n"
        "Without arguments: interactive input.\n"
        "Example: matrix.exe 23 8 Y\n"
        "Variant: left to right, diamond. Press Esc to exit.\n";
}

int Application::run(int argumentCount, char* arguments[]) {
    if (argumentCount == 2 && (std::string(arguments[1]) == "--help"
        || std::string(arguments[1]) == "/?")) {
        printHelp();
        return 0;
    }
    if (argumentCount == 1) {
        if (!readParameters()) return 1;
    } else if (argumentCount != 4 || !parseNumber(arguments[1], speed)
        || !parseNumber(arguments[2], length) || !parseMode(arguments[3], colorMode)) {
        std::cerr << "Invalid parameters. Use matrix.exe --help.\n";
        return 1;
    }

    Console console;
    if (!console.isReady() || console.getHeight() < 3) {
        std::cerr << "Run the program in a console window at least 3 rows high.\n";
        return 1;
    }
    std::srand(static_cast<unsigned>(std::time(nullptr)));
    console.clear();
    while (!console.isExitPressed()) {
        Line line(length, colorMode, console);
        ULONGLONG start = GetTickCount64();
        ULONGLONG steps = 0;
        while (!line.isFinished() && !console.isExitPressed()) {
            line.crawl(console);
            ++steps;
            // Считаем срок от начала, чтобы округление задержек не копилось.
            console.waitUntil(start + steps * 1000 / speed);
        }
    }
    return 0;
}
