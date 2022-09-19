#include "SanyoniEngine_pch.h"
#include "CollisionBounds.h"

#include "AABBCollider.h"
#include "CircleCollider.h"
#include "GameObject.h"

using namespace SanyoniEngine::Collision;

AABBBounds::AABBBounds() :
	AABBBounds(0.f, 0.f, 0.f, 0.f)
{
}

AABBBounds::AABBBounds(float _left, float _top, float _right, float _bottom)
	: left(_left),
	  top(_top),
	  right(_right),
	  bottom(_bottom)
{
}

AABBBounds::AABBBounds(AABBCollider* _collider)
{
	Set(_collider);
}

AABBBounds::AABBBounds(CircleCollider* _collider)
{
	Set(_collider);
}

AABBBounds::AABBBounds(ColliderBase* _collider)
{
	if (const auto _aabbCollider = dynamic_cast<AABBCollider*>(_collider);
		_aabbCollider != nullptr)
	{
		Set(_aabbCollider);
		return;
	}
	else if (const auto _circleCollider = dynamic_cast<CircleCollider*>(_collider);
		_circleCollider != nullptr)
	{
		Set(_circleCollider);
		return;
	}

	// AABBBounds를 생성할 수 없는 타입의 collider의 경우 이 코드를 실행하게 됩니다.
	// 이 곳으로 내려온다면 그것은 이 collider를 감싸는 AABB를 생성하는 코드를 누락했다는 것으로, 이를 만들어주어야 합니다.
	assert(false);
}

AABBBounds::AABBBounds(const CircleBounds& _bounds)
{
	const float _radius = _bounds.GetRadius();

	const float _left = _bounds.GetCenter().x - _radius;
	const float _top = _bounds.GetCenter().y - _radius;
	const float _right = _bounds.GetCenter().x + _radius;
	const float _bottom = _bounds.GetCenter().y + _radius;

	Set(_left, _top, _right, _bottom);
}

AABBBounds::AABBBounds(const OBBBounds& _bounds)
{
	const float _left = _bounds.GetLeftTop().x < _bounds.GetLeftBottom().x
		                    ? _bounds.GetLeftTop().x
		                    : _bounds.GetLeftBottom().x;
	const float _right = _bounds.GetRightTop().x > _bounds.GetRightBottom().x
		                     ? _bounds.GetRightTop().x
		                     : _bounds.GetRightBottom().x;

	const float _top = _bounds.GetLeftTop().y < _bounds.GetRightTop().y
		                   ? _bounds.GetLeftTop().y
		                   : _bounds.GetRightTop().y;
	const float _bottom = _bounds.GetLeftBottom().y > _bounds.GetRightBottom().y
		                      ? _bounds.GetLeftBottom().y
		                      : _bounds.GetRightBottom().y;

	Set(_left, _top, _right, _bottom);
}

AABBBounds::AABBBounds(const RayBounds& _bounds)
{
	float _left, _right;
	if (_bounds.GetStart().x < _bounds.GetEnd().x)
	{
		_left = _bounds.GetStart().x;
		_right = _bounds.GetEnd().x;
	}
	else
	{
		_left = _bounds.GetEnd().x;
		_right = _bounds.GetStart().x;
	}

	float _top, _bottom;
	if (_bounds.GetStart().y < _bounds.GetEnd().y)
	{
		_top = _bounds.GetStart().y;
		_bottom = _bounds.GetEnd().y;
	}
	else
	{
		_top = _bounds.GetEnd().y;
		_bottom = _bounds.GetStart().y;
	}

	Set(_left, _top, _right, _bottom);
}

bool AABBBounds::operator==(const AABBBounds&& _other) const
{
	return MathHelper::AlmostEquals(this->left, _other.left) &&
		MathHelper::AlmostEquals(this->top, _other.top) &&
		MathHelper::AlmostEquals(this->right, _other.right) &&
		MathHelper::AlmostEquals(this->bottom, _other.bottom);
}

void AABBBounds::Set(float _left, float _top, float _right, float _bottom)
{
	this->left = _left;
	this->top = _top;
	this->right = _right;
	this->bottom = _bottom;
}

void AABBBounds::Set(AABBCollider* _collider)
{
	// TODO: Box의 rotation에 따른 영역을 정확히 포함되지 않은 AABB를 생성하고 있습니다.

	const auto _transform = _collider->GetGameObject()->GetComponent<Transform>();

	const auto _position = _transform->GetWorldPosition();
	const float _halfWidth = (_collider->GetWidth() * _transform->GetWorldScale().x) / 2;
	const float _halfHeight = (_collider->GetHeight() * _transform->GetWorldScale().y) / 2;

	this->left = (_position.x - _halfWidth) + _collider->GetOffset().x;
	this->top = (_position.y - _halfHeight) + _collider->GetOffset().y;
	this->right = (_position.x + _halfWidth) + _collider->GetOffset().x;
	this->bottom = (_position.y + _halfHeight) + _collider->GetOffset().y;
}

