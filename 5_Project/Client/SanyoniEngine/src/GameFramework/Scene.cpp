#include "SanyoniEngine_pch.h"
#include "Scene.h"

#include "GameObject.h"

using namespace SanyoniEngine;

Scene::Scene(const tstring& _name):
	m_Name(_name),
	m_GameObjects(0)
{
}

Scene::~Scene()
{
	for (const auto _gameObject : m_GameObjects)
		delete _gameObject;

	for (const auto _component : m_Components)
		delete _component;
}

void Scene::ReserveDestroy(GameObject* _gameObject)
{
	m_WaitingForDestroyGameObjects.insert(_gameObject);
}

void Scene::ReserveDestroy(ComponentBase* _component)
{
	m_WaitingForDestroyComponents.insert(_component);
}

void Scene::Tick()
{
	ProceedDestroy();

	for (auto _iter = m_WaitingForStartComponents.begin();
	     _iter != m_WaitingForStartComponents.end();
	     ++_iter)
	{
		(*_iter)->OnStart();
	}

	m_WaitingForStartComponents.clear();

	for (const auto _component : m_Components)
		_component->PreTick();

	for (const auto _component : m_Components)
		_component->Tick();

	for (const auto _component : m_Components)
		_component->PostTick();
}

void Scene::Render()
{
	for (const auto _component : m_Renderables)
		_component->OnRender();
}

void Scene::OnStartScene()
{
}

void Scene::OnEndScene()
{
	while (m_GameObjects.empty() == false)
	{
		// 씬 내의 모든 게임 오브젝트를 파괴합니다.

		const auto _iter = m_GameObjects.begin();
		DestroyImmediate(*_iter);
	}
}

void Scene::ProceedDestroy()
{
	// 삭제가 예약된 컴포넌트들을 삭제합니다.
	for (const auto _component : m_WaitingForDestroyComponents)
	{
		DestroyImmediate(_component, false);
	}


	// 삭제가 예약된 게임 오브젝트들을 삭제합니다.
	for (const auto _gameObject : m_WaitingForDestroyGameObjects)
	{
		DestroyImmediate(_gameObject, false);
	}


	m_WaitingForDestroyComponents.clear();
	m_WaitingForDestroyGameObjects.clear();
}

void Scene::DestroyImmediate(GameObject* _gameObject, bool _deleteFromReserveQueue)
{
	if (_deleteFromReserveQueue == true)
	{
		if (const auto _iter = m_WaitingForDestroyGameObjects.find(_gameObject);
			_iter != m_WaitingForDestroyGameObjects.end())
		{
			m_WaitingForDestroyGameObjects.erase(_iter);
		}
	}


	if (const auto _iter = std::ranges::find(m_GameObjects, _gameObject);
		_iter != m_GameObjects.end())
	{
		m_GameObjects.erase(_iter);
	}

	// 게임 오브젝트의 하위에 있는 모든 자식들을 재귀적으로 삭제합니다.
	for (const auto _child : _gameObject->GetChilds())
		DestroyImmediate(_child);

	// 게임 오브젝트의 모든 컴포넌트를 삭제합니다.
	for (const auto _component : _gameObject->GetAllComponents())
		DestroyImmediate(_component);


	// 부모로부터 떼어냅니다.
	_gameObject->SetParent(nullptr);


	delete _gameObject;
}

void Scene::DestroyImmediate(ComponentBase* _component, bool _deleteFromReserveQueue)
{
	if (_deleteFromReserveQueue == true)
	{
		if (const auto _iter = m_WaitingForDestroyComponents.find(_component);
			_iter != m_WaitingForDestroyComponents.end())
		{
			m_WaitingForDestroyComponents.erase(_iter);
		}
	}


	if (const auto _iter = m_WaitingForStartComponents.find(_component);
		_iter != m_WaitingForStartComponents.end())
	{
		m_WaitingForStartComponents.erase(_iter);
	}

	if (const auto _iter = m_Components.find(_component);
		_iter != m_Components.end())
	{
		m_Components.erase(_iter);
	}

	// TODO: 제대로 캐스팅되지 않아 컴포넌트가 정상적으로 삭제되지 않는 문제를 해결해야 합니다.
	if (const auto _renderable = dynamic_cast<IRenderableComponent*>(_component);
		_renderable != nullptr)
	{
		RemoveRenderableComponent(_renderable);
	}

	// collider 컴포넌트라면, 이를 CollisionManager에서 삭제합니다.
	if (const auto _collider = dynamic_cast<ColliderBase*>(_component);
		_collider != nullptr)
	{
		CollisionManager::Instance.RemoveCollider(_collider);
	}

	delete _component;
}

void Scene::AddGameObject(GameObject* _gameObject)
{
	if (std::ranges::find(m_GameObjects, _gameObject) != m_GameObjects.end())
		return;

	m_GameObjects.push_back(_gameObject);

	_gameObject->SetScene(this);
}

void Scene::AddComponent(ComponentBase* _component)
{
	assert(m_Components.find(_component) == m_Components.end());
	assert(m_WaitingForStartComponents.find(_component) == m_WaitingForStartComponents.end());

	m_WaitingForStartComponents.insert(_component);
	m_Components.insert(_component);

	if (const auto _renderable = dynamic_cast<IRenderableComponent*>(_component);
		_renderable != nullptr)
	{
		AddRenderableComponent(_renderable);
	}

	// collider 컴포넌트라면, 이를 CollisionManager에 등록합니다.
	if (const auto _collider = dynamic_cast<ColliderBase*>(_component);
		_collider != nullptr)
	{
		CollisionManager::Instance.AddCollider(_collider);
	}
}
