#include <iostream>
#include <cstring>
#include "include/stack.hpp"
#include "include/fixed_vector_memory_resource.hpp"

// Сложный тип для демонстрации
struct ComplexType {
    int id;
    double value;
    char name[20];

    ComplexType(int i = 0, double v = 0.0, const char* n = "") 
        : id(i), value(v) {
        strncpy(name, n, 19);
        name[19] = '\0';
    }
};

int main() {
    std::cout << "=== Лабораторная работа 5: Стек с фиксированным аллокатором ===\n" << std::endl;
    
    fixed_vector_memory_resource pool(2048);

    std::cout << "Тест 1: Работа с типом int" << std::endl;
    std::cout << "--------------------------" << std::endl;
    
    Stack<int> int_stack(&pool);
    
    for (int i = 1; i <= 5; ++i) {
        int_stack.push(i * 10);
        std::cout << "Добавлен: " << i * 10 << std::endl;
    }
    
    std::cout << "\nРазмер стека: " << int_stack.size() << std::endl;
    std::cout << "Верхний элемент: " << int_stack.top() << std::endl;
    
    std::cout << "\nЭлементы через итератор (сверху вниз): ";
    for (auto it = int_stack.begin(); it != int_stack.end(); ++it) {
        std::cout << *it << " ";
    }
    std::cout << std::endl;
    
    std::cout << "\nИзвлечение элементов (LIFO): ";
    while (!int_stack.empty()) {
        std::cout << int_stack.top() << " ";
        int_stack.pop();
    }
    std::cout << std::endl;
    
    std::cout << "\nРазмер после извлечения: " << int_stack.size() << std::endl;

    std::cout << "\n\nТест 2: Работа со сложным типом ComplexType" << std::endl;
    std::cout << "-------------------------------------------" << std::endl;
    
    Stack<ComplexType> complex_stack(&pool);
    
    complex_stack.push(ComplexType(1, 3.14159, "Первый"));
    complex_stack.push(ComplexType(2, 2.71828, "Второй"));
    complex_stack.push(ComplexType(3, 1.41421, "Третий"));
    
    std::cout << "Добавлено 3 комплексных объекта" << std::endl;
    std::cout << "Размер стека: " << complex_stack.size() << std::endl;
    
    std::cout << "\nЭлементы через const итератор:" << std::endl;
    for (auto it = complex_stack.cbegin(); it != complex_stack.cend(); ++it) {
        std::cout << "  ID: " << it->id 
                  << ", Value: " << it->value 
                  << ", Name: " << it->name << std::endl;
    }
    
    std::cout << "\nИзвлечение элементов:" << std::endl;
    while (!complex_stack.empty()) {
        const auto& top = complex_stack.top();
        std::cout << "  Извлечён: ID=" << top.id 
                  << ", Value=" << top.value 
                  << ", Name=" << top.name << std::endl;
        complex_stack.pop();
    }

    std::cout << "\n\nТест 3: Перемещение стека" << std::endl;
    std::cout << "-------------------------" << std::endl;
    
    Stack<int> stack1(&pool);
    stack1.push(100);
    stack1.push(200);
    stack1.push(300);
    
    std::cout << "stack1 размер: " << stack1.size() << std::endl;
    
    Stack<int> stack2(std::move(stack1));
    
    std::cout << "После перемещения:" << std::endl;
    std::cout << "  stack1 размер: " << stack1.size() << " (должен быть 0)" << std::endl;
    std::cout << "  stack2 размер: " << stack2.size() << " (должен быть 3)" << std::endl;
    
    std::cout << "\nЭлементы в stack2: ";
    for (const auto& elem : stack2) {
        std::cout << elem << " ";
    }
    std::cout << std::endl;

    std::cout << "\n\nТест 4: Проверка аллокатора" << std::endl;
    std::cout << "----------------------------" << std::endl;
    
    Stack<double> double_stack(&pool);
    
    for (int i = 0; i < 5; ++i) {
        double_stack.push(i * 0.5);
    }
    
    std::cout << "Добавлено 5 элементов double" << std::endl;
    std::cout << "Верхний элемент: " << double_stack.top() << std::endl;
    
    std::cout << "\nОчистка стека double..." << std::endl;
    double_stack.clear();
    std::cout << "Размер после очистки: " << double_stack.size() << std::endl;

    std::cout << "\n=== Все тесты завершены успешно ===" << std::endl;
    std::cout << "Аллокатор автоматически освободит память при разрушении" << std::endl;
    
    return 0;
}
