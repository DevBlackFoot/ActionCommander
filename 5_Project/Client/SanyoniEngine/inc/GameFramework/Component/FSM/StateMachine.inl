#pragma once

#include "StateMachine.h"

template <typename TContext>
StateMachine<TContext>::StateMachine(GameObject* _gameObject, tstring _typeName) :
	ComponentBase(_gameObject, _typeName)
{
}

template <typename TContext>
void StateMachine<TContext>::OnStart()
{
	ComponentBase::OnStart();

	if (m_StartState == nullptr)
	{
		SetEnable(false);
		return;
	}

	m_CurrentState = m_StartState;
	m_CurrentState->OnStateEnter();
}

template <typename TContext>
void StateMachine<TContext>::Tick()
{
	ComponentBase::Tick();

	if (m_CurrentState == nullptr)
		return;

	for (auto _condition : m_CurrentState->GetConditionList())
	{
		if (_condition->IsSatisfied(GetGameObject()->GetComponent<TContext>()) == true)
		{
			m_CurrentState->OnStateExit();
			m_CurrentState = _condition->GetDestinationState();
			m_CurrentState->OnStateEnter();

			return;
		}
	}

	m_CurrentState->OnStateStay();
}
