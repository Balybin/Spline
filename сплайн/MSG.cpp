#pragma once
#include "Task.h"

#define FORMAT "%0.14e\n"
using namespace std;

struct matrix{
	vector <double> di, al, au;
};

vector<double> r, r1, z, z1, Az,  Azk1;	//Дополнительные вектора
vector<double> Mul;
vector<double> L, U;
vector<int> ia, ja;

matrix A, At, LU;

vector<double> f, x, res;
int N; //n - размер матрицы, m - кол-во внедиагональных элементов
int maxiter = 1000000, iter; //Максимальное кол-во итераций (защита от зацикливания), текущая итерация
int method, method2;
double eps = 1e-20; //Погрешность вычислений
double w; //Параметр релаксации
double discrep;
double e2;
long t1, t2;
double a, b;
int k;

void CreateMatrix(matrix &M)
{
	M.di.resize(N);
	M.al.resize(ia[N]);
	M.au.resize(ia[N]);
}

matrix Transpose(matrix M)
{
	vector<double> H;
	H = M.al;
	M.al = M.au;
	M.au = H;
	return M;
}

vector<double> mul_Matrix_Vector(vector<double> vec, matrix M)
{
	vector<double> result;
	result.resize(N);
	for (int i = 0; i < N; i++)
	{
		result[i] = M.di[i] * vec[i];

		for (int k = ia[i]; k < ia[i + 1]; k++)
		{
			result[i] += M.al[k] * vec[ja[k]];
			result[ja[k]] += M.au[k] * vec[i];
		}
	}
	return result;
}

double scalar(vector<double> vec1, vector<double> vec2)
{
	double res = 0;
	for (int i = 0; i < N; i++)
		res += vec1[i] * vec2[i];
	return res;
}

double norma(vector<double> vec)
{
	double res = scalar(vec, vec);
	return sqrt(res);
}

//----------------------------ОБХОДЫ-----------------------------------------

vector<double> Direct(vector<double> F, vector<double> M) // прямой ход Ly=F
{
	for (int i = 0; i < N; i++)
	{
		F[i] /= M[i];
	}
	return F;
}

vector<double> Reverse(vector<double> F, vector<double> M) // обратный ход Ux=y
{
	for (int i = N - 1; i >= 0; i--)
	{
		F[i] = F[i] / M[i];
	}
	return F;
}

vector<double> Direct_LU_L(vector<double> F, matrix M) // прямой ход Ly=F
{
	for (int i = 0; i < N; i++)
	{
		double sum = 0;
		for (int k = ia[i]; k < ia[i + 1]; k++)
		{
			sum += F[ja[k]] * M.al[k];
		}
		F[i] = (F[i] - sum) / M.di[i];
	}
	return F;
}

vector<double> Reverse_LU_L(vector<double> F, matrix M) 
{
	for (int i = N - 1; i >= 0; i--)
	{
		F[i] /= M.di[i];
		for (int k = ia[i]; k < ia[i + 1]; k++)
		{
			F[ja[k]] -= F[i] * M.al[k];
		}
	}
	return F;
}

vector<double> Direct_LU_U(vector<double> F, matrix M) // прямой ход Ly=F
{
	for (int i = 0; i < N; i++)
	{
		double sum = 0;
		for (int k = ia[i]; k < ia[i + 1]; k++)
		{
			sum += F[ja[k]] * M.au[k];
		}
		F[i] = (F[i] - sum);
		if(method2 == 2)
			F[i] /= M.di[i];
	}
	return F;
}

vector<double> Reverse_LU_U(vector<double> F, matrix M) // обратный ход Ux=y
{
	for (int i = N - 1; i >= 0; i--)
	{
		if (method2 == 2)
			F[i] /= M.di[i];
		for (int k = ia[i]; k < ia[i + 1]; k++)
		{
			F[ja[k]] -= F[i] * M.au[k];
		}
	}
	return F;
}

void LUp(vector<double> &L, vector<double> &U)
{
	for (int i = 0; i < N; i++)
		U[i] = 1;
	L = A.di;
}

void LUz(vector<double> &L, vector<double> &U)
{
	for (int i = 0; i < N; i++)
		L[i] = 1;
	U = A.di;
}

void LUsq(vector<double> &L, vector<double> &U)
{
	for (int i = 0; i < N; i++)
	{
		U[i] = sqrt(A.di[i]);
		L[i] = sqrt(A.di[i]);
	}
}

void LU_decomposition(matrix &LU)
{
	for (int i = 0; i < N; i++)
	{
		double bdi = 0;
		int i0 = ia[i];
		int i1 = ia[i + 1];
		for (int k = i0; k < i1; k++)
		{
			int j = ja[k];
			double bal = 0;
			double bau = 0;
			for (int ki = i0; ki < k; ki++)
			{
				for (int kj = ia[j]; kj < ia[j+1]; kj++)
				{
					if (ja[kj] == ja[ki])
					{
						bal += LU.al[ki] * LU.au[kj];
						bau += LU.au[ki] * LU.al[kj];
						break;
					}
				}
			}
			LU.al[k] = A.al[k] - bal;
			LU.au[k] = A.au[k] - bau;
			LU.au[k] = LU.au[k] / LU.di[ja[k]];
			bdi += LU.al[k] * LU.au[k];
		}
		LU.di[i] = A.di[i] - bdi;
	}

}

