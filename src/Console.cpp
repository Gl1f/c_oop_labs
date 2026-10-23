#include "Console.h"
#include <iostream>

Console::Console() : output(GetStdHandle(STD_OUTPUT_HANDLE)) {
    ready = GetConsoleScreenBufferInfo(output, &originalInfo)
        && GetConsoleCursorInfo(output, &originalCursor);
    if (ready) {
        frame.resize(getWidth() * getHeight());
        CONSOLE_CURSOR_INFO cursor = originalCursor;
        cursor.bVisible = FALSE;
        SetConsoleCursorInfo(output, &cursor);
    }
}

Console::~Console() {
    if (ready) {
        clear();
        SetConsoleTextAttribute(output, originalInfo.wAttributes);
        SetConsoleCursorInfo(output, &originalCursor);
        SetConsoleCursorPosition(output,
            {originalInfo.srWindow.Left, originalInfo.srWindow.Top});
    }
}

bool Console::isReady() const { return ready; }

int Console::getWidth() const {
    return originalInfo.srWindow.Right - originalInfo.srWindow.Left + 1;
}

int Console::getHeight() const {
    return originalInfo.srWindow.Bottom - originalInfo.srWindow.Top + 1;
}

void Console::clear() {
    for (int y = 0; y < getHeight(); ++y) {
        COORD position = {originalInfo.srWindow.Left,
            static_cast<SHORT>(originalInfo.srWindow.Top + y)};
        DWORD written;
        FillConsoleOutputCharacterA(output, ' ', getWidth(), position, &written);
        FillConsoleOutputAttribute(output, originalInfo.wAttributes,
            getWidth(), position, &written);
    }
}

void Console::draw(int x, int y, char value, WORD color) {
    if (x < 0 || x >= getWidth() || y < 0 || y >= getHeight()) return;
    CHAR_INFO& cell = frame[y * getWidth() + x];
    cell.Char.AsciiChar = value;
    cell.Attributes = color;
}

void Console::beginFrame() {
    for (CHAR_INFO& cell : frame) {
        cell.Char.AsciiChar = ' ';
        cell.Attributes = originalInfo.wAttributes;
    }
}

void Console::present() {
    COORD size = {static_cast<SHORT>(getWidth()), static_cast<SHORT>(getHeight())};
    COORD origin = {0, 0};
    SMALL_RECT area = originalInfo.srWindow;
    WriteConsoleOutputA(output, frame.data(), size, origin, &area);
}

bool Console::isExitPressed() const {
    return (GetAsyncKeyState(VK_ESCAPE) & 0x8000) != 0;
}

void Console::waitUntil(ULONGLONG deadline) const {
    while (!isExitPressed() && GetTickCount64() < deadline) Sleep(1);
}

bool Console::readText(const std::string& prompt, std::string& text) {
    std::cout << prompt;
    return static_cast<bool>(std::getline(std::cin, text));
}
