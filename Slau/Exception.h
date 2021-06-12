#pragma once
#ifndef _EXCEPTION_H
#define _EXCEPTION_H

#include <iostream>

class Exception
{
public:
	virtual void ShowMessage() = 0;
};

class BadDimensionException : public Exception
{
public:
	void ShowMessage()
	{
		std::cout << "\nРазмерности матрицы некорректны" << std::endl;
	}
};

class NonSquareMatrixException : public Exception
{
public:
	void ShowMessage()
	{
		std::cout << "\nМатрица не является квадратной" << std::endl;
	}
};

class BadIndexException : public Exception
{
public:
	void ShowMessage()
	{
		std::cout << "\nЭлемента с такими индексами в матрице не существует" << std::endl;
	}
};

class DimensionSumException : public Exception
{
public:
	void ShowMessage()
	{
		std::cout << "\nРазмерности суммируемых матриц должны совпадать" << std::endl;
	}
};

class DimensionProductException : public Exception
{
public:
	void ShowMessage()
	{
		std::cout << "\nКоличество столбцов первой матрицы должно совпадать с количеством строк второй матрицы" << std::endl;
	}
};

class ZeroDivideException : public Exception
{
public:
	void ShowMessage()
	{
		std::cout << "\nДеление на 0" << std::endl;
	}
};

class LimitExceedingException : public Exception
{
public:
	void ShowMessage()
	{
		std::cout << "\nПревышены размеры матрицы" << std::endl;
	}
};

class IncorrectNumberOfMethodTyped : public Exception
{
public:
	void ShowMessage()
	{
		std::cout << "\nУказан неверный номер для выбора метода" << std::endl;
	}
};
#endif // !_EXCEPTION_H