#include "SanyoniEngine_pch.h"
#include "SpriteAnimator.h"

#include "GameObject.h"
#include "GameTime.h"
#include "SpriteAnimationClip.h"
#include "SpriteRenderer.h"

using namespace SanyoniEngine;

SpriteAnimator::SpriteAnimator(GameObject* _gameObject, tstring _typeName) :
	ComponentBase(_gameObject, _typeName),
	m_Clip(nullptr),
	m_Index(0),
	m_LastTimeMillis(GameTime::Instance.GetGameTimeMillis())
{
}

void SpriteAnimator::Tick()
{
	ComponentBase::Tick();

	const long long _currentGameTimeMillis = GameTime::Instance.GetGameTimeMillis();
	const long long _delay = 1000 / m_Clip->fps;

	if (_currentGameTimeMillis - m_LastTimeMillis > _delay)
	{
		m_LastTimeMillis = _currentGameTimeMillis;
		m_Index = (m_Index + 1) % m_Clip->sprites.size();

		auto _renderer = m_GameObject->GetComponent<SpriteRenderer>();
		if (_renderer == nullptr)
			return;

		_renderer->SetSprite(m_Clip->sprites[m_Index]);
	}
}
