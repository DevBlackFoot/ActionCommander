#include "SanyoniEngine_pch.h"
#include "GameEvent.h"

#include "GameTime.h"

using namespace SanyoniEngine;

IMPLEMENT_SINGLETON_CLASS(GameEventHandler)

GameEventBase::GameEventBase(const tstring& _typeName) :
	typeName(_typeName),
	m_CreatedGameTimeMillis(GameTime::Instance.GetGameTimeMillis())
{
}

GameEventMessage::GameEventMessage(GameObject* _instigator, GameEventBase* _event) :
	instigator(_instigator),
	event(_event)
{
}
