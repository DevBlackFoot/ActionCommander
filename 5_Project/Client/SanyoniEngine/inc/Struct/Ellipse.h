#pragma once

#include "Point2.h"

namespace SanyoniEngine
{
	struct Ellipse : public D2D1_ELLIPSE
	{
	public:
		Ellipse() = default;

		Ellipse(const Point2& _center, float _radiusX, float _radiusY)
			: D2D1_ELLIPSE{_center, _radiusX, _radiusY}
		{
		}

		Ellipse(float _centerX, float _centerY, float _radiusX, float _radiusY)
			: D2D1_ELLIPSE{Point2{_centerX, _centerY}, _radiusX, _radiusY}
		{
		}
	};
}
