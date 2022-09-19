#pragma once

namespace SanyoniEngine
{
	class ColliderBase;

	interface IRenderableComponent abstract
	{
		virtual void OnRender() abstract;
	};

	interface ITriggerComponent abstract
	{
		virtual void OnTriggerEnter(ColliderBase* _other)
		{
		}

		virtual void OnTriggerStay(ColliderBase* _other)
		{
		}

		virtual void OnTriggerExit(ColliderBase* _other)
		{
		}
	};
}
