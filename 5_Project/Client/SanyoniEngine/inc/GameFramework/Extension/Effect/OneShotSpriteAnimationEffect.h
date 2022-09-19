#pragma once
#include "ComponentBase.h"

namespace SanyoniEngine
{
	class AnimationClip;

	class OneShotSpriteAnimationEffect :
		public ComponentBase
	{
	public:
		OneShotSpriteAnimationEffect(GameObject* _gameObject, const tstring& _typeName = TEXT("OneShotSpriteAnimationEffect"));

		void Tick() override;

	private:
		AnimationClip* m_AnimClip;

	public:
		void Set(AnimationClip* _clip, bool _flip = false);
	};
}
