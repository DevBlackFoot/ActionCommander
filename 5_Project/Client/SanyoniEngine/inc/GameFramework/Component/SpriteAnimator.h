#pragma once

#include "ComponentBase.h"

namespace SanyoniEngine
{
	class GameObject;

	struct SpriteAnimationClip;

	class SpriteAnimator final
		: public ComponentBase
	{
	public:
		SpriteAnimator(GameObject* _gameObject, tstring _typeName = TEXT("SpriteRenderer"));

		void Tick() override;

	private:
		SpriteAnimationClip* m_Clip;

		int m_Index;

		long long m_LastTimeMillis;

	public:
		inline SpriteAnimationClip* GetClip() const { return m_Clip; }

		inline void SetClip(SpriteAnimationClip* _clip) { m_Clip = _clip; }
	};
}
