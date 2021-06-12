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
		std::cout << "\n����������� ������� �����������" << std::endl;
	}
};

class NonSquareMatrixException : public Exception
{
public:
	void ShowMessage()
	{
		std::cout << "\n������� �� �������� ����������" << std::endl;
	}
};

class BadIndexException : public Exception
{
public:
	void ShowMessage()
	{
		std::cout << "\n�������� � ������ ��������� � ������� �� ����������" << std::endl;
	}
};

class DimensionSumException : public Exception
{
public:
	void ShowMessage()
	{
		std::cout << "\n����������� ����������� ������ ������ ���������" << std::endl;
	}
};

class DimensionProductException : public Exception
{
public:
	void ShowMessage()
	{
		std::cout << "\n���������� �������� ������ ������� ������ ��������� � ����������� ����� ������ �������" << std::endl;
	}
};

class ZeroDivideException : public Exception
{
public:
	void ShowMessage()
	{
		std::cout << "\n������� �� 0" << std::endl;
	}
};

class LimitExceedingException : public Exception
{
public:
	void ShowMessage()
	{
		std::cout << "\n��������� ������� �������" << std::endl;
	}
};

class IncorrectNumberOfMethodTyped : public Exception
{
public:
	void ShowMessage()
	{
		std::cout << "\n������ �������� ����� ��� ������ ������" << std::endl;
	}
};
#endif // !_EXCEPTION_H