void LUsq_decomposition(matrix &LU)
{
	for (int i = 0; i < N; i++)
	{
		double bdi = 0;
		int i0 = ia[i];
		int i1 = ia[i + 1];
		for (int k = i0; k < i1; k++)
		{
			int j = ja[k];
			double bal = 0;
			double bau = 0;
			for (int ki = i0; ki < k; ki++)
			{
				for (int kj = ia[j]; kj < ia[j + 1]; kj++)
				{
					if (ja[kj] == ja[ki])
					{
						bal += LU.al[ki] * LU.au[kj];
						bau += LU.au[ki] * LU.al[kj];
						break;
					}
				}
			}
			LU.al[k] = (A.al[k] - bal) / LU.di[ja[k]];
			LU.au[k] = (A.au[k] - bau) ;
			LU.au[k] = LU.au[k] / LU.di[ja[k]];
			bdi += LU.al[k] * LU.au[k];
		}
		LU.di[i] = sqrt(A.di[i] - bdi);
	}
}

//--------------------------Методы----------------------------------------------

void MSG()
{
	At = Transpose(A);
	r = mul_Matrix_Vector(f, At);
	z = r;
	e2 = norma(r) / norma(f);

	for (k = 1; k < maxiter && e2 > eps; k++)
	{
		z1 = z;
		r1 = r;
		Azk1 = mul_Matrix_Vector(z, A);
		Azk1 = mul_Matrix_Vector(Azk1, At);

		a = scalar(r, r) / scalar(Azk1, z);

		for (int i = 0; i < N; i++)
		{
			x[i] += a*z[i];
			r[i] -= a*Azk1[i];
		}
		b = scalar(r, r) / scalar(r1, r1);
		for (int i = 0; i < N; i++)
		{
			z[i] = r[i] + b*z1[i];
		}

		e2 = norma(r) / norma(f);
	}
	if (k == maxiter)
	{
		cout << "Вышел по макс. итерации, решение может быть неправильным" << endl;
		cin.get();
	}
}

void MSG_LU()
{
	switch (method2)
	{
		case 1: LU_decomposition(LU);		break;
		case 2: LUsq_decomposition(LU);		break;
	}

	At = Transpose(A);

	r = Direct_LU_L(f, LU);
	r = Reverse_LU_L(r, LU);
	r = mul_Matrix_Vector(r, At);
	r = Direct_LU_U(r, LU); 
	z = r;		
	e2 = norma(r) / norma(f);

	for (k = 1; k < maxiter && e2 > eps; k++)
	{
		z1 = z;
		r1 = r;

		Azk1 = Reverse_LU_U(z, LU);
		Azk1 = mul_Matrix_Vector(Azk1, A);
		Azk1 = Direct_LU_L(Azk1, LU);
		Azk1 = Reverse_LU_L(Azk1, LU);
		Azk1 = mul_Matrix_Vector(Azk1, At);
		Azk1 = Direct_LU_U(Azk1, LU);

		a = scalar(r, r) / scalar(Azk1, z);	
		for (int i = 0; i < N; i++)	
		{
			x[i] += a*z[i];
			r[i] -= a*Azk1[i];
		}
		b = scalar(r, r) / scalar(r1, r1);
		for (int i = 0; i < N; i++)
			z[i] = r[i] + b*z1[i];
		e2 = norma(r) / norma(f);
	}
	x = Reverse_LU_U(x, LU);
	if (k == maxiter)
	{
		cout << "Вышел по макс. итерации, решение может быть неправильным" << endl;
		cin.get();
	}
}

vector<double> makeSLAU(vector<double> &_di, vector<double> &_al, vector<double> &_au,
	vector<int> &_ia, vector<int> &_ja, vector<double> &_F)
{
	ia = _ia;
	ja = _ja;
	A.di = _di;
	A.al = _al;
	A.au = _au;
	N = _di.size();
	method2 = 2;
	CreateMatrix(LU);
	r.resize(N, 0);
	f.resize(N);
	x.resize(N);
	r1.resize(N, 0);
	z.resize(N, 0);
	z1.resize(N, 0);
	Azk1.resize(N, 0);
	L.resize(N, 0);
	U.resize(N, 0);
	f = _F;
	MSG_LU();
	return x;
}
//------------------------------------------------------------------------------

//void main()
//{
//	switch (method)
//	{
//	case 1:	MSG();		break;
//	case 3: MSG_LU();	break;
//	default: printf("Error");
//	}
//	system("pause");
//}