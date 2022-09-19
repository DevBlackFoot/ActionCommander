#pragma once

#include "SanyoniEngine_typedef.h"
#include "SingletonInstanceBase.h"

namespace SanyoniEngine
{
	class GameObject;

	/**
	 * \brief GameEvent의 베이스 클래스입니다.
	 * GameEvent는 이벤트 종류에 특수화된 정보를 담고 있어야 하며, 또한 이벤트가 처리될 때의 동작을 정의해야 합니다.
	 *
	 * note: 이 타입을 상속받아 Event를 정의할 때, 반드시 Event에 알맞는 typeName을 초기화할 수 있도록 해야 합니다.
	 */
	struct GameEventBase abstract
	{
		GameEventBase(const tstring& _typeName);

		virtual ~GameEventBase() = default;

		virtual bool Do(GameObject* _instigator) abstract;

		tstring typeName;

		long long m_CreatedGameTimeMillis;
	};


	/**
	 * \brief Event의 인스턴스와 Event를 발생시킨 주체를 담는 구조체입니다.
	 * GameEvent를 발생시킨다는 것은, 그 Event 인스턴스와 주체를 담은 GameEventMessage 인스턴스를 생성하고
	 * GameEventHandler에 Post한다는 것과 같습니다.
	 */
	struct GameEventMessage final
	{
	public:
		GameEventMessage(GameObject* _instigator, GameEventBase* _event);

		virtual ~GameEventMessage();

		GameObject* instigator;
		GameEventBase* event;
	};


	/**
	 * \brief GameEventHandler는 EventMessage를 처리하는 싱글턴 매니저 클래스입니다.
	 */
	class GameEventHandler final
	{
		DECLARE_SINGLETON_CLASS(GameEventHandler)

	public:
		void Post(GameObject* _instigator, GameEventBase* _message) { m_MessageQueue.emplace(_instigator, _message); }

		void Flush()
		{
			while (m_MessageQueue.empty() == false)
			{
				const auto _message = m_MessageQueue.front();
				m_MessageQueue.pop();

				_message.event->Do(_message.instigator);
			}
		}

	private:
		std::queue<GameEventMessage> m_MessageQueue;
	};
}
