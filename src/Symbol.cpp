#include "Symbol.h"
#include <cstdlib>

void Symbol::create(int newX, int newY, bool randomColor) {
    x = newX;
    y = newY;
    value = static_cast<char>(33 + std::rand() % 94); // Печатные ASCII-символы.
    color = static_cast<WORD>(randomColor ? 1 + std::rand() % 15 : 10);
    visible = true;
}

void Symbol::draw(Console& console) const {
    if (visible) console.draw(x, y, value, color);
}

void Symbol::erase(Console& console) {
    if (visible) console.draw(x, y, ' ', 0);
    visible = false;
}
