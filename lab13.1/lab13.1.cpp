// lab13.1.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.

#include <iostream>
#include <fstream>
#include <iomanip>
#include <cmath>

using namespace std;

// Функция приведения расширенной матрицы системы к ступенчатому виду
// Параметры:
//   a - двумерный динамический массив (расширенная матрица)
//   m - количество уравнений
//   n - количество неизвестных
// Возвращает ранг матрицы системы (без учета столбца свободных членов)
int gaussJordanToEchelon(double** a, int m, int n) {
    /*
     * Элементарные преобразования матриц, используемые в методе Гаусса-Жордана:
     * 1. Перестановка строк (строки можно менять местами)
     * 2. Умножение строки на ненулевое число
     * 3. Прибавление к одной строке другой строки, умноженной на число
     */

    int rank = 0;           // ранг матрицы
    int row = 0;            // текущая строка
    int col = 0;            // текущий столбец

    // Проходим по столбцам коэффициентов (от 0 до n-1)
    while (row < m && col < n) {
        // Поиск ненулевого элемента в текущем столбце, начиная с текущей строки
        int pivotRow = -1;
        for (int i = row; i < m; i++) {
            if (fabs(a[i][col]) > 1e-9) {
                pivotRow = i;
                break;
            }
        }

        // Если в столбце нет ненулевых элементов, переходим к следующему столбцу
        if (pivotRow == -1) {
            col++;
            continue;
        }

        // Перестановка строк, чтобы ведущий элемент оказался на позиции (row, col)
        if (pivotRow != row) {
            for (int j = col; j <= n; j++) {
                swap(a[row][j], a[pivotRow][j]);
            }
        }

        // Нормируем ведущую строку (делим на ведущий элемент)
        double pivot = a[row][col];
        for (int j = col; j <= n; j++) {
            a[row][j] /= pivot;
        }

        // Обнуляем элементы ниже текущей строки в этом столбце
        for (int i = row + 1; i < m; i++) {
            double factor = a[i][col];
            if (fabs(factor) > 1e-9) {
                for (int j = col; j <= n; j++) {
                    a[i][j] -= factor * a[row][j];
                }
            }
        }

        rank++;
        row++;
        col++;
    }

    return rank;
}

// Функция для вывода матрицы в консоль (для отладки)
void printMatrix(double** a, int m, int n) {
    for (int i = 0; i < m; i++) {
        for (int j = 0; j <= n; j++) {
            cout << setw(10) << setprecision(4) << a[i][j] << " ";
        }
        cout << endl;
    }
}

// Функция для вывода матрицы в файл
void printMatrixToFile(ofstream& out, double** a, int m, int n) {
    for (int i = 0; i < m; i++) {
        for (int j = 0; j <= n; j++) {
            out << setw(12) << setprecision(6) << a[i][j] << " ";
        }
        out << endl;
    }
}

int main() {
    setlocale(LC_ALL, "Russian");

    ifstream input("input.txt");
    if (!input.is_open()) {
        cerr << "Ошибка: не удалось открыть файл input.txt" << endl;
        return 1;
    }

    int m, n;
    input >> m >> n;  // m - количество уравнений, n - количество неизвестных

    // Выделение памяти под расширенную матрицу (m строк, n+1 столбцов)
    double** matrix = new double* [m];
    for (int i = 0; i < m; i++) {
        matrix[i] = new double[n + 1];
        for (int j = 0; j <= n; j++) {
            input >> matrix[i][j];
        }
    }
    input.close();

    cout << "Исходная расширенная матрица системы:" << endl;
    printMatrix(matrix, m, n);
    cout << endl;

    // Приведение матрицы к ступенчатому виду
    int rank = gaussJordanToEchelon(matrix, m, n);

    cout << "Матрица после приведения к ступенчатому виду:" << endl;
    printMatrix(matrix, m, n);
    cout << "\nРанг матрицы системы: " << rank << endl;

    // Запись результата в выходной файл
    ofstream output("output.txt");
    if (!output.is_open()) {
        cerr << "Ошибка: не удалось создать файл output.txt" << endl;

        // Освобождение памяти
        for (int i = 0; i < m; i++) {
            delete[] matrix[i];
        }
        delete[] matrix;

        return 1;
    }

    output << "Исходная система: " << m << " уравнений, " << n << " неизвестных" << endl;
    output << "Расширенная матрица после приведения к ступенчатому виду:" << endl;
    printMatrixToFile(output, matrix, m, n);
    output << "\nРанг матрицы системы: " << rank << endl;

    // Определение типа системы по рангу
    output << "\nАнализ системы:" << endl;

    // Проверка на несовместность (наличие строки вида 0 = b, где b != 0)
    bool inconsistent = false;
    for (int i = 0; i < m; i++) {
        bool allZero = true;
        for (int j = 0; j < n; j++) {
            if (fabs(matrix[i][j]) > 1e-9) {
                allZero = false;
                break;
            }
        }
        if (allZero && fabs(matrix[i][n]) > 1e-9) {
            inconsistent = true;
            break;
        }
    }

    if (inconsistent) {
        output << "Система несовместна (решений нет)." << endl;
    }
    else if (rank == n) {
        output << "Система совместна и имеет единственное решение." << endl;
    }
    else {
        output << "Система совместна и имеет бесконечно много решений." << endl;
        output << "Количество свободных переменных: " << n - rank << endl;
    }

    output.close();

    cout << "\nРезультат сохранен в файл output.txt" << endl;

    // Освобождение памяти
    for (int i = 0; i < m; i++) {
        delete[] matrix[i];
    }
    delete[] matrix;

    return 0;
}