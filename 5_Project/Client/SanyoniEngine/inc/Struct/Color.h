#pragma once

#include "SanyoniEngine.h"

namespace SanyoniEngine
{
	struct Color : public D2D1_COLOR_F
	{
	public:
		Color() = default;

		Color(double _r, double _g, double _b, double _a);

		Color(float _r, float _g, float _b, float _a);

		Color(int _r, int _g, int _b, int _a);

		Color operator+(const Color& _other) const;

		Color operator-(const Color& _other) const;

		Color operator*(const float& _scalar) const;

		Color operator/(const float& _scalar) const;

		bool operator==(const Color& _other) const;

		bool operator!=(const Color& _other) const;

	public:
		static const Color White;
		static const Color Black;
		static const Color Gray;
		static const Color Red;
		static const Color Green;
		static const Color Blue;
	};
}

namespace std
{
	template <>
	struct hash<SanyoniEngine::Color>
	{
		std::size_t operator()(const SanyoniEngine::Color& _value) const noexcept
		{
			return (std::hash<float>()(_value.a) << 24
				^ (std::hash<float>()(_value.r) << 16)
				^ (std::hash<float>()(_value.g) << 8)
				^ (std::hash<float>()(_value.b))
			);
		}
	};
}
