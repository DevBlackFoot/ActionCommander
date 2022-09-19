#pragma once
#include "ComponentBase.h"
#include "RigidBody.h"

namespace SanyoniEngine
{
	class RigidBodyController :
		public ComponentBase,
		public IRigidBodyController
	{
	public:
		RigidBodyController(GameObject* _gameObject, tstring _typeName = TEXT("RigidBodyController"));

		void AddForce(const Vector2& _force);

		void Move();

		void OnPreMove(Vector2& _refDeltaVelocity) override;

		void OnPostMove(Vector2& _refDeltaVelocity) override;

		bool OnVerticalCollision(const RaycastHitInfo& _hitInfo, Vector2& _refDeltaVelocity) override;

		bool OnHorizontalCollision(const RaycastHitInfo& _hitInfo, Vector2& _refDeltaVelocity) override;

	public:
		Vector2 deltaVelocity;

		static constexpr float GRAVITY = 9.81f;

		static constexpr float GRAVITY_MULTIPLIER = 3.5f;

	protected:
		float dragX;

		float additionalDeltaVelocityX;

		bool m_bOverride;

		Vector2 m_OverrideDeltaVelocity;

		bool m_bStop;

	public:
		inline Vector2 GetOverrideDeltaVelocity() const { return m_OverrideDeltaVelocity; }

		inline void SetOverrideDeltaVelocity(const Vector2& _velocity)
		{
			m_bOverride = true;
			m_OverrideDeltaVelocity = _velocity;
		}

		void Stop() { m_bStop = true; }
	};
}
