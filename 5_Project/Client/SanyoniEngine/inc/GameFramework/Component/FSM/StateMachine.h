#pragma once

#include "ComponentBase.h"
#include "StateBase.h"

namespace SanyoniEngine
{
	class GameObject;

	template <typename TContext>
	class StateBase;

	template <typename TContext>
	class StateMachine :
		public ComponentBase
	{
		using TState = StateBase<TContext>;

	public:
		StateMachine(GameObject* _gameObject, tstring _typeName = TEXT("StateMachine"));

		void OnStart() override;

		void Tick() override;

	private:
		TState* m_StartState;

		TState* m_CurrentState;

	public:
		void SetStartState(TState* _state) { m_StartState = _state; }
	};
}

#include "StateMachine.inl"
