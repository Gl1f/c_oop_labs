#include "Application.h"
#include "Line.h"
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <algorithm>

bool Application::parseNumber(const std::string& text, int& value, int minValue, int maxValue) {
    if (text.empty()) return false;
    int number = 0;
    for (char digit : text) {
        if (digit < '0' || digit > '9') return false;
        number = number * 10 + digit - '0';
        if (number > maxValue) return false; // Заодно исключаем переполнение.
    }
    if (number < minValue) return false;
    value = number;
    return true;
}

bool Application::readNumber(const std::string& prompt, int& value, int minValue, int maxValue) {
    std::string text;
    while (Console::readText(prompt, text)) {
        if (parseNumber(text, value, minValue, maxValue)) return true;
        std::cout << "Enter an integer from " << minValue << " to " << maxValue << ".\n";
    }
    return false;
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
    if (!readNumber("Explosion probability (1-1000): ", probability, 1, 1000)) return false;
    if (!readNumber("Minimum radius (1-10): ", minRadius, 1, 10)) return false;
    if (!readNumber("Maximum radius (minimum-10): ", maxRadius, minRadius, 10)) return false;
    return true;
}

void Application::printHelp() {
    std::cout << "Matrix - lab 3, student number 11\n"
        "Usage: matrix.exe [frequency speed length Y/N probability minRadius maxRadius]\n"
        "Frequency: 1-30 lines per second, at random moments.\n"
        "Speed: 1-30 symbols per second. Length: 1-30 symbols.\n"
        "Y: random color for each symbol. N: green.\n"
        "Without arguments: interactive input.\n"
        "Probability: 1-1000 chances out of 1000 per line step.\n"
        "Radii: 1-10, maxRadius must be >= minRadius.\n"
        "Explosions expand at 2 cells/sec, always with random colors.\n"
        "Example: matrix.exe 3 23 8 Y 50 2 5\n"
        "Variant: left to right, diamond. Press Esc to exit.\n";
}

void Application::planSecond() {
    for (int index = 0; index < frequency; ++index)
        spawnTimes[index] = std::rand() % 1001;
    std::sort(spawnTimes, spawnTimes + frequency);
    nextSpawn = 0;
}

void Application::spawnLines(ULONGLONG now, const Console& console) {
    // Если кадр задержался, не теряем события предыдущих секунд.
    while (true) {
        while (nextSpawn < frequency && now >= secondStart + spawnTimes[nextSpawn]) {
            ULONGLONG birthTime = secondStart + spawnTimes[nextSpawn];
            lines.emplace_back(new Line(length, speed, colorMode, console, birthTime,
                probability, minRadius, maxRadius));
            ++nextSpawn;
        }
        if (now < secondStart + 1000) break;
        secondStart += 1000;
        planSecond();
    }
}

void Application::drawFigure(const Figure& figure, Console& console) const {
    figure.draw(console); // Виртуальный вызов: Line или Explosion.
}

int Application::run(int argumentCount, char* arguments[]) {
    if (argumentCount == 2 && (std::string(arguments[1]) == "--help"
        || std::string(arguments[1]) == "/?")) {
        printHelp();
        return 0;
    }
    if (argumentCount == 1) {
        if (!readParameters()) return 1;
    } else if (argumentCount != 8 || !parseNumber(arguments[1], frequency)
        || !parseNumber(arguments[2], speed) || !parseNumber(arguments[3], length)
        || !parseMode(arguments[4], colorMode)
        || !parseNumber(arguments[5], probability, 1, 1000)
        || !parseNumber(arguments[6], minRadius, 1, 10)
        || !parseNumber(arguments[7], maxRadius, minRadius, 10)) {
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
        for (auto line = lines.begin(); line != lines.end();) {
            if ((*line)->update(now, explosions)) changed = true;
            if ((*line)->isFinished()) line = lines.erase(line);
            else ++line;
        }
        for (auto explosion = explosions.begin(); explosion != explosions.end();) {
            if (explosion->update(now)) changed = true;
            if (explosion->isFinished()) explosion = explosions.erase(explosion);
            else ++explosion;
        }
        if (changed) {
            // Собираем общий кадр: хвост одной линии не стирает другую.
            console.beginFrame();
            for (const auto& line : lines) drawFigure(*line, console);
            for (const Explosion& explosion : explosions) drawFigure(explosion, console);
            console.present();
        }
        console.waitUntil(GetTickCount64() + 1);
    }
    lines.clear();
    explosions.clear();
    return 0;
}
