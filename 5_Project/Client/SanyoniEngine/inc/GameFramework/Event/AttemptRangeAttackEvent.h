#pragma once
#include "CollisionBounds.h"
#include "GameEvent.h"

namespace SanyoniEngine
{
	class GameObject;

	struct AttemptRangeAttackEvent :
		GameEventBase
	{
		AttemptRangeAttackEvent(const tstring& _typeName = TEXT("AttemptRangeAttackEvent")) :
			GameEventBase(_typeName)
		{
		}

		bool Do(GameObject* _instigator) override;

		Collision::AABBBounds bounds;
		float damage;
	};
}
