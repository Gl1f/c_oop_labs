#pragma once
#include "Symbol.h"

// Ромб движется слева направо, вариант для номера 11.
class Line {
private:
    Symbol* symbols; // Массив из заданного количества символов.
    int length;
    int row;
    int width;
    int step = 0;
    int pathLength;
    bool randomColor;
    int speed; // Скорость этой линии в символах в секунду.
    ULONGLONG startTime; // Запланированный момент рождения.
    void crawl(); // Меняем голову и хвост на один символ.

public:
    Line(int lineLength, int lineSpeed, bool colorMode,
        const Console& console, ULONGLONG birthTime);
    ~Line();
    Line(const Line&) = delete; // Массив принадлежит только одной линии.
    Line& operator=(const Line&) = delete;
    bool update(ULONGLONG now); // Выполняем шаги, для которых уже настало время.
    void draw(Console& console) const;
    bool isFinished() const;
};
