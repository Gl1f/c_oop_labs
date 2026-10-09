#pragma once
#include <windows.h>
#include <string>

// Работа с окном консоли и вводом.
class Console {
private:
    HANDLE output; // Буфер вывода.
    CONSOLE_SCREEN_BUFFER_INFO originalInfo{};
    CONSOLE_CURSOR_INFO originalCursor{};
    bool ready = false;

public:
    Console(); // Запоминаем настройки консоли.
    ~Console(); // Возвращаем цвет и курсор.
    Console(const Console&) = delete;
    Console& operator=(const Console&) = delete;
    bool isReady() const;
    int getWidth() const;
    int getHeight() const;
    void clear(); // Очищаем видимую часть окна.
    void draw(int x, int y, char value, WORD color);
    bool isExitPressed() const; // Выход по Escape.
    void waitUntil(ULONGLONG deadline) const;
    static bool readText(const std::string& prompt, std::string& text);
};
