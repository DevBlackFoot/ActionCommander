#include "pch.h"
#include "Heart.h"

#include "AABBCollider.h"
#include "ActionCommanderGameManager.h"
#include "Camera.h"
#include "GameObject.h"
#include "Health.h"
#include "Player.h"

Heart::Heart(GameObject* _gameObject, const tstring& _typeName) :
	ComponentBase(_gameObject, _typeName),
	m_HealAmount(40)
{
	const auto _collider = GetGameObject()->AddComponent<AABBCollider>();
	_collider->SetWidth(50);
	_collider->SetHeight(50);
	_collider->SetIsTrigger(true);
}

void Heart::OnTriggerEnter(ColliderBase* _other)
{
	const auto _player = _other->GetGameObject()->GetComponent<Player>();
	if (_player == nullptr)
		return;

	const auto _health = _player->GetGameObject()->GetComponent<Health>();
	if (_health == nullptr)
		return;

	_health->Add(m_HealAmount);
	GetGameObject()->ReserveDestroy();
}
