#pragma once
#include "Vector2.h"

namespace SanyoniEngine
{
	class ColliderBase;

	struct RaycastHitInfo
	{
		RaycastHitInfo() :
			start(Vector2::Zero),
			direction(Vector2::Zero),
			distance(0.f),
			point(Vector2::Zero),
			collider(nullptr)
		{
		}

		Vector2 start;
		Vector2 direction;
		float distance;
		Vector2 point;
		ColliderBase* collider;

		bool operator<(const RaycastHitInfo& _other) const
		{
			return distance < _other.distance;
		}

		bool operator>(const RaycastHitInfo& _other) const
		{
			return distance > _other.distance;
		}
	};
}
