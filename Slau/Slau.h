#pragma once
#ifndef _SLAU_H_
#define _SLAU_H_
#include <iostream>
#include "Matrix.h"
#include "Exception.h"
#include "DE2app.h"
using namespace std;

class Slau
{
private:
	int m, n;
	Matrix* a, * b, * x;
	bool isSolved;
	int* reoder;
	int rang;
public:
	Slau(int, int);
	~Slau();
	void set_Matrix(int, double**);
	void Solve();
	void Kramer();
	void InverseMatrix();
	void JordanGauss();
	void PrintSolution();
	friend ostream& operator<< (ostream&, Slau&);
	//friend istream& operator>> (istream&, Slau&);
};

Slau::Slau(int _m, int _n)
{
	this->m = _m + 1;
	this->n = _n + 1;

	this->a = new Matrix(this->m, this->n);
	this->a->init_Matrix();
	//cout << "Введите матрицу коэффициентов:\n" << endl;
	//this->a->fillMatrix(1);

	this->b = new Matrix(this->m, 1);
	this->b->init_Matrix();
	//cout << "Введите вектор свободных членов:\n" << endl;
	//this->b->fillMatrix(2);

	this->x = new Matrix(this->m, 1);
	this->reoder = new int[this->n];
	for (int i = 0; i < this->n; i++)
		this->reoder[i] = i;

	this->isSolved = false;
	this->rang = 0;
}

Slau::~Slau()
{}

void Slau::Kramer()
{
	if (this->m != this->n)
		throw NonSquareMatrixException();
	double det = this->a->Determinant();
	if (det == 0)
		throw ZeroDivideException();
	this->rang = this->m;

	double** temp_a = new double* [this->m];		// временная переменная для хранения матрицы коэффициентов с заменяемым j-м столбцом
	for (int i = 0; i < this->n; i++)
		temp_a[i] = new double[this->n];
	
	temp_a = this->a->get_Matrix();
	
	Matrix temp(this->m, this->n);					// временный объект для вычисления детерминанта Δj
	temp.set_Matrix(temp_a);

	double** old_temp_a = new double*[this->m];		// временная переменная для хранения матрицы коэффициентов
	for (int i = 0; i < this->n; i++)
		old_temp_a[i] = new double[this->n];

	double** temp_x = this->x->get_Matrix();		// временная переменная для хранения значений вектора решений

	for (int j = 0; j < this->n; j++)
	{
		for (int i = 0; i < this->m; i++)
		{
			old_temp_a[i][j] = temp_a[i][j];
			temp_a[i][j] = this->b->get_Matrix()[i][0];
		}

		temp.set_Matrix(temp_a);
		temp_x[j][0] = temp.Determinant() / det;

		for (int i = 0; i < this->m; i++)
			temp_a[i][j] = old_temp_a[i][j];
	}
	this->x->set_Matrix(temp_x);
	this->isSolved = true;
}

void Slau::set_Matrix(int _m, double** _matrix)
{
	if (_m == 1)
		this->a->set_Matrix(_matrix);
	if (_m == 2)
		this->b->set_Matrix(_matrix);
}

void Slau::InverseMatrix()
{
	if (this->m != this->n)
		throw NonSquareMatrixException();
	Matrix obr(this->m, this->n);
	obr = ~*a;
	double** mul = obr.mul_Matrix(*this->b);
	this->x->set_Matrix(mul);
	this->rang = this->m;
	this->isSolved = true;
}

