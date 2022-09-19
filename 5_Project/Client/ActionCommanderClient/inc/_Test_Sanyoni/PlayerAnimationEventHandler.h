#pragma once
#include "AnimationEventHandlerBase.h"
#include "UnitAnimationEventHandler.h"

class PlayerAnimationEventHandler :
	public UnitAnimationEventHandler
{
public:
	PlayerAnimationEventHandler(GameObject* _gameObject, const tstring& _typeName = TEXT("PlayerAnimationEventHandler"));

	bool OnHandle(const tstring& _functionName, const tstring& _parameter) override;
};
