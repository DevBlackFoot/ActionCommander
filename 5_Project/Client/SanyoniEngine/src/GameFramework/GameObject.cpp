#include "SanyoniEngine_pch.h"
#include "GameObject.h"
#include "ComponentBase.h"

#include "Scene.h"

using namespace SanyoniEngine;

GameObject::GameObject(Scene* _scene, EGameObjectTag _tag, tstring _typeName) :
	m_Property(_typeName),
	m_bEnable(true),
	m_Tag(_tag),
	m_Parent(nullptr),
	m_Childs(0),
	m_Components(0),
	m_Transform(new Transform(this))
{
	_scene->AddGameObject(this);
}

void GameObject::SetScene(Scene* _scene)
{
	m_Scene = _scene;
}

void GameObject::SetParent(GameObject* _parent)
{
	if (m_Parent == _parent)
	{
		return;
	}

	// 만약 기존의 부모가 있다면,
	// 기존의 부모로부터 분리합니다.
	if (m_Parent != nullptr)
		m_Parent->RemoveChild(this);

	m_Parent = _parent;

	if (_parent != nullptr)
	{
		// 새로 지정한 부모가 nullptr가 아닌 경우
		// 부모에게도 이 게임 오브젝트를 자식으로 추가합니다.
		_parent->AddChild(this);
	}
}

void GameObject::AddChild(GameObject* _newChild)
{
	// 이미 자식으로 포함되어 있다면, 추가하지 않습니다.
	if (std::ranges::find(m_Childs, _newChild) != m_Childs.end())
		return;

	m_Childs.push_back(_newChild);

	// 새로운 자식이 원래 다른 씬에 있던 경우, 소속된 씬 정보도 갱신합니다.
	_newChild->SetScene(m_Scene);

	_newChild->SetParent(this);
}

void GameObject::RemoveChild(GameObject* _child)
{
	if (const auto _iter = std::ranges::find(m_Childs, _child);
		_iter != m_Childs.end())
	{
		m_Childs.erase(_iter);

		_child->SetParent(nullptr);
	}
}

std::vector<ComponentBase*> GameObject::GetAllComponents()
{
	return m_Components;
}

void GameObject::ReserveDestroy()
{
	if (m_Scene != nullptr)
		m_Scene->ReserveDestroy(this);
}

int GameObject::GetUID() const { return m_Property.GetUID(); }

tstring GameObject::GetName() const { return m_Property.GetName(); }

void GameObject::BroadcastTriggerEnter(ColliderBase* _other)
{
	for (const auto _trigger : m_Triggers)
		_trigger->OnTriggerEnter(_other);
}

void GameObject::BroadcastTriggerStay(ColliderBase* _other)
{
	for (const auto _trigger : m_Triggers)
		_trigger->OnTriggerStay(_other);
}

void GameObject::BroadcastTriggerExit(ColliderBase* _other)
{
	for (const auto _trigger : m_Triggers)
		_trigger->OnTriggerExit(_other);
}
