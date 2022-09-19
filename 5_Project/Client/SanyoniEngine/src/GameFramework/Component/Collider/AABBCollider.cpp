#include "SanyoniEngine_pch.h"
#include "AABBCollider.h"

#include "CircleCollider.h"
#include "GameObject.h"
#include "Screen.h"
#include "Collision.h"
#include "Core.h"

using namespace SanyoniEngine;
using namespace SanyoniEngine::Collision;

AABBCollider::AABBCollider(GameObject* _gameObject, tstring _typeName)
	: ColliderBase(_gameObject, _typeName),
	  m_Width(10),
	  m_Height(10),
	  m_Bounds(0, 0, 0, 0),
	  m_WorldTMAtLastBoundsUpdated(Matrix3x2::Identity)
{
}

void AABBCollider::OnRender()
{
	if (Core::GetEnableDebug())
	{
		ColliderBase::OnRender();

		const auto _bounds = GetBounds();

		Screen::Instance.DrawRectangleWorldSpace(m_DebugBoundsColor,
		                                         Rect(_bounds.GetLeft(), _bounds.GetTop(), _bounds.GetRight(), _bounds.GetBottom())
		);

		Screen::Instance.DrawRectangleWorldSpace(
			m_DebugTransformedColor,
			m_GameObject->GetComponent<Transform>(),
			m_Width, m_Height,
			.2f);
	}
}

void AABBCollider::UpdateBounds()
{
	const auto _transform = m_GameObject->GetComponent<Transform>();

	if (m_bDirty == false
		&& m_WorldTMAtLastBoundsUpdated == _transform->GetWorldTM())
	{
		// Bounds가 가장 마지막으로 갱신되었을 때 이후로
		// 멤버 변수의 값이 변경되지 않았거나
		// 오브젝트 TM과 지금의 TM이 다르지 않다면,
		// 물체에 아무런 변형이 가해지지 않았다는 것이므로, Bounds를 갱신하지 않아도 좋습니다.
		return;
	}

	m_Bounds = m_AABBBounds = AABBBounds{this};
}

bool AABBCollider::SimpleCollision(ColliderBase* _other)
{
	assert(_other != nullptr);

	if (const auto _otherCollider = dynamic_cast<AABBCollider*>(_other);
		_otherCollider != nullptr)
	{
		const bool _collision = AABB::OverlapAABB(this->GetBounds(), _otherCollider->GetBounds());
		return _collision;
	}
	else if (const auto _otherCollider = dynamic_cast<CircleCollider*>(_other);
		_otherCollider != nullptr)
	{
		const bool _collision = AABB::OverlapCircle(this->GetBounds(), _otherCollider->GetBounds());
		return _collision;
	}

	// 어떠한 if문에도 걸리지 않고 이곳으로 온 경우는 상대방 collider 타입에 대한 충돌 구현이 되어있지 않다는 것을 의미합니다.
	// 이 때 assert합니다.
	assert(false);

	return false;
}
