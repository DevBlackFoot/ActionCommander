#include "pch.h"
#include "Nepen/NepenRigidBodyController.h"

#include "ColliderBase.h"
#include "GameObject.h"
#include "GameTime.h"
#include "Platform.h"

NepenRigidBodyController::NepenRigidBodyController(GameObject* _gameObject, tstring _typeName) :
	RigidBodyController(_gameObject, _typeName)
{
}

void NepenRigidBodyController::OnStart()
{
	if (const auto _rigidBody = GetGameObject()->GetComponent<RigidBody>();
		_rigidBody != nullptr)
	{
		_rigidBody->SetExcludeCollsion({{EGameObjectTag::Player, EGameObjectTag::Enemy}});
	}
}

void NepenRigidBodyController::PreTick()
{
	RigidBodyController::PreTick();
}

void NepenRigidBodyController::OnPreMove(Vector2& _refDeltaVelocity)
{
	_refDeltaVelocity.x = 0;

	RigidBodyController::OnPreMove(_refDeltaVelocity);
}

void NepenRigidBodyController::OnPostMove(Vector2& _refDeltaVelocity)
{
	RigidBodyController::OnPostMove(_refDeltaVelocity);
}

bool NepenRigidBodyController::OnVerticalCollision(const RaycastHitInfo& _hitInfo, Vector2& _refDeltaVelocity)
{
	// 위에서 떨어지는게 플레이어면..? false를 반환합니다.
	if (const auto _tag = _hitInfo.collider->GetGameObject()->GetTag();
		_tag == EGameObjectTag::Player || _tag == EGameObjectTag::Enemy)
	{
		return false;
	}

	return RigidBodyController::OnVerticalCollision(_hitInfo, _refDeltaVelocity);
}

bool NepenRigidBodyController::OnHorizontalCollision(const RaycastHitInfo& _hitInfo, Vector2& _refDeltaVelocity)
{
	// 옆의 충돌 방향과 상관 없이 _tag가 플레이어나 몬스터면 false를 반환합니다.
	if (const auto _tag = _hitInfo.collider->GetGameObject()->GetTag();
		_tag == EGameObjectTag::Player || _tag == EGameObjectTag::Enemy)
	{
		return false;
	}

	return RigidBodyController::OnHorizontalCollision(_hitInfo, _refDeltaVelocity);
}
