#pragma once
#include "List.h"
#include <stdexcept>
#include <utility>

// Каждый узел хранит элемент и указатель на следующий узел.
template <typename T>
class LinkedList : public List<T> {
private:
    struct Node {
        T value;
        Node* next;

        Node(T&& item, Node* following) : value(std::move(item)), next(following) {}
    };

    Node* first = nullptr;
    Node* last = nullptr;
    int count = 0;

    Node* nodeAt(int position) const {
        if (position < 0 || position >= count) throw std::out_of_range("LinkedList index");
        Node* node = first;
        for (int index = 0; index < position; ++index) node = node->next;
        return node;
    }

    // Удаляем узел и возвращаем следующий за ним.
    Node* unlink(Node* previous, Node* node) {
        Node* next = node->next;
        if (previous) previous->next = next;
        else first = next;
        if (node == last) last = previous;
        delete node;
        --count;
        return next;
    }

public:
    LinkedList() = default;
    ~LinkedList() override { clear(); }
    LinkedList(const LinkedList&) = delete;
    LinkedList& operator=(const LinkedList&) = delete;

    void pushBack(T value) override {
        Node* node = new Node(std::move(value), nullptr);
        if (last) last->next = node;
        else first = node;
        last = node;
        ++count;
    }

    void insert(int position, T value) override {
        if (position < 0 || position > count) throw std::out_of_range("LinkedList insert index");
        if (position == count) {
            pushBack(std::move(value));
        } else if (position == 0) {
            first = new Node(std::move(value), first);
            ++count;
        } else {
            Node* previous = nodeAt(position - 1);
            previous->next = new Node(std::move(value), previous->next);
            ++count;
        }
    }

    void remove(int position) override {
        if (position < 0 || position >= count) throw std::out_of_range("LinkedList remove index");
        Node* previous = position == 0 ? nullptr : nodeAt(position - 1);
        unlink(previous, previous ? previous->next : first);
    }

    int size() const override { return count; }
    T& at(int position) override { return nodeAt(position)->value; }
    const T& at(int position) const override { return nodeAt(position)->value; }

    void clear() {
        while (first) unlink(nullptr, first);
    }

    // Обходим узлы напрямую, чтобы не искать каждый элемент заново по индексу.
    template <typename Action>
    void forEach(Action action) const {
        for (const Node* node = first; node; node = node->next) action(node->value);
    }

    template <typename Predicate>
    void removeIf(Predicate predicate) {
        Node* previous = nullptr;
        Node* node = first;
        while (node) {
            if (predicate(node->value)) {
                node = unlink(previous, node);
            } else {
                previous = node;
                node = node->next;
            }
        }
    }
};
