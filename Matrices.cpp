#include "Matrices.h"
#include <iostream>
#include <vector>
#include <iomanip>

using namespace std;

namespace Matrices
{
	Matrix::Matrix(int _rows, int _cols) : rows(_rows), cols(_cols)
	{
		this->a.assign(rows, vector<double>(cols, 0));
	}

	Matrix operator+(const Matrix& a, const Matrix& b)
	{
		if (a.getRows() != b.getRows() || a.getCols() != b.getCols())
		{
			throw runtime_error("Error: dimensions of matrices must agree.");
		}


		Matrix result(a.getRows(), a.getCols());
		for (int i = 0; i < result.getRows(); i++)
		{
			for (int j = 0; j < result.getCols(); j++)
			{
				result(i, j) = a(i, j) + b(i, j);
			}
		}
		return result;

	}

	Matrix operator*(const Matrix& a, const Matrix& b) 
	{
		if (a.getCols() != b.getRows()) 
		{
			throw runtime_error("Error: dimensions of matrices must agree.");
		}

		Matrix result(a.getRows(), b.getCols());

		for (int k = 0; k < a.getRows(); ++k) 
		{
			for (int i = 0; i < b.getCols(); ++i) 
			{
				for (int j = 0; j < a.getCols(); ++j) 
				{
					result(k, i) += a(k, j) * b(j, i);
				}
			}
		}

		return result;
	}

	bool operator==(const Matrix& a, const Matrix& b) 
	{
		if (a.getRows() != b.getRows() || a.getCols() != b.getCols()) 
		{
			return false;
		}

		for (int i = 0; i < a.getRows(); ++i) 
		{
			for (int j = 0; j < a.getCols(); ++j) 
			{
				if ((abs(a(i, j) - b(i, j))) > 0) 
				{
					return false;
				}
			}
		}

		return true;
	}
	bool operator!=(const Matrix& a, const Matrix& b) 
	{
		return !(a == b);
	}

	ostream& operator<<(ostream& os, const Matrix& a) 
	{
		for (int i = 0; i < a.getRows(); ++i) 
		{
			for (int j = 0; j < a.getCols(); ++j) 
			{
				os << setw(8) << a(i, j);
				if (j != a.getCols() - 1) 
				{
					os << " ";
				}
			}
			os << "\n";
		}

		return os;

	}

	TranslationMatrix::TranslationMatrix(double xShift, double yShift, int nCols) : Matrix(2, nCols)
	{
    		for (unsigned short i = 0; i < nCols; i++) 
   		 {
      		  (*this)(0, i) = xShift;
    		  (*this)(1, i) = yShift;
   		 }
	}

	ScalingMatrix::ScalingMatrix(double scale) : Matrix(2, 2) 
	{
    		(*this)(0, 0) = scale;
   		(*this)(0, 1) = 0;
  		(*this)(1, 0) = 0;
  		(*this)(1, 1) = scale;
	}

	RotationMatrix::RotationMatrix(double theta) : Matrix(2, 2) 
	{
		    (*this)(0, 0) = cos(theta);
		    (*this)(0, 1) = -sin(theta);
		    (*this)(1, 0) = sin(theta);
		    (*this)(1, 1) = cos(theta);
	}



}
