#pragma once

#include "RigidBodyController.h"

namespace SanyoniEngine
{
	class ColliderBase;
}

// 패트롤, 추격 등 이동과 관련된 부분을 담당합니다.
class NepenRigidBodyController :
	public RigidBodyController
{
public:
	NepenRigidBodyController(GameObject* _gameObject, tstring _typeName = TEXT("NepenRigidBodyController"));

	void OnStart() override;

	void PreTick() override;

	// 네펜데스는 움직이지 않습니다..
	void OnPreMove(Vector2& _refDeltaVelocity) override;

	void OnPostMove(Vector2& _refDeltaVelocity) override;

	// 충돌처리를 없애주는 처리를 할 필요가 있습니다.
	bool OnVerticalCollision(const RaycastHitInfo& _hitInfo, Vector2& _refDeltaVelocity) override;

	bool OnHorizontalCollision(const RaycastHitInfo& _hitInfo, Vector2& _refDeltaVelocity) override;

private:
	

public:

};


