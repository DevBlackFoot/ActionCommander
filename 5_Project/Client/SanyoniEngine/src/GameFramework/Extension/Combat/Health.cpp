#include "SanyoniEngine_pch.h"
#include "Health.h"

using namespace SanyoniEngine;

Health::Health(GameObject* _gameObject, const tstring& _typeName) :
	ComponentBase(_gameObject, _typeName),
	m_MaxHealth(1000),
	m_Health(m_MaxHealth),
	m_OnHealthChanged(nullptr),
	m_OnHealthZero(nullptr)
{
}

void Health::Add(int _amount)
{
	if (m_Health <= 0)
		return;

	m_Health = m_Health + _amount;

	if (m_Health > m_MaxHealth)
		m_Health = m_MaxHealth;

	if (m_OnHealthChanged != nullptr)
		m_OnHealthChanged(_amount);

	if (m_Health <= 0)
	{
		m_Health = _amount;

		if (m_OnHealthZero != nullptr)
			m_OnHealthZero();
	}
}

void Health::Reset()
{
	m_Health = m_MaxHealth;
}
