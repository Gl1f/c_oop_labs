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

public:
    Line(int lineLength, bool colorMode, const Console& console);
    ~Line();
    Line(const Line&) = delete; // Массив принадлежит только одной линии.
    Line& operator=(const Line&) = delete;
    void crawl(Console& console); // Один новый символ и удаление хвоста.
    bool isFinished() const;
};
