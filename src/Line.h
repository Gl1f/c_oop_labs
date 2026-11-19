#pragma once
#include "Symbol.h"
#include "LinkedList.h"

// Ромб движется слева направо
class Line {
private:
    LinkedList<Symbol> symbols; // От хвоста к голове.
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
    bool update(ULONGLONG now); // Выполняем шаги, для которых уже настало время.
    void draw(Console& console) const;
    bool isFinished() const;
};
