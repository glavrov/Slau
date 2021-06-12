#pragma once
#ifndef _DE2APP_H_
#define _DE2APP_H_
#include <iostream>
#include <random>
#include <ctime>
#include <math.h>
#include <cmath>
#include "Exception.h"

using namespace std;

class DE2app
{
private:
    double** matrix_A;
    double** matrix_B;
    int row, col;
    double N, a, b, h, Xi, Ki, Qi; 
public:
    DE2app();
    DE2app(int, int, int);
    ~DE2app();
    void init_Matrix();
    void fill_Matrix(int);
    double** get_Matrix(int);
    void print_Matrix(int);
    double k(double);
    double q(double);
    double f(double);
    //void set_Matrix();
};

DE2app::DE2app()
{
    this->row = rand() % 10;
    this->col = this->row;
    this->N = 10;
    this->a = 0;
    this->b = 2;
    this->h = (this->b - this->a) / (double)this->N;
    this->Qi = 2;
    this->init_Matrix();
}

DE2app::DE2app(int _m, int _n, int _N)
{
    this->row = _m + 1;
    this->col = _n + 1;
    this->N = (double)_N;
    this->a = 0.0;
    this->b = 2.0;
    this->h = (this->b - this->a) / this->N;
    this->Qi = 2;
    this->init_Matrix();
}

DE2app::~DE2app()
{
    for (int i = 0; i < this->row; i++)
    {
        delete[] this->matrix_A[i];
        delete[] this->matrix_B[i];
    }
    delete[] this->matrix_A;
    delete[] this->matrix_B;
}

void DE2app::init_Matrix()
{
    this->matrix_A = new double* [this->row];
    this->matrix_B = new double* [this->row];;
    for (int i = 0; i < this->row; i++)
    {
        this->matrix_A[i] = new double[this->col];
        this->matrix_B[i] = new double[1];
    }
}

double DE2app::k(double x)
{
    return -x + 4.0;
}

double DE2app::q(double x)
{
    return 2.0;
}

double DE2app::f(double x)
{
    return 10.0 * (x * x - 5.0 * x + 9.0) / exp(x);
}

void DE2app::fill_Matrix(int _m)
{
    this->Xi = 0.0;
    if (_m == 1)
    {
        /*for (int i = 1; i < this->row; i++)
        {
            for (int j = 0; j < this->col; j++)
            {
                this->Xi = (double)i * this->h;
                if (j == i - 1)
                    //this->matrix_A[i][j] = (double)(i - 1.0)*this->h + 4 + 4 * (-(double)i * this->h + 4) - (double)(i + 1.0) * this->h + 4;        // нижняя диагональ
                    this->matrix_A[i][j] = 4.0 * k(Xi) - k(Xi + this->h) + k(Xi - this->h);
                else if (j == i)
                    //this->matrix_A[i][j] = -(8 * ((double)i * this->h + 4) + 4 * this->h * this->h * this->Qi);                                     // главная диагональ
                    this->matrix_A[i][j] = -8.0 * k(Xi) - 4.0 * this->h * this->h * q(Xi);
                else if (j == i + 1)
                    //this->matrix_A[i][j] = -(double)(i - 1.0) * this->h + 4 + 4 * (-(double)i * this->h + 4) + (double)(i + 1.0) * this->h + 4;     // верхняя диагональ
                    this->matrix_A[i][j] = 4.0 * k(Xi) + k(Xi + this->h) - k(Xi - this->h);
                else
                    this->matrix_A[i][j] = 0;
            }
        }*/
        for (int i = 0; i < this->row; i++)
        {
            for (int j = 0; j < this->col; j++)
            {
                this->matrix_A[i][j] = 0.0;
            }
        }

        this->matrix_A[0][0] = 1.0;
        this->matrix_A[this->row - 1][this->col - 1] = 1.0;

        int i1, i2;
        for (int i = 1; i < this->row - 1; i++)
        {
            i1 = i - 1;
            i2 = i + 1;
            this->Xi = (double)i * this->h;
            //this->matrix_A[i][i1] = (double)(i - 1.0) * this->h + 4 + 4 * (-(double)i * this->h + 4) - (double)(i + 1.0) * this->h + 4;
            //this->matrix_A[i][i] = -(8 * ((double)i * this->h + 4) + 4 * this->h * this->h * this->Qi);
            //this->matrix_A[i][i2] = -(double)(i - 1.0) * this->h + 4 + 4 * (-(double)i * this->h + 4) + (double)(i + 1.0) * this->h + 4;
            this->matrix_A[i][i1] = 4.0 * this->k(this->Xi) - this->k(this->Xi + this->h) + this->k(this->Xi - this->h);
            this->matrix_A[i][i] = -8.0 * this->k(this->Xi) - 4.0 * this->h * this->h * this->q(this->Xi);
            this->matrix_A[i][i2] = 4.0 * this->k(this->Xi) + this->k(this->Xi + this->h) - this->k(this->Xi - this->h);
        }
    }
    if (_m == 2)
    {
        this->matrix_B[0][0] = 0.0;
        for (int i = 1; i < this->row - 1; i++)
        {
                this->Xi = (double)i * this->h;
                //this->matrix_B[i][0] = -4.0 * this->h * this->h * 10.0 * (this->Xi * this->Xi - 5.0 * this->Xi + 9) / exp(this->Xi);
                this->matrix_B[i][0] = -4.0 * this->h * this->h * this->f(this->Xi);
        }
        this->matrix_B[this->row-1][0] = 20.0 / exp(2.0);
    }
}

double** DE2app::get_Matrix(int _m)
{
    if (_m == 1)
        return this->matrix_A;
    else if (_m == 2)
        return this->matrix_B;
    else
        return nullptr;
}

void DE2app::print_Matrix(int _m)
{
    if (_m == 1)
    {
        cout << "\nМатрица коэффициентов:\n" << endl;
        for (int i = 0; i < this->row; i++)
        {
            for (int j = 0; j < this->col; j++)
            {
                cout << this->matrix_A[i][j] << "   ";
            }
            cout << endl << endl;
        }
    }
    else if (_m == 2)
    {
        cout << "\nВектор свободных членов:\n" << endl;
        for (int i = 0; i < this->row; i++)
        {
            for (int j = 0; j < 1; j++)
            {
                cout << this->matrix_B[i][j] << "   ";
            }
            cout << endl;
        }
    }
    else
        throw IncorrectNumberOfMethodTyped();
}
#endif // !_DE2APP_H_