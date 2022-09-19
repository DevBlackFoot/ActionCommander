#pragma once
#include "AnimationEventHandlerBase.h"
#include "UnitAnimationEventHandler.h"

class NepenAnimationEventHandler :
	public UnitAnimationEventHandler
{
public:
	NepenAnimationEventHandler(GameObject* _gameObject, const tstring& _typeName = TEXT("NepenAnimationEventHandler"));

	bool OnHandle(const tstring& _functionName, const tstring& _parameter) override;
};

