#pragma once
#include "ComponentBase.h"

namespace SanyoniEngine
{
	class Health final :
		public ComponentBase
	{
	public:
		Health(GameObject* _gameObject, const tstring& _typeName = TEXT("Health"));

		void Add(int _amount);

		void Reset();

	private:
		int m_MaxHealth;

		// TODO: 여러 callback을 등록할 수 있는 멤버로 변경해야 합니다. (ex: c#의 event처럼 구현되어야 합니다.)

		std::function<void(int)> m_OnHealthChanged;

		std::function<void()> m_OnHealthZero;

		int m_Health;

	public:
		void SetMaxHealth(int _maxHealth)
		{
			m_MaxHealth = _maxHealth;
			m_Health = MathHelper::Clamp<int>(m_Health, m_Health, _maxHealth);
		}

		int GetMaxHealth() const { return m_MaxHealth; }
		int GetHealth() const { return m_Health; }

		void SetOnHealthChanged(std::function<void(int)> _event)
		{
			m_OnHealthChanged = _event;
		}

		void SetOnHealthZero(std::function<void()> _event)
		{
			m_OnHealthZero = _event;
		}
	};
}
