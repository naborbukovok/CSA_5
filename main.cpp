#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <list>
#include <mutex>
#include <stack>
#include <string>
#include <thread>
#include <utility>
#include <vector>

#include "Book.cpp"

std::mutex locker;
// Каталог книг.
std::list<Book> catalog;
// Книги, которые необходимо занести в каталог.
std::stack<Book> books;

// Добавление записи о книге в каталог.
void addRecord() {
    while (!books.empty()) {
        locker.lock();
        Book book = books.top();
        // Убираем книгу из списка необходимых к внесению.
        books.pop();
        // Добавляем запись о ней в каталог.
        catalog.push_back(book);
        std::cout << "One of activists added book #" << book.id << "...\n";
        locker.unlock();
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

// Сравнение книг по идентификационному номеру.
bool compareBooks(Book first, Book second) {
    return first.id < second.id;
}

// Вывод информации о книгах в порядке возрастания идентификационного номера.
void printCatalog() {
    std::cout << "\nLIBRARY CATALOG IS COMPLETED:\n";
    catalog.sort(compareBooks);
    for (Book book: catalog) {
        book.printBook();
    }
}

// Рандомная генерация списка библиотечных книг на основе информации
// о размерах библиотеки (M рядов по N шкафов по K книг в каждом шкафу).
void setBooks(int M, int N, int K) {
    std::cout << "Generating library...\n";
    // Создадим вектор, хранящий свободные id. Изначально свободны все: от 0 до (M * N * K - 1).
    std::vector<int> ids = {};
    for (int i = 0; i < (M * N * K); i++) {
        ids.push_back(i);
    }
    // Для каждого шкафа в каждом ряду выберем K свободных id книг.
    for (int m = 0; m < M; m++) {
        for (int n = 0; n < N; n++) {
            for (int k = 0; k < K; k++) {
                // Генерируем индекс id из списка свободных.
                int id_number = rand() % ids.size();
                int id = ids[id_number];
                // Удаляем его из этого списка.
                std::swap(ids[id_number], ids[ids.size() - 1]);
                ids.pop_back();
                // Создаем книгу с выбранным id, находящуюся в ряду m и шкафу n.
                Book *book = new Book(id, m, n);
                // Добавляем её в список книг, не записанных в каталог.
                books.push(*book);
            }
        }
    }
    std::cout << "Generating is finished.\n";
}

// Проверка корректности формата команды.
static void incorrectCommand() {
    std::cout << "Incorrect command!\n"
                 "Try: <M> <N> <K> <activists count>\n";
}

// Проверка корректности одного агрумента.
static bool isArgumentCorrect(const std::string &input, int max, const std::string &message) {
    if (!input.empty() &&
        std::all_of(input.begin(),input.end(),::isdigit)) {
        if (std::stol(input) < 1 || std::stol(input) > max) {
            std::cout << message;
            return false;
        }
    }
    return true;
}

// Ограничение по количеству рядов/шкафов/книг в шкафу - 1000. Ограничение по количеству активистов - 100.
static bool isInputCorrect(const std::string &first, const std::string &second,
                           const std::string &third, const std::string &fourth) {
    if (!isArgumentCorrect(first, 100, "Try M between 1 and 50.") ||
        !isArgumentCorrect(second, 100, "Try N between 1 and 50.") ||
        !isArgumentCorrect(third, 100, "Try K between 1 and 100.") ||
        !isArgumentCorrect(fourth, 100, "Try number of activists between 1 and 50.")) {
        return false;
    }
    return true;
}

int main(int argc, char *argv[]) {
    clock_t start_time = clock();

    if (argc != 5) {
        incorrectCommand();
        return 1;
    }
    if (!isInputCorrect(argv[1], argv[2], argv[3], argv[4])) {
        return 1;
    }

    int M = std::stol(argv[1]);
    int N = std::stol(argv[2]);
    int K = std::stol(argv[3]);
    int activistsCount = std::stol(argv[4]);
    auto *activists = new std::thread[activistsCount];

    setBooks(M, N, K);

    catalog = {};
    for (int i = 0; i < activistsCount; ++i) {
        activists[i] = std::thread(addRecord);
    }
    for (int i = 0; i < activistsCount; ++i) {
        activists[i].join();
    }

    printCatalog();

    clock_t end_time = clock();
    std::cout << "Time taken: " << (double) (end_time - start_time) / CLOCKS_PER_SEC << "s";

    return 0;
}
