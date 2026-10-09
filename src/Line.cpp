#include "Line.h"
#include <cstdlib>

Line::Line(int lineLength, bool colorMode, const Console& console)
    : symbols(new Symbol[lineLength]), length(lineLength),
      row(1 + std::rand() % (console.getHeight() - 2)),
      width(console.getWidth()), randomColor(colorMode) {
    // В чётном столбце один символ, в нечётном два: верх и низ ромба.
    pathLength = (width / 2) * 3 + width % 2;
}

Line::~Line() { delete[] symbols; }

void Line::crawl(Console& console) {
    Symbol& symbol = symbols[step % length];
    symbol.erase(console);
    if (step < pathLength) {
        int x = (step / 3) * 2;
        int y = row;
        if (step % 3 == 1) { ++x; --y; }
        if (step % 3 == 2) { ++x; ++y; }
        symbol.create(x, y, randomColor);
        symbol.draw(console);
    }
    ++step;
}

bool Line::isFinished() const { return step >= pathLength + length; }
