#include <iostream>
#include <ctime>
#include <Eigen/Dense>
#include <Eigen/Sparse>
#include "Slau.h"

using namespace std;
using namespace Eigen;

double DenseM(int _m, int _n, int _N)
{
    try
    {
        double a = 0;
        double b = 2;
        double N = (double)_N;
        double h = (b - a) / N;
        int m = _m;
        int n = _n;
        //cout << "Введите количество уравнений системы:" << endl;
        //cin >> m;
        //cout << "Введите количество неизвестных:" << endl;
        //cin >> n;

        //Slau* s = new Slau(m, n);
        DE2app* de = new DE2app(m, n, N);

        de->init_Matrix();
        de->fill_Matrix(1);
        de->fill_Matrix(2);
        
        //de->print_Matrix(1);
        //de->print_Matrix(2);
        //return 1;

// EIGEN / DENSE

        MatrixXd A(m + 1, n + 1);
        VectorXd B(m + 1);

        for (int i = 0; i < m + 1; i++)
        {
            for (int j = 0; j < n + 1; j++)
            {
                A(i, j) = de->get_Matrix(1)[i][j];
            }
            B(i) = de->get_Matrix(2)[i][0];
        }

        cout << "\nМатрица коэффициентов:\n" << endl;
        cout << A << endl << endl;
        cout << "\nВектор правой части:\n" << endl;
        cout << B << endl << endl;

        //s->set_Matrix(1, de->get_Matrix(1));
        //s->set_Matrix(2, de->get_Matrix(2));
        //s->Solve();
        //s->PrintSolution();

        VectorXd x = A.colPivHouseholderQr().solve(B);

        cout << "\nВектор решений:\n" << endl;
        cout << x << endl << endl;

        cout << "\nНевязка:\n" << endl;
        cout << A * x - B << endl << endl;

        //VectorXd cn = A * x - B;
        //VectorXd cn = x - 10*;

        double max = abs(x(0) - 10 * 0 * h / exp(0));

        for (int i = 1; i < m + 1; i++)
        {
            double temp = abs(x(i) - 10 * (double)i * h / exp(double(i) * h));
            if (temp > max)
                max = temp;
        }

        cout << "max = " << max << endl;
        
        return max;

    }
    catch (Exception& e)
    {
        e.ShowMessage();
    }
}

double SparseM(int _m, int _n, int _N)
{
    // EIGEN / SPARSE
    try
    {
        double a = 0;
        double b = 2;
        double N = (double)_N;
        double h = (b - a) / N;
        int m = _m;
        int n = _n;
        //cout << "Введите количество уравнений системы:" << endl;
        //cin >> m;
        //cout << "Введите количество неизвестных:" << endl;
        //cin >> n;

        DE2app* de = new DE2app(m, n, N);

        de->init_Matrix();
        de->fill_Matrix(1);
        de->fill_Matrix(2);

        SparseMatrix<double> A1(m + 1, n + 1);

        for (int i = 0; i < m + 1; i++)
        {
            A1.insert(i, i) = de->get_Matrix(1)[i][i];
        }
        int j;
        for (int i = 0; i < m; i++)
        {
            j = i + 1;
            A1.insert(i, j) = de->get_Matrix(1)[i][j];
        }
        for (int i = 1; i < m; i++)
        {
            j = i - 1;
            A1.insert(i, j) = de->get_Matrix(1)[i][j];
        }
        
        cout << A1 << endl;

        VectorXd x(n + 1), B(n + 1);
        for (int i = 0; i < n + 1; i++)
        {
            B(i) = de->get_Matrix(2)[i][0];
        }
        cout << B << endl;

        SimplicialLDLT<SparseMatrix<double>> solver;
        solver.compute(A1);
        if (solver.info() != Success)
        {
            cout << "Decomposition failed!" << endl;
            return(1);
        }

        x = solver.solve(B);
        if (solver.info() != Success)
        {
            cout << "Solving failed!" << endl;
            return(2);
        }

        cout << "\nВектор решений:" << endl;
        cout << x << endl;
        
        cout << "\nНевязка:" << endl;
        cout << A1 * x - B << endl;

        double max = abs(x(0) - 10 * 0 * h / exp(0));
        double temp;
        for (int i = 1; i < m + 1; i++)
        {
            temp = abs(x(i) - 10 * (double)i * h / exp(double(i) * h));
            if (temp > max)
                max = temp;
        }

        cout << "\nmax = " << max << endl;

        return max;
    }
    catch (Exception& e)
    {
        e.ShowMessage();
    }
}

int main()
{
    setlocale(LC_ALL, "Russian");
    srand(unsigned int(time(NULL)));

    int inputm;
    cout << "Введите метод (1 - Eigen/Dense, 2 - Eigen/Sparse):" << endl;
    cin >> inputm;

    if (inputm == 1)
    {
        double d10 = DenseM(10, 10, 10);
        double d20 = DenseM(20, 20, 20);
        double d40 = DenseM(40, 40, 40);
        double d80 = DenseM(80, 80, 80);

        cout << "\nD10/D20 = " << d10 / d20 << endl;
        cout << "D20/D40 = " << d20 / d40 << endl;
        cout << "D40/D80 = " << d40 / d80 << endl;
    }
    else if (inputm == 2)
    {
        double ds10 = SparseM(10, 10, 10);
        double ds20 = SparseM(20, 20, 20);
        //double ds40 = SparseM(40, 40, 40);
        //double ds80 = SparseM(80, 80, 80);

        cout << "\nD10/D20 = " << ds10 / ds20 << endl;
        //cout << "D20/D40 = " << ds20 / ds40 << endl;
        //cout << "D40/D80 = " << ds40 / ds80 << endl;
    }
    else
    {
        return 0;
    }

    return 0;
}