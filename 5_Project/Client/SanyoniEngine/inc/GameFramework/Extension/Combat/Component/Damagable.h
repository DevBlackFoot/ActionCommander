#pragma once
#include "ComponentBase.h"

namespace SanyoniEngine
{
	class GameObject;

	class Damagable :
		public ComponentBase
	{
	public:
		using TTakeDamageCallbackSignature = void(int, const Vector2&);

		Damagable(GameObject* _gameObject, tstring _typeName = TEXT("Damagable"));

		void Take(int _damage, const Vector2& _push = Vector2::Zero);
	};
}
