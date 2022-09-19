#include "pch.h"
#include "FollowPlayer.h"

#include "GameObject.h"
#include "Player.h"

FollowPlayer::FollowPlayer(SanyoniEngine::GameObject* _gameObject, tstring _typeName) :
	ComponentBase(_gameObject, _typeName)
{
}

void FollowPlayer::Tick()
{
	ComponentBase::Tick();

	if (m_Player == nullptr)
		return;

	const auto _transform = m_GameObject->GetComponent<Transform>();
	const auto _deltaPosition
		= Vector2::Lerp(_transform->GetWorldPosition(),
		                m_Player->GetGameObject()->GetComponent<Transform>()->GetWorldPosition(),
		                0.2f);

	_transform->SetWorldPosition(_deltaPosition);
}
