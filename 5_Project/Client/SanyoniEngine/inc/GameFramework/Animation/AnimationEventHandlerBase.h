#pragma once
#include "ComponentBase.h"

namespace SanyoniEngine
{
	class AnimationEventHandlerBase abstract :
		public ComponentBase
	{
	public:
		AnimationEventHandlerBase(GameObject* _gameObject, const tstring& _typeName = TEXT("AnimationEventHandlerBase"));

		virtual bool OnHandle(const tstring& _functionName, const tstring& _parameter) abstract;
	};
}
