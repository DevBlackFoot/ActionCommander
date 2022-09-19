#include "pch.h"
#include "Arrow.h"

#include "AABBCollider.h"
#include "ColliderBase.h"
#include "GameObject.h"
#include "GameTime.h"
#include "Health.h"
#include "Player.h"
#include "Resources.h"
#include "SpriteRenderer.h"

using namespace SanyoniEngine;

Arrow::Arrow(GameObject* _gameObject, const tstring& _typeName) :
	ComponentBase(_gameObject, _typeName)
{
	GetGameObject()->AddComponent<SpriteRenderer>()->SetSprite(
		Resources::Instance.LoadSpriteSheetFromFile(TEXT("Assets/SpriteSheet/Object/Arrow.png"))
	);

	auto _collider = GetGameObject()->AddComponent<AABBCollider>();
	_collider->SetWidth(10);
	_collider->SetHeight(5);
	_collider->SetIsTrigger(true);
}

void Arrow::OnTriggerEnter(ColliderBase* _other)
{
	const auto _tag = _other->GetGameObject()->GetTag();

	if (_tag == EGameObjectTag::Platform
		|| _tag == EGameObjectTag::Wall)
	{
		GetGameObject()->ReserveDestroy();
		return;
	}

	const auto _player = _other->GetGameObject()->GetComponent<Player>();
	if (_player == nullptr)
		return;

	const auto _health = _player->GetGameObject()->GetComponent<Health>();
	if (_health == nullptr)
		return;

	_health->Add(-m_Damage);
	GetGameObject()->ReserveDestroy();
}

void Arrow::Tick()
{
	GetGameObject()->GetComponent<Transform>()->SetWorldPosition(
		GetGameObject()->GetComponent<Transform>()->GetWorldPosition()
		+ m_Direction * m_MoveSpeed * GameTime::Instance.GetDeltaTime()
	);
}

void Arrow::Set(const Vector2& _direction, float _moveSpeed, float _damage)
{
	m_Direction = _direction;
	m_MoveSpeed = _moveSpeed;
	m_Damage = _damage;

	GetGameObject()->GetComponent<SpriteRenderer>()->SetFlip(m_Direction.x >= 0 ? false : true);
}
