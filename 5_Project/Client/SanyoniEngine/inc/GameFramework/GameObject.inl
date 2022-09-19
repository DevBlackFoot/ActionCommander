#pragma once

#include "ColliderBase.h"
#include "CollisionManager.h"
#include "GameObject.h"

#include "SceneManager.h"
#include "Scene.h"

using namespace SanyoniEngine;

template <class TComponent>
TComponent* GameObject::GetComponent()
{
	for (auto _component : m_Components)
	{
		TComponent* _castedComponent = dynamic_cast<TComponent*>(_component);

		if (_castedComponent != nullptr)
			return _castedComponent;
	}

	return nullptr;
}

template <class TComponent>
TComponent* GameObject::AddComponent()
{
	static_assert(std::is_base_of<ComponentBase, TComponent>::value, "TComponent must inherit from ComponentBase");

	TComponent* _newComponent = new TComponent(this);

	// renderable 컴포넌트라면, 이를 멤버 컬렉션 triggers에 등록합니다.
	if (const auto _trigger = dynamic_cast<ITriggerComponent*>(_newComponent);
		_trigger != nullptr)
	{
		m_Triggers.insert(_trigger);
	}

	// 생성한 컴포넌트를 scene에 등록합니다.
	GetScene()->AddComponent(_newComponent);

	m_Components.push_back(_newComponent);
	return _newComponent;
}

template <class TComponent>
void GameObject::RemoveComponent()
{
	for (auto _iter = m_Components.begin();
	     _iter != m_Components.end(); ++_iter)
	{
		if (const auto _castedComponent = dynamic_cast<TComponent*>(*_iter);
			_castedComponent != nullptr)
		{
			// 컴포넌트를 scene에서 삭제합니다.
			SceneManager::Instance.GetActiveScene()->ReserveDestroy(_castedComponent);

			m_Components.erase(_castedComponent);
			return;
		}
	}
}

template <class TComponent>
void GameObject::RemoveComponent(TComponent* _component)
{
	const auto _iter = std::ranges::find(m_Components, _component);
	if (_iter == m_Components.end())
		return;

	m_Components.erase(_iter);

	// 컴포넌트를 scene에서 삭제합니다.
	SceneManager::Instance.GetActiveScene()->ReserveDestroy(_component);
}
