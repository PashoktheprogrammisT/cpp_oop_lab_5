#ifndef STACK_HPP
#define STACK_HPP

#include <memory_resource>
#include <iterator>

// Узел односвязного списка для стека
template<typename T>
struct StackNode {
    T data;
    StackNode<T>* next;
    
    StackNode(const T& value, StackNode<T>* nxt = nullptr) 
        : data(value), next(nxt) {}
};

// Forward итератор для стека
template<typename T>
class StackIterator {
public:
    using iterator_category = std::forward_iterator_tag;
    using value_type = T;
    using difference_type = std::ptrdiff_t;
    using pointer = T*;
    using reference = T&;

private:
    StackNode<T>* current;

public:
    explicit StackIterator(StackNode<T>* ptr = nullptr) : current(ptr) {}
    
    template<typename U>
    StackIterator(const StackIterator<U>& other) : current(other.get_node()) {}
    
    StackNode<T>* get_node() const { return current; }

    reference operator*() const { return current->data; }
    pointer operator->() const { return &current->data; }

    StackIterator& operator++() {
        if (current) current = current->next;
        return *this;
    }

    StackIterator operator++(int) {
        StackIterator tmp = *this;
        ++(*this);
        return tmp;
    }

    bool operator==(const StackIterator& other) const {
        return current == other.current;
    }

    bool operator!=(const StackIterator& other) const {
        return !(*this == other);
    }
};

// Константная версия итератора
template<typename T>
class StackIterator<const T> {
public:
    using iterator_category = std::forward_iterator_tag;
    using value_type = const T;
    using difference_type = std::ptrdiff_t;
    using pointer = const T*;
    using reference = const T&;

private:
    const StackNode<T>* current;

public:
    explicit StackIterator(const StackNode<T>* ptr = nullptr) : current(ptr) {}
    
    StackIterator(const StackIterator<T>& other) : current(other.get_node()) {}
    
    const StackNode<T>* get_node() const { return current; }

    reference operator*() const { return current->data; }
    pointer operator->() const { return &current->data; }

    StackIterator& operator++() {
        if (current) current = current->next;
        return *this;
    }

    StackIterator operator++(int) {
        StackIterator tmp = *this;
        ++(*this);
        return tmp;
    }

    bool operator==(const StackIterator& other) const {
        return current == other.current;
    }

    bool operator!=(const StackIterator& other) const {
        return !(*this == other);
    }
};

// Шаблонный стек с полиморфным аллокатором
template<typename T>
class Stack {
private:
    StackNode<T>* top_node;
    std::pmr::polymorphic_allocator<StackNode<T>> allocator;
    std::size_t size_;

public:
    using iterator = StackIterator<T>;
    using const_iterator = StackIterator<const T>;

    explicit Stack(std::pmr::memory_resource* mr = std::pmr::get_default_resource())
        : top_node(nullptr), allocator(mr), size_(0) {}
    
    ~Stack() { clear(); }

    Stack(const Stack&) = delete;
    Stack& operator=(const Stack&) = delete;

    Stack(Stack&& other) noexcept
        : top_node(other.top_node), allocator(std::move(other.allocator)), size_(other.size_) {
        other.top_node = nullptr;
        other.size_ = 0;
    }

    Stack& operator=(Stack&& other) noexcept {
        if (this != &other) {
            clear();
            top_node = other.top_node;
            allocator = std::move(other.allocator);
            size_ = other.size_;
            other.top_node = nullptr;
            other.size_ = 0;
        }
        return *this;
    }

    void push(const T& value) {
        StackNode<T>* new_node = allocator.allocate(1);
        try {
            allocator.construct(new_node, value, top_node);
        } catch (...) {
            allocator.deallocate(new_node, 1);
            throw;
        }
        top_node = new_node;
        ++size_;
    }

    void pop() {
        if (empty()) return;
        StackNode<T>* old_top = top_node;
        top_node = top_node->next;
        allocator.destroy(old_top);
        allocator.deallocate(old_top, 1);
        --size_;
    }

    T& top() { return top_node->data; }
    const T& top() const { return top_node->data; }

    bool empty() const { return top_node == nullptr; }
    std::size_t size() const { return size_; }

    void clear() {
        while (!empty()) pop();
    }

    iterator begin() { return iterator(top_node); }
    iterator end() { return iterator(nullptr); }
    
    const_iterator begin() const { return const_iterator(top_node); }
    const_iterator end() const { return const_iterator(nullptr); }
    
    const_iterator cbegin() const { return const_iterator(top_node); }
    const_iterator cend() const { return const_iterator(nullptr); }
};

#endif
