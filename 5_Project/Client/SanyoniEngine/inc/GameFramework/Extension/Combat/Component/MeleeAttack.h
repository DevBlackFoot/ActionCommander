#pragma once
#include "ComponentBase.h"

namespace SanyoniEngine
{
	class ColliderBase;

	class MeleeAttack :
		public ComponentBase
	{
	public:
		MeleeAttack(GameObject* _gameObject, tstring _typeName = TEXT("MeleeAttack"));

		void Attack(tstring _attackName);

	private:
		bool m_bAttacking;
	};
}
