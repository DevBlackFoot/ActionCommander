#pragma once
#include "AnimationEventHandlerBase.h"
#include "UnitAnimationEventHandler.h"

class GoblinArcherAnimationEventHandler :
	public UnitAnimationEventHandler
{
public:
	GoblinArcherAnimationEventHandler(GameObject* _gameObject, const tstring& _typeName = TEXT("GoblinArcherAnimationEventHandler"));

	bool OnHandle(const tstring& _functionName, const tstring& _parameter) override;

};

