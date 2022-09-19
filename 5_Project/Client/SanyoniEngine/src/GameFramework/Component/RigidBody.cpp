#include "SanyoniEngine_pch.h"
#include "RigidBody.h"

#include "AABBCollider.h"
#include "Core.h"
#include "GameObject.h"

#include "GameTime.h"
#include "Screen.h"

using namespace SanyoniEngine;

RigidBody::RigidBody(GameObject* _gameObject, tstring _typeName) :
	ComponentBase(_gameObject, _typeName)
{
}

void RigidBody::Move(Vector2& _refDeltaVelocity)
{
	DoCollisions(_refDeltaVelocity);

	const auto _deltaPosition =
		m_GameObject->GetComponent<Transform>()->GetWorldPosition()
		+ _refDeltaVelocity;

	m_GameObject->GetComponent<Transform>()->SetWorldPosition(_deltaPosition);
}

void RigidBody::DoCollisions(Vector2& _refDeltaVelocity)
{
	const auto _collider = m_GameObject->GetComponent<ColliderBase>();
	if (_collider == nullptr)
		return;

	const auto _controller = m_GameObject->GetComponent<IRigidBodyController>();
	if (_controller == nullptr)
		return;

	std::vector<RaycastHitInfo> _hitInfos;

	// y축으로 충돌 검사를 실시합니다.

	VerticalCollisions(_collider, _refDeltaVelocity, _hitInfos);

	for (const auto _hit : _hitInfos)
	{
		if (_controller->OnVerticalCollision(_hit, _refDeltaVelocity) == true)
			break;
	}

	_hitInfos.clear();

	// x축으로 충돌 검사를 실시합니다.

	HorizontalCollisions(_collider, _refDeltaVelocity, _hitInfos);

	for (const auto _hit : _hitInfos)
	{
		if (_controller->OnHorizontalCollision(_hit, _refDeltaVelocity) == true)
			break;
	}
}

void RigidBody::VerticalCollisions(ColliderBase* _collider, const Vector2& _deltaVelocity, std::vector<RaycastHitInfo>& _outHitInfos)
{
	constexpr int RAY_COUNT = 3;
	constexpr float COLLIDER_SKIN_WIDTH = 1.f;

	const int _rayDirection = MathHelper::Sign(_deltaVelocity.y);
	if (_rayDirection == 0)
		return;

	const auto _bounds = _collider->GetAABBBounds();
	const float _rayInterval = (_bounds.GetRight() - _bounds.GetLeft()) / (RAY_COUNT - 1);
	const float _rayStartY = _rayDirection > 0
		                         ? _bounds.GetBottom() + COLLIDER_SKIN_WIDTH
		                         : _bounds.GetTop() - COLLIDER_SKIN_WIDTH;

	const float _rayLength = std::abs(_deltaVelocity.y);

	RaycastHitInfo _hitInfo;

	for (int i = 0; i < RAY_COUNT; ++i)
	{
		const float _rayX = _bounds.GetLeft() + _rayInterval * i;
		const Vector2 _rayStart{_rayX, _rayStartY};
		const Vector2 _rayDirectionVector{0, static_cast<float>(_rayDirection)};

		if (CollisionManager::Instance.Raycast(_rayStart, _rayDirectionVector, _rayLength, _hitInfo, m_ExcludeCollision))
		{
			bool _alreadyHit = false;

			for (const auto& _item : _outHitInfos)
			{
				if (_item.collider == _hitInfo.collider)
					_alreadyHit = true;
			}

			if (_alreadyHit == true)
			{
				// 이미 충돌이 감지되었던 오브젝트라면
				// 충돌 정보를 추가하지 않고 무시합니다.

				continue;
			}

			if (_hitInfo.collider == _collider
				|| _hitInfo.distance >= _rayLength)
			{
				// Raycast로 자기 자신을 검출했거나,
				// 또는 충돌했던 충돌체 중 거리가 더 짧았던 충돌체가 이전에 있었다면
				// 아무것도 하지 않습니다.
			}
			else
			{
				_outHitInfos.push_back(_hitInfo);
			}
		}


		if (Core::GetEnableDebug())
		{
			// debug draw

			const Color _debugRayColor = _hitInfo.collider != nullptr ? Color::Red : Color::Green;
			const float _debugRayLength = _hitInfo.collider != nullptr ? _hitInfo.distance : _rayLength;

			Screen::Instance.DrawLineWorldSpace(
				_debugRayColor,
				{_rayStart.x, _rayStartY},
				{_rayStart.x, _rayStartY + _debugRayLength * _rayDirection},
				Matrix3x2::Identity, 1.f);
		}
	}

	// 가장 가까이 충돌한 순서대로 충돌 정보를 정렬합니다.
	std::ranges::sort(_outHitInfos, [](auto _lhs, auto _rhs)-> bool
	{
		return _lhs.distance < _rhs.distance;
	});
}

