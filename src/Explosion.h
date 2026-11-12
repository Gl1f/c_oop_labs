#pragma once
#include "Figure.h"
#include "Symbol.h"
#include <vector>

// Расширяющееся кольцо, независимое от скорости линии.
class Explosion : public Figure {
private:
    int centerX;
    int centerY;
    int maxRadius;
    int radius = 0;
    ULONGLONG startTime;
    bool finished = false;
    std::vector<Symbol> symbols;
    void createRing(); // Простой контур ромба, как на рисунке ТЗ.

public:
    Explosion(int x, int y, int minRadius, int maxRadius, ULONGLONG birthTime);
    bool update(ULONGLONG now);
    void draw(Console& console) const override;
    bool isFinished() const override;
};
