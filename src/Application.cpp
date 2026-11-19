#include "Application.h"
#include "Line.h"
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <algorithm>

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
        if (!Console::readText("Frequency (1-30 lines/sec): ", text)) return false;
        if (parseNumber(text, frequency)) break;
        std::cout << "Enter an integer from 1 to 30.\n";
    } while (true);
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
    std::cout << "Matrix - lab 4, student number 11\n"
        "Usage: matrix.exe [frequency speed length Y/N]\n"
        "Frequency: 1-30 lines per second, at random moments.\n"
        "Speed: 1-30 symbols per second. Length: 1-30 symbols.\n"
        "Y: random color for each symbol. N: green.\n"
        "Without arguments: interactive input.\n"
        "Example: matrix.exe 3 23 8 Y\n"
        "Variant: left to right, diamond. Press Esc to exit.\n";
}

void Application::planSecond() {
    spawnTimes.clear();
    for (int index = 0; index < frequency; ++index)
        spawnTimes.pushBack(secondStart + std::rand() % 1001);
    std::sort(spawnTimes.data(), spawnTimes.data() + spawnTimes.size());
    nextSpawn = 0;
}

void Application::spawnLines(ULONGLONG now, const Console& console) {
    // Если кадр задержался, не теряем события предыдущих секунд.
    while (true) {
        while (nextSpawn < spawnTimes.size() && now >= spawnTimes.at(nextSpawn)) {
            ULONGLONG birthTime = spawnTimes.at(nextSpawn);
            lines.pushBack(std::unique_ptr<Line>(new Line(length, speed, colorMode, console, birthTime)));
            ++nextSpawn;
        }
        if (now < secondStart + 1000) break;
        secondStart += 1000;
        planSecond();
    }
}

int Application::run(int argumentCount, char* arguments[]) {
    if (argumentCount == 2 && (std::string(arguments[1]) == "--help"
        || std::string(arguments[1]) == "/?")) {
        printHelp();
        return 0;
    }
    if (argumentCount == 1) {
        if (!readParameters()) return 1;
    } else if (argumentCount != 5 || !parseNumber(arguments[1], frequency)
        || !parseNumber(arguments[2], speed) || !parseNumber(arguments[3], length)
        || !parseMode(arguments[4], colorMode)) {
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
    secondStart = GetTickCount64();
    planSecond();
    while (!console.isExitPressed()) {
        ULONGLONG now = GetTickCount64();
        spawnLines(now, console);
        bool changed = false;
        lines.removeIf([now, &changed](const std::unique_ptr<Line>& line) {
            if (line->update(now)) changed = true;
            return line->isFinished();
        });
        if (changed) {
            // Собираем общий кадр: хвост одной линии не стирает другую.
            console.beginFrame();
            lines.forEach([&console](const std::unique_ptr<Line>& line) {
                line->draw(console);
            });
            console.present();
        }
        console.waitUntil(GetTickCount64() + 1);
    }
    lines.clear();
    return 0;
}
