#pragma once

#include "RigidBodyController.h"

namespace SanyoniEngine
{
	class ColliderBase;
}

// ��Ʈ��, �߰� �� �̵��� ���õ� �κ��� ����մϴ�.
class NepenRigidBodyController :
	public RigidBodyController
{
public:
	NepenRigidBodyController(GameObject* _gameObject, tstring _typeName = TEXT("NepenRigidBodyController"));

	void OnStart() override;

	void PreTick() override;

	// ���浥���� �������� �ʽ��ϴ�..
	void OnPreMove(Vector2& _refDeltaVelocity) override;

	void OnPostMove(Vector2& _refDeltaVelocity) override;

	// �浹ó���� �����ִ� ó���� �� �ʿ䰡 �ֽ��ϴ�.
	bool OnVerticalCollision(const RaycastHitInfo& _hitInfo, Vector2& _refDeltaVelocity) override;

	bool OnHorizontalCollision(const RaycastHitInfo& _hitInfo, Vector2& _refDeltaVelocity) override;

private:
	

public:

};


