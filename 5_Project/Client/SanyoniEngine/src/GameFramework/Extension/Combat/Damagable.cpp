#include "SanyoniEngine_pch.h"
#include "Damagable.h"

#include "GameObject.h"
#include "Health.h"
#include "RigidBodyController.h"


using namespace SanyoniEngine;

Damagable::Damagable(GameObject* _gameObject, tstring _typeName) :
	ComponentBase(_gameObject, _typeName)
{
}

void Damagable::Take(int _damage, const Vector2& _push)
{
	if (const auto _health = GetGameObject()->GetComponent<Health>();
		_health != nullptr)
	{
		_health->Add(-_damage);
	}

	if (const auto _controller = GetGameObject()->GetComponent<RigidBodyController>();
		_controller != nullptr)
	{
		// TODO: 미는 힘이 소스코드에 하드코딩되어 있습니다.
		_controller->AddForce(_push.Normalize() * 10);
	}
}
