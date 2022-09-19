#include "SanyoniEngine_pch.h"
#include "ComponentBase.h"

#include "GameObject.h"

using namespace SanyoniEngine;

ComponentBase::ComponentBase(GameObject* _gameObject, tstring _typeName)
	: m_Property(_typeName),
	  m_GameObject(_gameObject),
	  m_bEnable(true),
	  m_Ticked(false)
{
}

void ComponentBase::PreTick()
{
	m_Ticked = false;
}

void ComponentBase::PostTick()
{
}

void ComponentBase::TryTick()
{
	if (m_bEnable == false)
		return;

	if (m_Ticked == true)
		return;

	Tick();
}

void ComponentBase::Tick()
{
	m_Ticked = true;
}

void ComponentBase::OnStart()
{
}

void ComponentBase::OnChangeEnable(bool _newEnable)
{
}
