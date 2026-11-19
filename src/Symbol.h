#pragma once
#include "Console.h"

// Символ хранит свою позицию и цвет. После появления он не сдвигается.
class Symbol {
private:
    int x = 0;
    int y = 0;
    char value = ' ';
    WORD color = 10;

public:
    void create(int newX, int newY, bool randomColor);
    void draw(Console& console) const;
};
