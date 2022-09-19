#include "pch.h"
#include "FollowMouse.h"

#include "AABBCollider.h"
#include "CircleCollider.h"
#include "Damagable.h"
#include "GameObject.h"
#include "Input.h"
#include "Screen.h"

using namespace SanyoniEngine;

FollowMouse::FollowMouse(GameObject* _gameObject, tstring _typeName) :
	ComponentBase(_gameObject, _typeName),
	m_StayCount(0)
{
}

void FollowMouse::OnStart()
{
	auto _newCircleCollider = m_GameObject->AddComponent<CircleCollider>();
	_newCircleCollider->SetRadius(50);
}

void FollowMouse::Tick()
{
	m_GameObject->GetComponent<Transform>()->SetLocalPosition(
		Screen::ScreenToWorldPosition(Input::Instance.GetMousePosition())
	);

	if (Input::Instance.GetKeyState(EVirtualKey::MouseRight) & EKeyStateBit::Bit_Down)
	{
		if (const auto _circleCollider = m_GameObject->GetComponent<CircleCollider>();
			_circleCollider != nullptr)
		{
			m_GameObject->RemoveComponent(_circleCollider);
			auto _newAABBCollider = m_GameObject->AddComponent<AABBCollider>();
			_newAABBCollider->SetWidth(70);
			_newAABBCollider->SetHeight(40);
		}
		else if (const auto _aabbCollider = m_GameObject->GetComponent<AABBCollider>();
			_aabbCollider != nullptr)
		{
			m_GameObject->RemoveComponent(_aabbCollider);
			auto _newCircleCollider = m_GameObject->AddComponent<CircleCollider>();
			_newCircleCollider->SetRadius(50);
		}
	}

	if (Input::Instance.GetKeyState(EVirtualKey::MouseLeft) & EKeyStateBit::Bit_Down)
	{
		std::vector<ColliderBase*> _colliders;
		CollisionManager::Instance.OverlapAABB(
			m_GameObject->GetComponent<ColliderBase>()->GetAABBBounds(),
			_colliders
		);

		for (auto _collider : _colliders)
		{
			if (const auto _damagable = _collider->GetGameObject()->GetComponent<Damagable>();
				_damagable != nullptr)
			{
				_damagable->Take(
					10.f,
					(_collider->GetGameObject()->GetComponent<Transform>()->GetWorldPosition() - m_GameObject->GetComponent<Transform>()->GetWorldPosition()) / 5
				);
			}
		}
	}

	m_GameObject->GetComponent<ColliderBase>()->SetDebugBoundsColor(
		m_StayCount > 0
			? Color::Red
			: Color::White
	);
}

void FollowMouse::OnTriggerEnter(ColliderBase* _other)
{
	DebugHelper::PrintDebugString(TEXT("enter \n"));
	++m_StayCount;
}

void FollowMouse::OnTriggerStay(ColliderBase* _other)
{
}

void FollowMouse::OnTriggerExit(ColliderBase* _other)
{
	DebugHelper::PrintDebugString(TEXT("exit \n"));
	--m_StayCount;
}
