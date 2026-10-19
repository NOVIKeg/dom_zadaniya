#include <iostream>
#include <cstddef>
#include <limits>

int ** concat_rows(
    const int * const * lhs, size_t n1, size_t m1,
    const int * const * rhs, size_t n2, size_t m2
) {
    size_t max_rows = 0;
    if (n1 > n2) {
        max_rows = n1;
    } else {
        max_rows = n2;
    }

    int** result = new (std::nothrow) int*[max_rows];
    if (result == nullptr) {
        return nullptr;
    }

    for (size_t i = 0; i < max_rows; ++i) {
        // определяю сколько выделить памяти под i-ю строку результата
        size_t len = 0;
        bool has_lhs = (i < n1);
        bool has_rhs = (i < n2);
        if (has_lhs && has_rhs) {
            if (m1 > std::numeric_limits<size_t>::max() - m2) {
                for (size_t j = 0; j < i; ++j) {
                    delete[] result[j];
                }
                delete[] result;
                return nullptr;
            }
            len = m1 + m2;
        } else if (has_lhs) {
            len = m1;
        } else {
            len = m2;
        }

        result[i] = new (std::nothrow) int[len];
        if (result[i] == nullptr) {
            for (size_t j = 0; j < i; ++j) {
                delete[] result[j];
            }
            delete[] result;
            return nullptr;
        }

        // заполняю строку
        size_t pos = 0;
        if (has_lhs) {
            for (size_t j = 0; j < m1; ++j) {
                result[i][pos] = lhs[i][j];
                ++pos;
            }
        }
        if (has_rhs) {
            for (size_t j = 0; j < m2; ++j) {
                result[i][pos] = rhs[i][j];
                ++pos;
            }
        }
    }
    return result;
}

// функция для ввода
int input_matrix(size_t rows, size_t cols, int**& matrix) {
    matrix = new (std::nothrow) int*[rows];
    if (matrix == nullptr) {
        return 2;
    }
    for (size_t i = 0; i < rows; ++i) {
        matrix[i] = new (std::nothrow) int[cols];
        if (matrix[i] == nullptr) {
            for (size_t j = 0; j < i; ++j) {
                delete[] matrix[j];
            }
            delete[] matrix;
            matrix = nullptr;
            return 2;
        }
    }
    for (size_t i = 0; i < rows; ++i) {
        for (size_t j = 0; j < cols; ++j) {
            std::cin >> matrix[i][j];
            if (std::cin.fail()) {
                for (size_t k = 0; k < rows; ++k) {
                    delete[] matrix[k];
                }
                delete[] matrix;
                matrix = nullptr;
                return 1;
            }
        }
    }
    return 0;
}

// функция освобождения памяти под матрицу
void free_matrix(int** matrix, size_t rows) {
    if (matrix == nullptr) {
        return ;
    }
    for (size_t i = 0; i < rows; ++i) {
        delete[] matrix[i];
    }
    delete[] matrix;
}

int main() {
    size_t n1 = 0, m1 = 0, n2 = 0, m2 = 0;
    std::cin >> n1 >> m1;
    if (std::cin.fail() || n1 == 0 || m1 == 0) {
        std::cerr << "Ошибка: некорректные размеры первой матрицы\n";
        return 1;
    }
    std::cin >> n2 >> m2;
    if (std::cin.fail() || n2 == 0 || m2 == 0) {
        std::cerr << "Ошибка: некорректные размеры второй матрицы\n";
        return 1;
    }

    // ввод первой матрицы
    int** lhs = nullptr;
    int status = input_matrix(n1, m1, lhs);
    if (status == 1) {
        std::cerr << "Ошибка: не удалось ввести элемент матрицы\n";
        return 1;
    }
    if (status == 2) {
        std::cerr << "Ошибка: не удалось выделить память\n";
        return 2;
    }

    // ввод второй матрицы
    int** rhs = nullptr;
    status = input_matrix(n2, m2, rhs);
    if (status == 1) {
        free_matrix(lhs, n1);
        std::cerr << "Ошибка: не удалось ввести элемент матрицы\n";
        return 1;
    }
    if (status == 2) {
        free_matrix(lhs, n1);
        std::cerr << "Ошибка: не удалось выделить память\n";
        return 2;
    }

    // соединяем строки
    int** result = concat_rows(lhs, n1, m1, rhs, n2, m2);
    if (result == nullptr) {
        free_matrix(lhs, n1);
        free_matrix(rhs, n2);
        std::cerr << "Ошибка: не удалось выделить память\n";
        return 2;
    }

    // определяю размеры результата для вывода
    size_t max_rows;
    if (n1 > n2) {
        max_rows = n1;
    } else {
        max_rows = n2;
    }

    // вывожу результат
    for (size_t i = 0; i < max_rows; ++i) {
        size_t len = 0;
        bool has_lhs = (i < n1);
        bool has_rhs = (i < n2);
        if (has_lhs && has_rhs) {
            len = m1 + m2;
        } else if (has_lhs) {
            len = m1;
        } else {
            len = m2;
        }
        for (size_t j = 0; j < len; ++j) {
            std::cout << result[i][j];
            if (j + 1 < len) {
                std::cout << ' ';
            }
        }
        std::cout << '\n';
    }

    // освобождаю память
    free_matrix(lhs, n1);
    free_matrix(rhs, n2);
    free_matrix(result, max_rows);
    return 0;
}