void RigidBody::HorizontalCollisions(ColliderBase* _collider, const Vector2& _deltaVelocity, std::vector<RaycastHitInfo>& _outHitInfos)
{
	constexpr int RAY_COUNT = 3;
	constexpr float COLLIDER_SKIN_WIDTH = 1.f;

	const int _rayDirection = MathHelper::Sign(_deltaVelocity.x);
	if (_rayDirection == 0)
		return;

	const auto _bounds = _collider->GetAABBBounds();
	const float _rayInterval = (_bounds.GetBottom() - _bounds.GetTop()) / (RAY_COUNT - 1);
	const float _rayStartX = _rayDirection > 0
		                         ? _bounds.GetRight() + COLLIDER_SKIN_WIDTH
		                         : _bounds.GetLeft() - COLLIDER_SKIN_WIDTH;

	const float _rayLength = std::abs(_deltaVelocity.x);

	RaycastHitInfo _hitInfo;

	for (int i = 0; i < RAY_COUNT; ++i)
	{
		const float _rayY = _bounds.GetTop() + _rayInterval * i;
		const Vector2 _rayStart{_rayStartX, _rayY};
		const Vector2 _rayDirectionVector{static_cast<float>(_rayDirection), 0};

		if (CollisionManager::Instance.Raycast(_rayStart, _rayDirectionVector, _rayLength, _hitInfo, m_ExcludeCollision))
		{
			bool _alreadyHit = false;

			for (const auto& _item : _outHitInfos)
			{
				if (_item.collider == _hitInfo.collider)
					_alreadyHit = true;
			}

			if (_alreadyHit == true)
			{
				// 이미 충돌이 감지되었던 오브젝트라면
				// 충돌 정보를 추가하지 않고 무시합니다.

				continue;
			}

			if (_hitInfo.collider == _collider
				|| _hitInfo.distance >= _rayLength)
			{
				// Raycast로 자기 자신을 검출했거나,
				// 또는 충돌했던 충돌체 중 거리가 더 짧았던 충돌체가 이전에 있었다면
				// 아무것도 하지 않습니다.
			}
			else
			{
				_outHitInfos.push_back(_hitInfo);
			}
		}


		if (Core::GetEnableDebug())
		{
			// debug draw

			const Color _debugRayColor = _hitInfo.collider != nullptr ? Color::Red : Color::Green;
			const float _debugRayLength = _hitInfo.collider != nullptr ? _hitInfo.distance : _rayLength;

			Screen::Instance.DrawLineWorldSpace(
				_debugRayColor,
				{_rayStartX, _rayY},
				{_rayStartX + _debugRayLength * _rayDirection, _rayY},
				Matrix3x2::Identity, 1.f);
		}
	}


	// 가장 가까이 충돌한 순서대로 충돌 정보를 정렬합니다.
	std::ranges::sort(_outHitInfos, [](auto _lhs, auto _rhs)-> bool
	{
		return _lhs.distance < _rhs.distance;
	});
}
