#pragma once
#include "List.h"
#include <memory>
#include <stdexcept>
#include <utility>

// Элементы хранятся подряд. При заполнении выделяем массив побольше.
// Для этой реализации T должен иметь конструктор по умолчанию и поддерживать присваивание.
template <typename T>
class ArrayList : public List<T> {
private:
    std::unique_ptr<T[]> elements;
    int count = 0;
    int capacity = 0;

    void checkIndex(int position) const {
        if (position < 0 || position >= count) throw std::out_of_range("ArrayList index");
    }

public:
    ArrayList() = default;
    ArrayList(const ArrayList&) = delete;
    ArrayList& operator=(const ArrayList&) = delete;

    void reserve(int newCapacity) {
        if (newCapacity <= capacity) return;
        std::unique_ptr<T[]> larger(new T[newCapacity]{});
        for (int index = 0; index < count; ++index)
            larger[index] = std::move(elements[index]);
        elements = std::move(larger);
        capacity = newCapacity;
    }

    void pushBack(T value) override { insert(count, std::move(value)); }

    void insert(int position, T value) override {
        if (position < 0 || position > count) throw std::out_of_range("ArrayList insert index");
        if (count == capacity) reserve(capacity == 0 ? 4 : capacity * 2);
        for (int index = count; index > position; --index)
            elements[index] = std::move(elements[index - 1]);
        elements[position] = std::move(value);
        ++count;
    }

    void remove(int position) override {
        checkIndex(position);
        for (int index = position; index < count - 1; ++index)
            elements[index] = std::move(elements[index + 1]);
        elements[count - 1] = T{};
        --count;
    }

    int size() const override { return count; }
    T& at(int position) override { checkIndex(position); return elements[position]; }
    const T& at(int position) const override { checkIndex(position); return elements[position]; }

    void clear() {
        while (count > 0) remove(count - 1);
    }

    T* data() { return elements.get(); }
};
