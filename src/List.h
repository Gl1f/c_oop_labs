#pragma once

// Интерфейс списка. Индексы начинаются с нуля.
template <typename T>
class List {
public:
    virtual ~List() = default;
    virtual void pushBack(T value) = 0;
    virtual void insert(int position, T value) = 0;
    virtual void remove(int position) = 0;
    virtual int size() const = 0;
    virtual T& at(int position) = 0;
    virtual const T& at(int position) const = 0;
};
