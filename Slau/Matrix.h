#pragma once
#ifndef _MATRIX_H_
#define _MATRIX_H_
#include <iostream>
#include <random>
#include <ctime>
#include <locale>
#include "Exception.h"

using namespace std;

class Matrix
{
private:
    double** matrix;
    double** temp_matrix;
    int row = 0; int col = 0;
public:
    Matrix();
    Matrix(int, int);
    ~Matrix();
    double** get_Matrix();
    double** get_Matrix_temp();
    void set_Matrix();
    void set_Matrix(double**);
    void set_Matrix(Matrix&);
    void init_Matrix();
    double** init_Matrix(int, int);
    void fill_Matrix();
    void fillMatrix(int);
    double** add_Matrix(Matrix&);
    double** add_Matrix(Matrix&, Matrix&);
    double** mul_Matrix(Matrix&);
    double** mul_Matrix(Matrix&, Matrix&);
    double Determinant();
    Matrix* SubMatrix(int, int);
    friend Matrix& operator~ (Matrix&);
    Matrix& operator= (Matrix&);
    friend Matrix operator* (Matrix&, Matrix&);
    friend ostream& operator<< (ostream&, Matrix&);
    friend istream& operator>> (istream&, Matrix&);
};

Matrix::Matrix()
{
    while (this->row < 3 || this->col < 3 || this->row > 10 || this->col > 10)
    {
        cin >> this->row;
        cin >> this->col;
    }
    this->set_Matrix();
}

Matrix::Matrix(int row1, int col1)
{
    //if (row1 < 3 || col1 < 3 || row1 > 10 || col1 > 10)
    //    throw LimitExceedingException();
    this->row = row1;
    this->col = col1;
    this->set_Matrix();
}

Matrix::~Matrix()
{
    for (int i = 0; i < this->row; i++)
        delete[] this->matrix[i];
    delete[] this->matrix;
}

double** Matrix::get_Matrix() { return this->matrix; }

double** Matrix::get_Matrix_temp() { return this->temp_matrix; }

void Matrix::set_Matrix() { this->matrix = init_Matrix(this->row, this->col); }

void Matrix::set_Matrix(double** _matrix) { this->matrix = _matrix; }

void Matrix::set_Matrix(Matrix& ob) { this->matrix = ob.matrix; }

void Matrix::init_Matrix()
{
    this->matrix = new double* [this->row];
    for (int i = 0; i < this->row; i++)
        this->matrix[i] = new double[this->col];
}

double** Matrix::init_Matrix(int row, int col)
{
    this->matrix = new double* [this->row];
    for (int i = 0; i < this->row; i++)
        this->matrix[i] = new double[this->col];

    return this->matrix;
}

void Matrix::fill_Matrix()
{
    for (int i = 0; i < this->row; i++)
        for (int j = 0; j < this->col; j++)
            this->matrix[i][j] = rand() % 10;
}

void Matrix::fillMatrix(int _m)
{
    if (_m == 1)
    {
        for (int i = 0; i < this->row; i++)
        {
            cout << "Строка " << i + 1 << ":" << endl;
            for (int j = 0; j < this->col; j++)
            {
                if (_m == 2) { cout << "Элемент [" << i + 1 << "; " << j + 1 << "]" << ":" << endl; }
                cin >> this->matrix[i][j];
            }
        }
    }
    if (_m == 2)
    {
        for (int i = 0; i < this->row; i++)
        {
            for (int j = 0; j < this->col; j++)
            {
                cout << "Элемент [" << i + 1 << "; " << j + 1 << "]" << ":" << endl;
                cin >> this->matrix[i][j];
            }
        }
    }
}

double** Matrix::add_Matrix(Matrix& ob)
{
    if ((this->row != ob.row) || (this->col != ob.col))
        throw DimensionSumException();

    double** additionMatrix = Matrix::init_Matrix(this->row, this->col);

    for (int i = 0; i < this->row; i++)
        for (int j = 0; j < this->col; j++)
            additionMatrix[i][j] = this->matrix[i][j] + ob.matrix[i][j];

    return additionMatrix;
}

double** Matrix::add_Matrix(Matrix& ob1, Matrix& ob2)
{
    if ((ob1.row != ob2.row) || (ob1.col != ob2.col))
        throw DimensionSumException();

    double** additionMatrix = Matrix::init_Matrix(ob1.row, ob1.col);

    for (int i = 0; i < ob1.row; i++)
        for (int j = 0; j < ob1.col; j++)
            additionMatrix[i][j] = ob1.matrix[i][j] + ob2.matrix[i][j];
    
    return additionMatrix;
}

double** Matrix::mul_Matrix(Matrix& ob)
{
    if (this->col != ob.row)
        throw DimensionProductException();

    double** multiply_matrix = new double* [this->row];
    for (int i = 0; i < this->row; i++)
    {
        multiply_matrix[i] = new double[ob.col];
    }

    for (int i = 0; i < this->row; i++)
    {
        for (int j = 0; j < ob.col; j++)
        {
            multiply_matrix[i][j] = 0;
            for (int p = 0; p < this->col; p++)
            {
                multiply_matrix[i][j] += this->matrix[i][p] * ob.matrix[p][j];
            }
        }
    }
    return multiply_matrix;
}

