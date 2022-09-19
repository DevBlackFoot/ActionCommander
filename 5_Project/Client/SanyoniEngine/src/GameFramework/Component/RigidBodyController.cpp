#include "SanyoniEngine_pch.h"
#include "RigidBodyController.h"

#include "GameObject.h"
#include "GameTime.h"

RigidBodyController::RigidBodyController(GameObject* _gameObject, tstring _typeName) :
	ComponentBase(_gameObject, _typeName),
	deltaVelocity(Vector2::Zero),
	dragX(7.5f),
	additionalDeltaVelocityX(0.f)
{
}

void RigidBodyController::AddForce(const Vector2& _force)
{
	deltaVelocity.y += _force.y;
	additionalDeltaVelocityX += _force.x;
}

void RigidBodyController::Move()
{
	const auto _rigidBody = m_GameObject->GetComponent<RigidBody>();
	if (_rigidBody == nullptr)
		return;


	if (m_bStop == true)
	{
		m_bStop = false;

		// 이번 프레임에 stop한다면
		// 모든 velocity를 0으로 초기화합니다.

		deltaVelocity = Vector2::Zero;
		additionalDeltaVelocityX = 0;
		m_bOverride = false;
		m_OverrideDeltaVelocity = Vector2::Zero;
	}
	else if (m_bOverride == true)
	{
		// 이번 프레임에 velocity를 override했다면,
		// velocity를 계산하지 않고 덮어씁니다.

		m_bOverride = false;
		deltaVelocity = m_OverrideDeltaVelocity;
	}
	else
	{
		// override하지 않았다면,
		// velocity를 계산합니다.


		// 중력 가속도를 속도에 적용합니다.
		deltaVelocity.y += (GRAVITY * GRAVITY_MULTIPLIER) * GameTime::Instance.GetDeltaTime();


		// TODO: 충돌 검사의 한계로, 길이 제한을 강제로 실시합니다.
		deltaVelocity.x = MathHelper::Clamp<float>(deltaVelocity.x, -500.f, 500.f);
		deltaVelocity.y = MathHelper::Clamp<float>(deltaVelocity.y, -500.f, 500.f);

		OnPreMove(deltaVelocity);

		// rigidbody의 가로 축(x축) 이동에 마찰력을 적용합니다.
		if (MathHelper::AlmostEquals(additionalDeltaVelocityX, 0) == true)
		{
			additionalDeltaVelocityX = 0;
		}
		else
		{
			deltaVelocity.x += additionalDeltaVelocityX;
			float _changeAdditionalDeltaVelocityX = -additionalDeltaVelocityX * dragX * GameTime::Instance.
				GetDeltaTime();
			additionalDeltaVelocityX += _changeAdditionalDeltaVelocityX;
		}
	}

	_rigidBody->Move(deltaVelocity);

	OnPostMove(deltaVelocity);
}

void RigidBodyController::OnPreMove(Vector2& _refDeltaVelocity)
{
}

void RigidBodyController::OnPostMove(Vector2& _refDeltaVelocity)
{
}

bool RigidBodyController::OnVerticalCollision(const RaycastHitInfo& _hitInfo, Vector2& _refDeltaVelocity)
{
	if (_hitInfo.direction.y >= 0)
	{
		// y축으로 내려가는 중 충돌했다면,
		// 충돌한 것이 무엇인지 상관하지 않고 올라섭니다.
		// 이는 이 컴포넌트를 상속한 하위 타입 컴포넌트에서 오버라이딩하여
		// 구체적인 동작을 커스터마이징할 수 있습니다.

		_refDeltaVelocity.y = _hitInfo.direction.y * _hitInfo.distance;
		return true;
	}

	return false;
}

bool RigidBodyController::OnHorizontalCollision(const RaycastHitInfo& _hitInfo, Vector2& _refDeltaVelocity)
{
	// x축으로 이동하는 중 충돌했다면,
	// 충돌한 것이 무엇인지 상관하지 않고 충돌체 앞에서 멈춥니다.
	// 이는 이 컴포넌트를 상속한 하위 타입 컴포넌트에서 오버라이딩하여
	// 구체적인 동작을 커스터마이징할 수 있습니다.

	_refDeltaVelocity.x = _hitInfo.direction.x * _hitInfo.distance;
	return true;
}
