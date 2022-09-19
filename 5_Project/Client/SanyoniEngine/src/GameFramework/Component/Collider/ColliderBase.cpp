#include "SanyoniEngine_pch.h"
#include "ColliderBase.h"

#include "CollisionManager.h"
#include "Core.h"
#include "GameObject.h"
#include "Screen.h"

using namespace SanyoniEngine;
using namespace SanyoniEngine::Collision;

ColliderBase::ColliderBase(GameObject* _gameObject, tstring _typeName)
	: ComponentBase(_gameObject, _typeName),
	  m_Offset(Vector2::Zero),
	  m_bIsTrigger(false),
	  m_bDirty(true),
	  m_AABBBounds({0, 0, 0, 0}),
	  m_bDebug(true),
	  m_DebugTransformedColor(Color::White),
	  m_DebugBoundsColor(Color::Green),
	  m_Grid(nullptr)
{
}

ColliderBase::~ColliderBase()
{
	m_Grid->RemoveCollider(this);
}

ECollisionState ColliderBase::DoSimpleCollision(ColliderBase* _other)
{
	if ((m_bIsTrigger == true && _other->m_bIsTrigger == true)
		|| (m_bIsTrigger == false && _other->m_bIsTrigger == false))
	{
		// 둘 중 하나의 충돌체만 trigger일 때에만 충돌 검사를 진행합니다.
		// 만약 둘 다 trigger이거나, 둘 다 trigger가 아닌 경우,
		// 검사를 더 이상 진행하지 않고 건너뜁니다.

		return ECollisionState::None;
	}

	// 이미 충돌 검사를 실시했다면 그 결과값을 바로 반환합니다.
	if (const auto _iter = m_CollisionStateThisFrame.find(_other);
		_iter != m_CollisionStateThisFrame.end())
	{
		return _iter->second;
	}

	ECollisionState _state;

	// 이전 프레임에서 서로 충돌했는지에 대한 여부를 얻습니다.
	const bool _previousCollision = m_CollidedLastFrame.contains(_other);

	// 이번 프레임에서 서로 충돌했는지에 대한 여부를 얻습니다.
	const bool _collision = SimpleCollision(_other);

	if (_collision == true)
	{
		// 서로에게 이번 프레임에 충돌했다는 정보를 추가합니다.
		AddCollided(_other);
		_other->AddCollided(this);

		// 지난 프레임에서도 충돌하고 있었다면 Stay 상태이고,
		// 아니라면 이번 프레임부터 충돌을 시작한 것이니 Enter 상태입니다.
		_state = _previousCollision ? ECollisionState::Stay : ECollisionState::Enter;
	}
	else
	{
		// 지난 프레임에서 충돌했다면 이번 프레임에서부터 충돌 범위 바깥으로 나갔으니 Exit 상태이고,
		// 아니라면 이번 프레임에서도 지난 프레임에서도 충돌이 없었으므로 None 상태입니다.
		_state = _previousCollision ? ECollisionState::Exit : ECollisionState::None;
	}


	// 서로에게 이번 프레임에 충돌 검사를 실시했다는 정보를 추가합니다.
	AddAlreadyChecked(_other, _state);
	_other->AddAlreadyChecked(this, _state);

	return _state;
}

void ColliderBase::DoCollision()
{
	for (const auto _cell : m_CellSet)
	{
		for (const auto _other : _cell->GetColliderSet())
		{
			if (_other == this)
				continue;

			DoSimpleCollision(_other);
		}
	}

	// 이전 프레임에서 충돌되었지만 이번 프레임에서 충돌 검사하지 않은 오브젝트 (= 상호간 Exit 상태인 오브젝트)를
	// ExitColliders에 추가합니다.
	ApplyNotCheckedYet();
}

void ColliderBase::PreTick()
{
	m_CollidedLastFrame = m_CollidedThisFrame;
	m_NotCheckedYet = m_CollidedThisFrame;

	m_CollisionStateThisFrame.clear();
	m_CollidedThisFrame.clear();
}

void ColliderBase::OnRender()
{
	const auto _transform = GetGameObject()->GetComponent<Transform>();

	if (Core::GetEnableDebug())
	{
		// debug draw
		Screen::Instance.DrawCircleWorldSpace(
			Color::Blue,
			{
				{_transform->GetWorldPosition().x, _transform->GetWorldPosition().y},
				5, 5
			}
		);
	}
}
