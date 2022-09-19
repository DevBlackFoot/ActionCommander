#pragma once

#include "ColliderBase.h"
#include "Collision.h"

namespace SanyoniEngine
{
	class CircleCollider final
		: public ColliderBase
	{
	public :
		CircleCollider(class GameObject* _gameObject, tstring _typeName = TEXT("CircleCollider"));

		void OnRender() override;

		void UpdateBounds() override;

	protected:
		bool SimpleCollision(ColliderBase* _other) override;

	private:
		float m_Radius;

		/**
		 * \brief 충돌 영역입니다.
		 */
		Collision::CircleBounds m_Bounds;

		/**
		 * \brief 이 collider의 Bounds가 가장 마지막으로 갱신되었을 때 <br>
		 * 이 collider가 부착되어 있는 오브젝트의 월드 행렬입니다. <br>
		 * UpdateBounds()를 호출했을 때, 오브젝트의 월드 행렬과 이 값을 비교하여 <br>
		 * bounds를 갱신해야하는지, 할 필요가 없는지를 판별할 수 있습니다.
		 */
		Matrix3x2 m_WorldTMAtLastBoundsUpdated;

	public:
		inline float GetRadius() const { return m_Radius; }

		inline Collision::CircleBounds GetBounds()
		{
			UpdateBounds();
			return m_Bounds;
		}

		inline void SetRadius(float _radius)
		{
			if (m_Radius == _radius)
				return;

			m_Radius = _radius;
			m_bDirty = true;
		}
	};
}