void AABBBounds::Set(CircleCollider* _collider)
{
	const auto _transform = _collider->GetGameObject()->GetComponent<Transform>();

	const auto _position = _transform->GetWorldPosition();
	const float _radius = _transform->GetWorldScale().x > _transform->GetWorldScale().y
		                      ? _transform->GetWorldScale().x * _collider->GetRadius()
		                      : _transform->GetWorldScale().y * _collider->GetRadius();

	this->left = (_position.x - _radius) + _collider->GetOffset().x;
	this->top = (_position.y - _radius) + _collider->GetOffset().y;
	this->right = (_position.x + _radius) + _collider->GetOffset().x;
	this->bottom = (_position.y + _radius) + _collider->GetOffset().y;
}

OBBBounds::OBBBounds(const Vector2&& _leftTop, const Vector2&& _rightTop, const Vector2&& _rightBottom,
                     Vector2 _leftBottom)
	: leftTop(_leftTop),
	  rightTop(_rightTop),
	  rightBottom(_rightBottom),
	  leftBottom(_leftBottom)
{
}

bool OBBBounds::operator==(const OBBBounds&& _other) const
{
	return this->leftTop == _other.leftTop &&
		this->rightTop == _other.rightTop &&
		this->rightBottom == _other.rightBottom &&
		this->leftBottom == _other.leftBottom;
}

void OBBBounds::Set(const Vector2&& _leftTop, const Vector2&& _rightTop, const Vector2&& _rightBottom,
                    const Vector2&& _leftBottom)
{
	this->leftTop = _leftTop;
	this->rightTop = _rightTop;
	this->rightBottom = _rightBottom;
	this->leftBottom = _leftBottom;
}

CircleBounds::CircleBounds(Point2 _center, float _radius)
	: center(_center),
	  radius(_radius),
	  radiusPow(_radius * _radius)
{
}

CircleBounds::CircleBounds(float _centerX, float _centerY, float _radius)
	: center({_centerX, _centerY}),
	  radius(_radius),
	  radiusPow(_radius * _radius)
{
}

CircleBounds::CircleBounds(CircleCollider* _collider)
{
	Set(_collider);
}

bool CircleBounds::operator==(const CircleBounds&& _other) const
{
	return this->center == _other.center &&
		MathHelper::AlmostEquals(this->radius, _other.radius);
}

void CircleBounds::Set(const Point2&& _center, float _radius)
{
	this->center = _center;
	this->radius = _radius;
	this->radiusPow = this->radius * this->radius;
}

void CircleBounds::Set(float _centerX, float _centerY, float _radius)
{
	this->center = {_centerX, _centerY};
	this->radius = _radius;
	this->radiusPow = this->radius * this->radius;
}

void CircleBounds::Set(CircleCollider* _collider)
{
	const auto _transform = _collider->GetGameObject()->GetComponent<Transform>();

	const auto _position = _transform->GetWorldPosition();
	const float _radius = _transform->GetWorldScale().x > _transform->GetWorldScale().y
		                      ? _transform->GetWorldScale().x * _collider->GetRadius()
		                      : _transform->GetWorldScale().y * _collider->GetRadius();

	this->center = Point2(_position.x, _position.y);
	this->radius = _radius;
	this->radiusPow = this->radius * this->radius;
}

RayBounds::RayBounds(const Vector2& _start, const Vector2& _direction, const float _length)
	: start(_start),
	  direction(_direction.Normalize()),
	  length(_length)
{
	UpdateEnd();
}

RayBounds::RayBounds(const Vector2& _start, const Vector2& _end)
{
	Set(_start, _end);
}

bool RayBounds::operator==(const RayBounds&& _other) const
{
	return this->start == _other.start &&
		this->direction == _other.direction &&
		MathHelper::AlmostEquals(this->length, _other.length);
}

void RayBounds::Set(const Vector2& _start, const Vector2& _direction, const float _length)
{
	this->start = _start;
	this->direction = _direction;
	this->length = _length;
	this->end = _start + _direction * _length;
}

void RayBounds::Set(const Vector2& _start, const Vector2& _end)
{
	this->start = _start;
	SetEnd(_end);
}