void Slau::JordanGauss()
{
	double** A = this->a->get_Matrix();
	double** B = this->b->get_Matrix();
	//Matrix A = *this->a;
	//Matrix B = *this->b;
	int count_null_cols = 0;
	for (int i = 0; i < this->m; i++)
	{
		if (A[i][i] != 0)
		{
			for (int k = 0; k < this->m; k++)
			{
				if (k == i)
					continue;
				double d = A[k][i] / A[i][i];
				for (int j = i; j < this->n; j++)
					A[k][j] = A[k][j] - d * A[i][j];
				B[k][0] = B[k][0] - d * B[i][0];
			}
			for (int j = i + 1; j < this->n; j++)
				A[i][j] /= A[i][i];
			B[i][0] /= A[i][i];
			A[i][i] = 1;
		}
		else
		{
			int k;
			for (k = i + 1; k < this->m; k++)
				if (A[k][i] != 0)
					break;
			if (k == this->m)
			{
				if (i == this->n - 1 - count_null_cols)
				{
					count_null_cols++;
					break;
				}
				for (int j = 0; j < this->m; j++)
				{
					double t = A[j][i];
					A[j][i] = A[j][this->n - count_null_cols - 1];
					A[j][this->n - count_null_cols - 1] = t;
				}
				int t = this->reoder[i];
				this->reoder[i] = this->reoder[this->n - count_null_cols - 1];
				this->reoder[this->n - count_null_cols - 1] = t;
				count_null_cols++;
				i--;
			}
			else
			{
				double* t = A[i];
				A[i] = A[k];
				double p = B[i][0];
				B[i][0] = B[k][0];
				B[k][0] = p;
				i--;
			}
		}
	}
	this->rang = this->m < this->n - count_null_cols ? this->m : this->n - count_null_cols;
	int null_rows = this->m - this->rang;
	for (int i = this->rang; i < this->m; i++)
		if (B[i][0] != 0)
		{
			this->isSolved = false;
			return;
		}
	Matrix res(this->rang, 1 + this->n - this->rang);
	for (int i = 0; i < this->rang; i++)
	{
		res.get_Matrix()[i][0] = B[i][0];
		for (int j = this->rang; j < this->n; j++)
			res.get_Matrix()[i][j - this->rang + 1] = -A[i][j];
	}
	this->x = &res;
	this->isSolved = true;
}

void Slau::Solve()
{
	if (m == n)
	{
		cout << "\nМетод Крамера - 1\nМетод обратной матрицы - 2:" << endl;
		int i;
		cin >> i;
		if (i == 1)
			this->Kramer();
		else if (i == 2)
			this->InverseMatrix();
		else
			throw IncorrectNumberOfMethodTyped();
	}
	else
	{
		cout << "\nМетод Жордана-Гаусса:\n" << endl;
		this->JordanGauss();
	}
}

void Slau::PrintSolution()
{
	if (!this->isSolved)
	{
		cout << "\nСистема несовместна" << endl;
		return;
	}
	cout << "\nВектор решений:\n";
	if (this->rang < this->n)
	{
		for (int i = 0; i < this->rang; i++)
		{
			cout << "x" << (this->reoder[i] + 1) << " = " << this->x->get_Matrix()[i][0];
			for (int j = 1; j < this->n - this->rang; j++)
			{
				if (this->x->get_Matrix()[i][j] == 0)
					continue;
				if (this->x->get_Matrix()[i][j] > 0)
					cout << "+" << this->x->get_Matrix()[i][j] << "*x" << (this->reoder[this->rang + j - 1] + 1);
				else
					cout << this->x->get_Matrix()[i][j] << "*x" << (this->reoder[this->rang + j - 1] + 1);
			}
			cout << endl;
		}
	}
	else
	{
		cout << "X = (";
		for (int i = 0; i < this->n - 1; i++)
			cout << this->x->get_Matrix()[i][0] << ", ";
		cout << this->x->get_Matrix()[this->n - 1][0] << ")" << endl;
	}
}

ostream& operator<< (ostream& out, Slau& ob)
{
	for (int i = 0; i < ob.m; i++)
	{
		for (int j = 0; j < ob.n; j++)
			out << ob.a->get_Matrix()[i][j] << "\t";
		out << "\t" << ob.b->get_Matrix()[i][0] << endl;
	}
	try
	{
		ob.PrintSolution();
	}
	catch (Exception& e)
	{
		e.ShowMessage();
	}
	return out;
}

/* istream& operator>> (istream& in, Slau& ob)
{
	cout << "Матрица коэффициентов: ";
	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 3; j++)
			in >> ob.a->set_Matrix();


	in >> ob.a->fillMatrix();
	cout << "Вектор свободных членов: ";
	in >> *ob.b;
	return in;
} */
#endif // _SLAU_H_