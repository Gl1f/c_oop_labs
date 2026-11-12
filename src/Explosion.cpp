#include "Explosion.h"
#include <cstdlib>

Explosion::Explosion(int x, int y, int minRadius, int maxRadius,
    ULONGLONG birthTime)
    : centerX(x), centerY(y),
      maxRadius(minRadius + std::rand() % (maxRadius - minRadius + 1)),
      startTime(birthTime) {
}

void Explosion::createRing() {
    symbols.clear();
    for (int offsetY = -radius; offsetY <= radius; ++offsetY) {
        int offsetX = radius - std::abs(offsetY);
        Symbol symbol;
        symbol.create(centerX + offsetX, centerY + offsetY, true);
        symbols.push_back(symbol);
        if (offsetX != 0) {
            symbol.create(centerX - offsetX, centerY + offsetY, true);
            symbols.push_back(symbol);
        }
    }
}

bool Explosion::update(ULONGLONG now) {
    if (finished || now < startTime) return false;
    // На каждом радиусе остаёмся 500 мс: скорость 2 клетки в секунду.
    ULONGLONG stage = (now - startTime) / 500;
    if (stage >= static_cast<ULONGLONG>(maxRadius)) {
        symbols.clear();
        finished = true;
        return true;
    }
    int newRadius = 1 + static_cast<int>(stage);
    if (newRadius == radius) return false;
    radius = newRadius;
    createRing(); // На каждой стадии новые случайные знаки и цвета.
    return true;
}

void Explosion::draw(Console& console) const {
    for (const Symbol& symbol : symbols) symbol.draw(console);
}

bool Explosion::isFinished() const { return finished; }
