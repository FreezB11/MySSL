#pragma once
#define ll long long

class matrix {
private:
	ll row, column;
	double** data;
public:
    matrix(ll rows, ll cols);
    ~matrix();

    int getRows() const;
    int getCols() const;

    double& at(int row, int col);
    double at(int row, int col) const;

    matrix operator+(const matrix& other) const;
    matrix operator-(const matrix& other) const;
    matrix operator*(const matrix& other) const;

    void display() const;
};