#pragma once
#include "Matrices.h"

using namespace Matrices;
class Matrices::RotationMatrix : public Matrix {
public:
	RotationMatrix(double theta);
};
