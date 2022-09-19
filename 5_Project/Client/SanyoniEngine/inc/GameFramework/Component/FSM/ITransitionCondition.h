#pragma once

#include "SanyoniEngine_types.h"

namespace SanyoniEngine
{
	template <typename TContext>
	class StateBase;

	template <typename TContext>
	interface ITransitionCondition
	{
		using TState = StateBase<TContext>;

		/*
		 * 이 Condition이 만족되었을 때 전이될 목적지 State입니다.
		 */
		virtual TState* GetDestinationState() abstract;

		/*
		 * 이 Condition이 만족하는지에 대한 여부를 반환합니다.
		 * 이 조건을 만족하였을 때 DestinationState로 전이됩니다.
		 */
		virtual bool IsSatisfied(TContext* _context) abstract;
	};
}
