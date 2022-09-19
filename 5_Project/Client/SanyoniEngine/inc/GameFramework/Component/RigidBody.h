#pragma once

#include "SanyoniEngine_types.h"
#include "ComponentBase.h"
#include "EGameObjectTag.h"
#include "RaycastHitInfo.h"

namespace SanyoniEngine
{
	class GameObject;
	class ColliderBase;

	interface IRigidBodyController abstract
	{
		virtual void OnPreMove(Vector2& _refDeltaVelocity) abstract;

		virtual void OnPostMove(Vector2& _refDeltaVelocity) abstract;

		virtual bool OnVerticalCollision(const RaycastHitInfo& _hitInfo, Vector2& _refDeltaVelocity) abstract;

		virtual bool OnHorizontalCollision(const RaycastHitInfo& _hitInfo, Vector2& _refDeltaVelocity) abstract;
	};

	class RigidBody :
		public ComponentBase
	{
	public:
		RigidBody(GameObject* _gameObject, tstring _typeName = TEXT("RigidBody"));

		void Move(Vector2& _refDeltaVelocity);

		void DoCollisions(Vector2& _refDeltaVelocity);

	private:
		void VerticalCollisions(ColliderBase* _collider, const Vector2& _deltaVelocity, std::vector<RaycastHitInfo>& _outHitInfos);

		void HorizontalCollisions(ColliderBase* _collider, const Vector2& _deltaVelocity, std::vector<RaycastHitInfo>& _outHitInfos);

	private:
		std::unordered_set<EGameObjectTag> m_ExcludeCollision;

	public:
		void SetExcludeCollsion(std::unordered_set<EGameObjectTag> _exclude) { m_ExcludeCollision = _exclude; }
	};
}
