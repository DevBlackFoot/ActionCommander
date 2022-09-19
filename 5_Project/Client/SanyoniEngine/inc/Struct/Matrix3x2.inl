#pragma once

#include "Helper.h"
#include "Matrix3x2.h"

using namespace SanyoniEngine;

inline Matrix3x2 Matrix3x2::operator+(const Matrix3x2& _other) const
{
	return {
		this->_11 + _other._11,
		this->_12 + _other._12,
		this->_21 + _other._21,
		this->_22 + _other._22,
		this->_31 + _other._31,
		this->_32 + _other._32
	};
}

inline Matrix3x2 Matrix3x2::operator-(const Matrix3x2& _other) const
{
	return {
		this->_11 - _other._11,
		this->_12 - _other._12,
		this->_21 - _other._21,
		this->_22 - _other._22,
		this->_31 - _other._31,
		this->_32 - _other._32
	};
}

inline Matrix3x2 Matrix3x2::operator*(const Matrix3x2& _other) const
{
	return {
		this->_11 * _other._11 + this->_12 * _other._21,
		this->_11 * _other._12 + this->_12 * _other._22,
		this->_21 * _other._11 + this->_22 * _other._21,
		this->_21 * _other._12 + this->_22 * _other._22,
		this->_31 * _other._11 + this->_32 * _other._21 + _other._31,
		this->_31 * _other._12 + this->_32 * _other._22 + _other._32
	};
}

inline Matrix3x2 Matrix3x2::operator*(float _scalar) const
{
	return {
		this->_11 * _scalar,
		this->_12 * _scalar,
		this->_21 * _scalar,
		this->_22 * _scalar,
		this->_31 * _scalar,
		this->_32 * _scalar
	};
}

inline bool Matrix3x2::operator==(const Matrix3x2&& _other) const
{
	return MathHelper::AlmostEquals(this->_11, _other._11) &&
		MathHelper::AlmostEquals(this->_12, _other._12) &&
		MathHelper::AlmostEquals(this->_21, _other._21) &&
		MathHelper::AlmostEquals(this->_22, _other._22) &&
		MathHelper::AlmostEquals(this->_31, _other._31) &&
		MathHelper::AlmostEquals(this->_32, _other._32);
}

inline Vector2 Matrix3x2::GetPosition() const
{
	return {this->_31, this->_32};
}

inline float Matrix3x2::GetRotationRad() const
{
	const auto _direction = Vector2(this->_22, this->_21).Normalize();
	const float _rad = _direction.y >= 0
		                   ? std::acos(_direction.x)
		                   : std::acos(_direction.x) * -1;

	return _rad;
}

inline float Matrix3x2::GetRotationDeg() const
{
	const float _rad = GetRotationRad();
	return _rad * RAD_TO_DEG;
}

inline Vector2 Matrix3x2::GetScale() const
{
	const float _xScale = sqrt(this->_11 * this->_11 + this->_12 * this->_12);
	const float _yScale = sqrt(this->_21 * this->_21 + this->_22 * this->_22);

	return Vector2{_xScale, _yScale};
}

Matrix3x2 Matrix3x2::MakeTranslateMatrix(const Vector2& _delta)
{
	const auto _translate = Matrix3x2{
		1.f,
		0.f,
		0.f,
		1.f,
		_delta.x,
		_delta.y
	};

	return _translate;
}

Matrix3x2 Matrix3x2::MakeRotateMatrixWithRad(const float _radian)
{
	const auto _rotate = Matrix3x2{
		cos(_radian),
		-sin(_radian),
		sin(_radian),
		cos(_radian),
		0.f,
		0.f
	};

	return _rotate;
}

Matrix3x2 Matrix3x2::MakeRotateMatrixWithDeg(const float _degree)
{
	const float _radian = _degree * DEG_TO_RAD;
	return MakeRotateMatrixWithRad(_radian);
}

Matrix3x2 Matrix3x2::MakeScaleMatrix(const Vector2& _newScale, const Point2& _center)
{
	const auto _scale = Matrix3x2{
		_newScale.x,
		0.f,
		0.f,
		_newScale.y,
		_center.x - _newScale.x * _center.x,
		_center.y - _newScale.y * _center.y
	};

	return _scale;
}
