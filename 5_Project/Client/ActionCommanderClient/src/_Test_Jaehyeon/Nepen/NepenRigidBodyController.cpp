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
	// ������ �������°� �÷��̾��..? false�� ��ȯ�մϴ�.
	if (const auto _tag = _hitInfo.collider->GetGameObject()->GetTag();
		_tag == EGameObjectTag::Player || _tag == EGameObjectTag::Enemy)
	{
		return false;
	}

	return RigidBodyController::OnVerticalCollision(_hitInfo, _refDeltaVelocity);
}

bool NepenRigidBodyController::OnHorizontalCollision(const RaycastHitInfo& _hitInfo, Vector2& _refDeltaVelocity)
{
	// ���� �浹 ����� ��� ���� _tag�� �÷��̾ ���͸� false�� ��ȯ�մϴ�.
	if (const auto _tag = _hitInfo.collider->GetGameObject()->GetTag();
		_tag == EGameObjectTag::Player || _tag == EGameObjectTag::Enemy)
	{
		return false;
	}

	return RigidBodyController::OnHorizontalCollision(_hitInfo, _refDeltaVelocity);
}
