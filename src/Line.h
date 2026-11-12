#pragma once
#include "Symbol.h"
#include "Figure.h"
#include "Explosion.h"
#include <vector>

// Ромб движется слева направо
class Line : public Figure {
private:
    std::vector<Symbol> symbols; // От хвоста к голове, максимум 30 символов.
    int length;
    int row;
    int width;
    int step = 0;
    int pathLength;
    bool randomColor;
    int speed; // Скорость этой линии в символах в секунду.
    ULONGLONG startTime; // Запланированный момент рождения.
    int probability;
    int minRadius;
    int maxRadius;
    void crawl(); // Меняем голову и хвост на один символ.

public:
    Line(int lineLength, int lineSpeed, bool colorMode,
        const Console& console, ULONGLONG birthTime,
        int explosionProbability, int minExplosionRadius, int maxExplosionRadius);
    bool update(ULONGLONG now, std::vector<Explosion>& explosions);
    void draw(Console& console) const override;
    bool isFinished() const override;
};
