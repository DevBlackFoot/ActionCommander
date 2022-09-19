#pragma once
#include "AnimationEventHandlerBase.h"

class UnitAnimationEventHandler :
	public AnimationEventHandlerBase
{
public:
	UnitAnimationEventHandler(GameObject* _gameObject, const tstring& _typeName = TEXT("UnitAnimationEventHandler"));

	bool OnHandle(const tstring& _functionName, const tstring& _parameter) override;
};
