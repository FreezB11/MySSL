#include "matrix.h"

#include <iostream>
#include <stdexcept>
#include "matrix.h"
#define ll long long

matrix::matrix(ll rows, ll cols):row(rows), column(cols) {
    data = new double* [rows];
    for (int i = 0; i < rows; ++i) {
        data[i] = new double[cols]();
    }
    std::cout << "initiated" << std::endl;
}

matrix::~matrix() {
    for (int i = 0; i < row; ++i) {
        delete[] data[i];
    }
    delete[] data;
}

int matrix::getRows() const { return row; }
int matrix::getCols() const { return column; }

double& matrix::at(int row, int col) {
    if (row >= row || col >= column || row < 0 || col < 0) {
        throw std::out_of_range("Index out of range");
    }
    return data[row][col];
}

double matrix::at(int row, int col) const {
    if (row >= row || col >= column || row < 0 || col < 0) {
        throw std::out_of_range("Index out of range");
    }
    return data[row][col];
}

matrix matrix::operator+(const matrix& other) const {
    if (row != other.row || column != other.column) {
        throw std::invalid_argument("Matrices must have the same dimensions to add");
    }
    matrix result(row, column);
    for (int i = 0; i < row; ++i) {
        for (int j = 0; j < column; ++j) {
            result.at(i, j) = this->at(i, j) + other.at(i, j);
        }
    }
    return result;
}

matrix matrix::operator-(const matrix& other) const {
    if (row != other.row || column != other.column) {
        throw std::invalid_argument("Matrices must have the same dimensions to subtract");
    }
    matrix result(row, column);
    for (int i = 0; i < row; ++i) {
        for (int j = 0; j < column; ++j) {
            result.at(i, j) = this->at(i, j) - other.at(i, j);
        }
    }
    return result;
}

matrix matrix::operator*(const matrix& other) const {
    if (column != other.row) {
        throw std::invalid_argument("matrix dimensions do not allow multiplication");
    }
    matrix result(row, other.column);
    for (int i = 0; i < row; ++i) {
        for (int j = 0; j < other.column; ++j) {
            for (int k = 0; k < column; ++k) {
                result.at(i, j) += this->at(i, k) * other.at(k, j);
            }
        }
    }
    return result;
}

void matrix::display() const {
    for (int i = 0; i < row; ++i) {
        for (int j = 0; j < column; ++j) {
            std::cout << data[i][j] << " ";
        }
        std::cout << std::endl;
    }
}

