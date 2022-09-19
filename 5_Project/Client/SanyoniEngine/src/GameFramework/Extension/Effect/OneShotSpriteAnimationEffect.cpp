#include "SanyoniEngine_pch.h"
#include "OneShotSpriteAnimationEffect.h"

#include "Animator.h"
#include "GameObject.h"
#include "SpriteRenderer.h"

using namespace SanyoniEngine;

OneShotSpriteAnimationEffect::OneShotSpriteAnimationEffect(GameObject* _gameObject, const tstring& _typeName) :
	ComponentBase(_gameObject, _typeName)
{
	GetGameObject()->AddComponent<SpriteRenderer>();
	GetGameObject()->AddComponent<Animator>();
}

void OneShotSpriteAnimationEffect::Tick()
{
	ComponentBase::Tick();

	const auto _animator = GetGameObject()->GetComponent<Animator>();

	if (_animator->GetReachedEnd() == true)
	{
		GetGameObject()->ReserveDestroy();
		return;
	}
}

void OneShotSpriteAnimationEffect::Set(AnimationClip* _clip, bool _flip)
{
	if (_clip == m_AnimClip)
		return;

	m_AnimClip = _clip;

	GetGameObject()->GetComponent<Animator>()->SetCurrentClip(_clip);
	GetGameObject()->GetComponent<SpriteRenderer>()->SetFlip(_flip);
}
