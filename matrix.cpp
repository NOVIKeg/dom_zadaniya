#include <iostream>

int main() {
    int rows = 0, cols = 0;
    std::cin >> rows >> cols;
    if (std::cin.fail()) 
    {
        std::cerr << "Ошибка: не удалось ввести размеры матрицы\n";
        return 1;
    }
    if (rows <= 0 || cols <= 0) 
    {
        std::cerr << "Ошибка: некорректные размеры матрицы\n";
        return 1;
    }

    // выделение памяти под массив указателей на строки
    int** matrix = new (std::nothrow) int*[rows];
    if (matrix == nullptr) 
    {
        std::cerr << "Ошибка: не удалось выделить память\n";
        return 2;
    }

    // выделение памяти под каждую строку
    for (int i = 0; i < rows; ++i) 
    {
        matrix[i] = new (std::nothrow) int[cols];
        if (matrix[i] == nullptr) 
        {
            // освобождаем уже выделенную память
            for (int j = 0; j < i; ++j) 
            {
                delete[] matrix[j];
            }
            delete[] matrix;
            std::cerr << "Ошибка: не удалось выделить память\n";
            return 2;
        }
    }

    // ввод элементов матрицы
    for (int i = 0; i < rows; ++i) 
    {
        for (int j = 0; j < cols; ++j) 
        {   
            std::cin >> matrix[i][j];
            if (std::cin.fail())
            {
                std::cerr << "Ошибка: не удалось ввести элемент матрицы\n";
                for (int k = 0; k < rows; ++k) 
                {
                    delete[] matrix[k];
                }
                delete[] matrix;
                return 1;
            }
        }
    }

    // выделение памяти для транспонированной
    int** transposed = new (std::nothrow) int*[cols];
    if (transposed == nullptr) 
    {
        std::cerr << "Ошибка: не удалось выделить память\n";
        return 2;
    }
    for (int j = 0; j < cols; ++j) 
    {
        transposed[j] = new (std::nothrow) int[rows];
        if (transposed[j] == nullptr) 
        {
            // освобождаем уже выделенную память под строки
            for (int k = 0; k < j; ++k)
            {
                delete[] transposed[k];
            }
            delete[] transposed;
            std::cerr << "Ошибка: не удалось выделить память\n";
            return 2;
        }
    }

    // транспонирование
    for (int i = 0; i < rows; ++i) 
    {
        for (int j = 0; j < cols; ++j) 
        {
            transposed[j][i] = matrix[i][j];
        }
    }

    // выводим транспонированную матрицу
    for (int j = 0; j < cols; ++j) 
    {
        for (int i = 0; i < rows; ++i) 
        {
            std::cout << transposed[j][i];
            if (i + 1 < rows) 
            {
                std::cout << ' ';
            }
        }
        std::cout << '\n';
    }

    //освободили память 
    for (int i = 0; i < rows; ++i) 
    {
        delete[] matrix[i];
    }
    delete[] matrix;

    for (int j = 0; j < cols; ++j)
    {
        delete[] transposed[j];
    }
    delete[] transposed;
    
    return 0;
}
