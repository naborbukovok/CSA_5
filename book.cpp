#include <iostream>

struct Book {
    // Идентификационный номер книги.
    int id;
    // Позиция книги: ряд и шкаф.
    std::pair<int, int> position;

    // Конструктор книги с параметрами.
    Book(int id, int row, int bookcase) {
        this->id = id;
        this->position.first = row;
        this->position.second = bookcase;
    }

    Book() = default;

    // Вывод информации о книге.
    void printBook() const {
        std::cout << "Book.\n" <<
                     "ID: " << id << ",\n" <<
                     "Position (row, bookcase): " << position.first << ", " << position.second << ".\n\n";
    }
};
