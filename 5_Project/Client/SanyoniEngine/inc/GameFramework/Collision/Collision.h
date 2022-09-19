#pragma once

#include "CollisionBounds.h"
#include "RaycastHitInfo.h"

namespace SanyoniEngine::Collision
{
	class AABB
	{
	private:
		/**
		 * \brief Box와 Circle의 충돌을 검사할 때 사용하는 열거형 클래스입니다.
		 * Box의 영역을 Center로 두고, Circle이 Box로부터
		 * 상대적으로 어디에 있는지에 대한 것을 나타내기 위해 사용합니다.
		 *
		 * 각 enum값은 이름에 대응되는 성분 bit들이 1인 값입니다.
		 * 0b1000: left 성분 bit입니다.
		 * 0b0100: top 성분 bit입니다.
		 * 0b0010: right 성분 bit입니다.
		 * 0b0100: bottom 성분 bit입니다.
		 */
		enum class RectZoneCase
		{
			LEFT = 0b1000,
			LEFT_BOTTOM = 0b1001,
			LEFT_TOP = 0b1100,
			CENTER = 0b0000,
			BOTTOM = 0x0001,
			TOP = 0b0100,
			RIGHT = 0b0010,
			RIGHT_BOTTOM = 0b0011,
			RIGHT_TOP = 0b0110
		};

		static constexpr int RectZoneCase_LeftBit = 0b1000;
		static constexpr int RectZoneCase_TopBit = 0b0100;
		static constexpr int RectZoneCase_RightBit = 0b0010;
		static constexpr int RectZoneCase_BottomBit = 0b0001;

	public:
		static bool OverlapAABB(const AABBBounds& _lhs, const AABBBounds& _rhs);

		static bool OverlapCircle(const AABBBounds& _box, const CircleBounds& _circle);

		static bool Overlap(const AABBBounds& _box, ColliderBase* _collider);

	private:
		static RectZoneCase GetRectZone(const AABBBounds& _box, const CircleBounds& _circle);
	};

	class OBB
	{
	public:
		static bool SimpleBoxAndBox(const OBBBounds& _lhs, const OBBBounds& _rhs);
	};

	class Circle
	{
	public:
		static bool OverlapCircle(const CircleBounds& _lhs, const CircleBounds& _rhs);

		static bool OverlapAABB(const CircleBounds& _circle, const AABBBounds& _box);

		static bool Overlap(const CircleBounds& _circle, ColliderBase* _collider);
	};

	class Ray
	{
	public:
		static bool RayIntersection(const RayBounds& _lhs, const RayBounds& _rhs, Vector2& _outPosition, float& _outDistance);

		static bool RaycastAABB(const RayBounds& _ray, const AABBBounds& _box, RaycastHitInfo& _outHit);

		static bool RaycastCircle(const RayBounds& _ray, const CircleBounds& _circle, RaycastHitInfo& _outHit);

		static bool Raycast(const RayBounds& _ray, ColliderBase* _collider, RaycastHitInfo& _outHit);
	};
}
