#include "SanyoniEngine_pch.h"
#include "SpriteFlicker.h"

#include "GameObject.h"
#include "GameTime.h"
#include "SpriteRenderer.h"

using namespace SanyoniEngine;

SpriteFlicker::SpriteFlicker(GameObject* _gameObject, tstring _typeName) :
	ComponentBase(_gameObject, _typeName),
	m_IntervalTimeMillis(100),
	m_DurationMillis(std::nullopt),
	m_StartFlickerTimeMillis(0),
	m_LastFlickerTimeMillis(0)
{
}

void SpriteFlicker::OnChangeEnable(bool _enable)
{
	ComponentBase::OnChangeEnable(_enable);

	if (_enable)
	{
		m_StartFlickerTimeMillis = GameTime::Instance.GetGameTimeMillis();
		m_LastFlickerTimeMillis = m_StartFlickerTimeMillis;
	}
	else
	{
		if (const auto _renderer = GetGameObject()->GetComponent<SpriteRenderer>();
			_renderer != nullptr)
		{
			_renderer->SetOpacity(1.f);
		}

		m_DurationMillis = std::nullopt;
	}
}

void SpriteFlicker::Tick()
{
	ComponentBase::Tick();

	if(m_bEnable == false)
		return;

	if (m_DurationMillis.has_value() == true
		&& GameTime::Instance.GetGameTimeMillis() - m_StartFlickerTimeMillis > m_DurationMillis)
	{
		// 재생 지속시간이 설정되어 있었을 때,
		// 지속시간이 끝나면 이 컴포넌트를 비활성합니다.

		SetEnable(false);
		return;
	}

	if (GameTime::Instance.GetGameTimeMillis() - m_IntervalTimeMillis < m_LastFlickerTimeMillis)
		return;

	m_LastFlickerTimeMillis = GameTime::Instance.GetGameTimeMillis();

	if (const auto _renderer = GetGameObject()->GetComponent<SpriteRenderer>();
		_renderer != nullptr)
	{
		_renderer->SetOpacity(
			_renderer->GetOpacity() > 0 ? 0 : 1.f
		);
	}
}

void SpriteFlicker::Start(long long _duration)
{
	SetEnable(true);

	m_DurationMillis = _duration;
}
