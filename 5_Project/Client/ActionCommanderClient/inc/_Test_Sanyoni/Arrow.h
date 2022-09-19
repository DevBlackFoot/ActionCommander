#pragma once
#include "ComponentBase.h"
#include "ComponentInterfaces.h"

class Arrow :
	public ComponentBase,
	public ITriggerComponent
{
public:
	Arrow(GameObject* _gameObject, const tstring& _typeName = TEXT("Arrow"));

	void OnTriggerEnter(ColliderBase* _other) override;

	void Tick() override;

	void Set(const Vector2& _direction, float _moveSpeed, float _damage);

private:
	Vector2 m_Direction;

	float m_MoveSpeed;

	float m_Damage;
};
