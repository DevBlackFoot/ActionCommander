#pragma once
#include "AnimationEventHandlerBase.h"
#include "UnitAnimationEventHandler.h"

class GoblinBossAnimationEventHandler :
	public UnitAnimationEventHandler
{
public:
	GoblinBossAnimationEventHandler(GameObject *_gameObject, const tstring& _typeName = TEXT("GoblinBossAnimationEventHandler"));

	bool OnHandle(const tstring& _functionName, const tstring& _parameter) override;;
};

