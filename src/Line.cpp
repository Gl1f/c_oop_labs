#include "Line.h"
#include <cstdlib>

Line::Line(int lineLength, int lineSpeed, bool colorMode,
    const Console& console, ULONGLONG birthTime,
    int explosionProbability, int minExplosionRadius, int maxExplosionRadius)
    : length(lineLength),
      row(1 + std::rand() % (console.getHeight() - 2)),
      width(console.getWidth()), randomColor(colorMode),
      speed(lineSpeed), startTime(birthTime), probability(explosionProbability),
      minRadius(minExplosionRadius), maxRadius(maxExplosionRadius) {
    // В чётном столбце один символ, в нечётном два: верх и низ ромба.
    pathLength = (width / 2) * 3 + width % 2;
    symbols.reserve(lineLength);
}

void Line::crawl() {
    if (!symbols.empty() && (static_cast<int>(symbols.size()) >= length || step >= pathLength))
        symbols.erase(symbols.begin());
    if (step < pathLength) {
        Symbol symbol;
        int x = (step / 3) * 2;
        int y = row;
        if (step % 3 == 1) { ++x; --y; }
        if (step % 3 == 2) { ++x; ++y; }
        symbol.create(x, y, randomColor);
        symbols.push_back(symbol);
    }
    ++step;
}

bool Line::update(ULONGLONG now, std::vector<Explosion>& explosions) {
    bool changed = false;
    if (now < startTime) return false;
    // Первый символ появляется при рождении, дальше шаги по собственной скорости.
    while (!isFinished() && now >= startTime + static_cast<ULONGLONG>(step) * 1000 / speed) {
        ULONGLONG stepTime = startTime + static_cast<ULONGLONG>(step) * 1000 / speed;
        crawl();
        if (!symbols.empty() && std::rand() % 1000 < probability) {
            // Первый по направлению движения символ — голова линии.
            explosions.emplace_back(symbols.back().getX(), symbols.back().getY(),
                minRadius, maxRadius, stepTime);
            --length;
            if (static_cast<int>(symbols.size()) > length) symbols.erase(symbols.begin());
        }
        changed = true;
    }
    return changed;
}

void Line::draw(Console& console) const {
    for (const Symbol& symbol : symbols) symbol.draw(console);
}

bool Line::isFinished() const { return length == 0 || (step >= pathLength && symbols.empty()); }
