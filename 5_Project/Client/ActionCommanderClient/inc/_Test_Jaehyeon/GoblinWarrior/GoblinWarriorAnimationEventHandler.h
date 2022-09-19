#pragma once
#include "AnimationEventHandlerBase.h"
#include "UnitAnimationEventHandler.h"

class GoblinWarriorAnimationEventHandler :
	public UnitAnimationEventHandler
{
public:
	GoblinWarriorAnimationEventHandler(GameObject* _gameObject, const tstring& _typeName = TEXT("GoblinWorriorAnimationEventHandler"));

	bool OnHandle(const tstring& _functionName, const tstring& _parameter) override;
};

