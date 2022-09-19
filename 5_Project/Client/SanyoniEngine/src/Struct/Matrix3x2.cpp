#include "SanyoniEngine_pch.h"
#include "Matrix3x2.h"

using namespace SanyoniEngine;

const Matrix3x2 Matrix3x2::Identity{1.f, 0.f, 0.f, 1.f, 0.f, 0.f};

Matrix3x2::Matrix3x2(FLOAT m11, FLOAT m12, FLOAT m21, FLOAT m22, FLOAT m31, FLOAT m32)
	: Matrix3x2F(m11, m12, m21, m22, m31, m32)
{
}
