#pragma once
#include "Console.h"

// Общий интерфейс фигур. Создать просто Figure нельзя.
class Figure {
public:
    virtual ~Figure() = default;
    virtual void draw(Console& console) const = 0;
    virtual bool isFinished() const = 0;
};
