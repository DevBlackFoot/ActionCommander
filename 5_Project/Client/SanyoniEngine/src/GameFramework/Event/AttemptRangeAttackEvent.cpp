#include "SanyoniEngine_pch.h"
#include "AttemptRangeAttackEvent.h"

#include "CollisionManager.h"

using namespace SanyoniEngine;

bool AttemptRangeAttackEvent::Do(GameObject* _instigator)
{
	std::vector<ColliderBase*> _colliders;
	CollisionManager::Instance.OverlapAABB(bounds, _colliders);

	for (const auto _collider : _colliders)
	{
		// TODO: 만약 공격 범위 내에 감지된 충돌체가 피격 대상이 되어야 하는게 맞다면, 데미지 이벤트를 발생시킵니다.
		// 또는 상대방의 데미지 함수를 직접 호출합니다.
		// 피격되었을 때 이벤트를 발생시킬지, 직접 함수를 호출할 것인지에 대해서는 아직 정하지 못했습니다.
		// const auto _health = _collider->GetGameObject()->GetComponent<Health>();
		// if (_health != nullptr && (compare tags...))
		// {
		// }
	}

	return true;
}
