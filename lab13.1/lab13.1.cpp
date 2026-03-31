// lab13.1.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.

#include <iostream>
#include <fstream>
#include <iomanip>
#include <cmath>

using namespace std;


int gaussJordanToEchelon(double** a, int m, int n) {
  

    int rank = 0;          
    int row = 0;            
    int col = 0;           

    
    while (row < m && col < n) {
        
        int pivotRow = -1;
        for (int i = row; i < m; i++) {
            if (fabs(a[i][col]) > 1e-9) {
                pivotRow = i;
                break;
            }
        }

        
        if (pivotRow == -1) {
            col++;
            continue;
        }

        
        if (pivotRow != row) {
            for (int j = col; j <= n; j++) {
                swap(a[row][j], a[pivotRow][j]);
            }
        }

        
        double pivot = a[row][col];
        for (int j = col; j <= n; j++) {
            a[row][j] /= pivot;
        }

        
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


void printMatrix(double** a, int m, int n) {
    for (int i = 0; i < m; i++) {
        for (int j = 0; j <= n; j++) {
            cout << setw(10) << setprecision(4) << a[i][j] << " ";
        }
        cout << endl;
    }
}


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
    input >> m >> n;  

    
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

    
    int rank = gaussJordanToEchelon(matrix, m, n);

    cout << "Матрица после приведения к ступенчатому виду:" << endl;
    printMatrix(matrix, m, n);
    cout << "\nРанг матрицы системы: " << rank << endl;

    
    ofstream output("output.txt");
    if (!output.is_open()) {
        cerr << "Ошибка: не удалось создать файл output.txt" << endl;

      
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

 
    output << "\nАнализ системы:" << endl;

   
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

   
    for (int i = 0; i < m; i++) {
        delete[] matrix[i];
    }
    delete[] matrix;

    return 0;
}