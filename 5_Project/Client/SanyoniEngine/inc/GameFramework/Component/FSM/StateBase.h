#pragma once

#include "SanyoniEngine_pch.h"

#include "ComponentBase.h"
#include "ITransitionCondition.h"

namespace SanyoniEngine
{
	class GameObject;

	template <typename TContext>
	interface ITransitionCondition;

	template <typename TContext>
	class StateBase abstract :
		public ComponentBase
	{
		using TTransitionCondition = ITransitionCondition<TContext>;

	public:
		StateBase(GameObject* _gameObject, const tstring& _typeName = TEXT("StateBase"));

		virtual ~StateBase();

		void OnStart() override;

		virtual void OnStateEnter() abstract;

		virtual void OnStateExit() abstract;

		virtual void OnStateStay() abstract;

	protected:
		virtual std::vector<TTransitionCondition*> CreateConditionList() const abstract;

	private:
		std::vector<TTransitionCondition*> m_Conditions;

	public:
		/*
		 * 어떠한 Condition이 만족되었을 때 어느 State로 전이가 가능한지에 대한 정보를 저장하는 리스트를 반환합니다.
		 * 반환하는 리스트의 정렬 순서는 전이 조건 검사 우선순위와 같아야 합니다.
		 * 만약 동시에 두 개 이상의 Condition을 만족한다면, 리스트의 앞쪽에 있는 Condition의 State로 전이합니다.
		 */
		const std::vector<TTransitionCondition*>& GetConditionList() const { return m_Conditions; };
	};

	template <typename TContext>
	StateBase<TContext>::StateBase(GameObject* _gameObject, const tstring& _typeName) :
		ComponentBase(_gameObject, _typeName)
	{
	}

	template <typename TContext>
	StateBase<TContext>::~StateBase()
	{
		for (auto _condition : m_Conditions)
			delete _condition;
	}

	template <typename TContext>
	void StateBase<TContext>::OnStart()
	{
		m_Conditions = CreateConditionList();
	}
}
