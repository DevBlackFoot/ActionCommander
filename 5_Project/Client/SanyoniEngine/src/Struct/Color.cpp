#include "SanyoniEngine_pch.h"
#include "Color.h"

#include "Helper.h"

using namespace SanyoniEngine;

const Color Color::White = {1.f, 1.f, 1.f, 1.f};
const Color Color::Black = {0.f, 0.f, 0.f, 1.f};
const Color Color::Gray = {0.623f, 0.623f, 0.623f, 1.f};
const Color Color::Red = {1.f, 0.f, 0.f, 1.f};
const Color Color::Green = {0.f, 1.f, 0.f, 1.f};
const Color Color::Blue = {0.f, 0.f, 1.f, 1.f};

Color::Color(double _r, double _g, double _b, double _a)
{
	this->r = static_cast<float>(_r);
	this->g = static_cast<float>(_g);
	this->b = static_cast<float>(_b);
	this->a = static_cast<float>(_a);
}

Color::Color(float _r, float _g, float _b, float _a)
{
	this->r = _r;
	this->g = _g;
	this->b = _b;
	this->a = _a;
}

Color::Color(int _r, int _g, int _b, int _a)
{
	this->r = _r / 255.f;
	this->g = _g / 255.f;
	this->b = _b / 255.f;
	this->a = _a / 255.f;
}

Color Color::operator+(const Color& _other) const
{
	return Color{
		this->r + _other.r,
		this->g + _other.g,
		this->b + _other.b,
		this->a + _other.a
	};
}

Color Color::operator-(const Color& _other) const
{
	return Color{
		this->r - _other.r,
		this->g - _other.g,
		this->b - _other.b,
		this->a - _other.a
	};
}

Color Color::operator*(const float& _scalar) const
{
	return Color{
		this->r * _scalar,
		this->g * _scalar,
		this->b * _scalar,
		this->a * _scalar
	};
}

Color Color::operator/(const float& _scalar) const
{
	return Color{
		this->r / _scalar,
		this->g / _scalar,
		this->b / _scalar,
		this->a / _scalar
	};
}

bool Color::operator==(const Color& _other) const
{
	return MathHelper::AlmostEquals(this->r, _other.r) &&
		MathHelper::AlmostEquals(this->g, _other.g) &&
		MathHelper::AlmostEquals(this->b, _other.b) &&
		MathHelper::AlmostEquals(this->a, _other.a);
}

bool Color::operator!=(const Color& _other) const
{
	return !(*this == _other);
}
