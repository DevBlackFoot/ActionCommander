#pragma once

#include <dcommon.h>

#include "SanyoniEngine_types.h"

namespace SanyoniEngine
{
	class Matrix3x2 : public D2D1::Matrix3x2F
	{
	public:
		Matrix3x2() = default;

		Matrix3x2(FLOAT m11, FLOAT m12,
		          FLOAT m21, FLOAT m22,
		          FLOAT m31, FLOAT m32);

		inline Matrix3x2 operator+(const Matrix3x2& _other) const;

		inline Matrix3x2 operator-(const Matrix3x2& _other) const;

		inline Matrix3x2 operator*(const Matrix3x2& _other) const;

		inline Matrix3x2 operator*(float _scalar) const;

		inline bool operator==(const Matrix3x2&& _other) const;

		inline Vector2 GetPosition() const;

		inline float GetRotationRad() const;

		inline float GetRotationDeg() const;

		inline Vector2 GetScale() const;

	public:
		static const Matrix3x2 Identity;

		static inline Matrix3x2 MakeTranslateMatrix(const Vector2& _delta);

		static inline Matrix3x2 MakeRotateMatrixWithRad(const float _radian);

		static inline Matrix3x2 MakeRotateMatrixWithDeg(const float _degree);

		static inline Matrix3x2 MakeScaleMatrix(const Vector2& _newScale, const Point2& _center = Point2::Zero);

	private:
		static constexpr float RAD_TO_DEG = 57.295779513f;

		static constexpr float DEG_TO_RAD = 0.01745329252f;
	};
}

#include "Matrix3x2.inl"
