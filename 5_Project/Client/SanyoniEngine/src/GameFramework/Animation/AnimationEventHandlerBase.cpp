#include "SanyoniEngine_pch.h"
#include "AnimationEventHandlerBase.h"

using namespace SanyoniEngine;

AnimationEventHandlerBase::AnimationEventHandlerBase(GameObject* _gameObject, const tstring& _typeName) :
	ComponentBase(_gameObject, _typeName)
{
}