double** Matrix::mul_Matrix(Matrix& ob1, Matrix& ob2)
{
    if (ob1.col != ob2.row)
        throw DimensionProductException();

    double** multiply_matrix = new double* [ob1.row];
    for (int i = 0; i < ob1.row; i++)
    {
        multiply_matrix[i] = new double[ob2.col];
    }

    for (int i = 0; i < ob1.row; i++)
    {
        for (int j = 0; j < ob2.col; j++)
        {
            multiply_matrix[i][j] = 0;
            for (int p = 0; p < ob1.col; p++)
            {
                multiply_matrix[i][j] += ob1.matrix[i][p] * ob2.matrix[p][j];
            }
        }
    }
    return multiply_matrix;
}

Matrix* Matrix::SubMatrix(int i1, int j1)
{
    Matrix* temp = new Matrix(this->row - 1, this->col - 1);
    for (int i = 0; i < i1; i++)
    {
        for (int j = 0; j < j1; j++)
            temp->matrix[i][j] = this->matrix[i][j];
        for (int j = j1 + 1; j < this->row; j++)
            temp->matrix[i][j - 1] = this->matrix[i][j];
    }
    for (int i = i1 + 1; i < this->row; i++)
    {
        for (int j = 0; j < j1; j++)
            temp->matrix[i - 1][j] = this->matrix[i][j];
        for (int j = j1 + 1; j < this->row; j++)
            temp->matrix[i - 1][j - 1] = this->matrix[i][j];
    }
    return temp;
}

double Matrix::Determinant()
{
    double det = 0;
    if (this->row == 1 && this->col == 1)
        return this->matrix[0][0];
    //else if (this->row == 2 || this->col == 2)
    //    return this->matrix[0][0] * this->matrix[1][1] - this->matrix[0][1] * this->matrix[1][0];
    //else if (this->row == 3 || this->col == 3)
    //    return ((this->matrix[0][0] * this->matrix[1][1] * this->matrix[2][2] + this->matrix[0][1] * this->matrix[1][2] * this->matrix[2][0] + this->matrix[1][0] * this->matrix[2][1] * this->matrix[0][2])
    //        - (this->matrix[0][2] * this->matrix[1][1] * this->matrix[2][0] + this->matrix[0][1] * this->matrix[1][0] * this->matrix[2][2] + this->matrix[1][2] * this->matrix[2][1] * this->matrix[0][0]));
    else
    {
        Matrix* temp1 = new Matrix(this->row - 1, this->col - 1);
        for (int j = 0; j < this->row; j++)
        {
            temp1 = SubMatrix(0, j);
            if (j % 2 == 0)
                det += temp1->Determinant() * this->matrix[0][j];
            else
                det -= temp1->Determinant() * this->matrix[0][j];
        }
        delete temp1;
    }
    return det;
}

Matrix& Matrix::operator= (Matrix& ob)
{
    if (this->row != ob.row || this->col != ob.col)
    {
        for (int i = 0; i < this->row; i++)
            delete[] this->matrix[i];
        delete[] this->matrix;
        this->row = ob.row;
        this->col = ob.col;
        this->matrix = new double* [this->row];
        for (int i = 0; i < this->row; i++)
            this->matrix[i] = new double[this->col];
    }
    for (int i = 0; i < this->row; i++)
        for (int j = 0; j < this->col; j++)
            this->matrix[i][j] = ob.matrix[i][j];
    return *this;
}

Matrix& operator~ (Matrix& ob)
{
    Matrix* res = new Matrix(ob.row, ob.col);
    double det = ob.Determinant();
    if (det == 0)
        throw ZeroDivideException();
    Matrix* temp = new Matrix(ob.row - 1, ob.col - 1);
    int z;
    for (int i = 0; i < ob.row; i++)
    {
        z = i % 2 == 0 ? 1 : -1;
        for (int j = 0; j < ob.row; j++)
        {
            temp = ob.SubMatrix(i, j);
            res->matrix[j][i] = z * temp->Determinant() / det;
            z = -z;
        }
    }
    delete temp;
    return *res;
}

Matrix operator* (Matrix& ob1, Matrix& ob2)
{
    if (ob1.col != ob2.row)
        throw DimensionProductException();

    Matrix* temp = new Matrix(ob1.row, ob2.col);
    for (int i = 0; i < ob1.row; i++)
    {
        for (int j = 0; j < ob2.col; j++)
        {
            temp->matrix[i][j] = 0;
            for (int p = 0; p < ob1.col; p++)
            {
                temp->matrix[i][j] += ob1.matrix[i][p] * ob2.matrix[p][j];
            }
        }
    }
    return *temp;
}

istream& operator>> (istream& in, Matrix& _matrix)
{
    if (_matrix.matrix != nullptr)
    {
        for (int i = 0; i < _matrix.row; i++)
            for (int j = 0; j < _matrix.col; j++)
                in >> _matrix.matrix[i][j];
    }
    return in;
}
#endif // !_MATRIX_H_