#include "Line.h"
#include <cstdlib>

Line::Line(int lineLength, int lineSpeed, bool colorMode,
    const Console& console, ULONGLONG birthTime)
    : length(lineLength),
      row(1 + std::rand() % (console.getHeight() - 2)),
      width(console.getWidth()), randomColor(colorMode),
      speed(lineSpeed), startTime(birthTime) {
    // В чётном столбце один символ, в нечётном два: верх и низ ромба.
    pathLength = (width / 2) * 3 + width % 2;
}

void Line::crawl() {
    // При заполнении или при выходе из окна убираем первый символ — хвост.
    if (symbols.size() > 0 && (symbols.size() == length || step >= pathLength))
        symbols.remove(0);
    if (step < pathLength) {
        Symbol symbol;
        int x = (step / 3) * 2;
        int y = row;
        if (step % 3 == 1) { ++x; --y; }
        if (step % 3 == 2) { ++x; ++y; }
        symbol.create(x, y, randomColor);
        symbols.pushBack(symbol);
    }
    ++step;
}

bool Line::update(ULONGLONG now) {
    bool changed = false;
    if (now < startTime) return false;
    // Первый символ появляется при рождении, дальше шаги по собственной скорости.
    while (!isFinished() && now >= startTime + static_cast<ULONGLONG>(step) * 1000 / speed) {
        crawl();
        changed = true;
    }
    return changed;
}

void Line::draw(Console& console) const {
    symbols.forEach([&console](const Symbol& symbol) { symbol.draw(console); });
}

bool Line::isFinished() const { return step >= pathLength && symbols.size() == 0; }
