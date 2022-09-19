#pragma once

#include "SanyoniEngine_types.h"
#include "ComponentInterfaces.h"

namespace SanyoniEngine
{
	class GameObject;
	class ComponentBase;
	class Camera;

	class Scene
	{
	public:
		Scene(const tstring& _name);

		~Scene();

		/**
		 * \brief 오브젝트 삭제를 예약합니다.
		 * 게임 오브젝트를 삭제할 때, 반드시 GameObject 객체를 직접 delete하지 않고 이 함수를 대신 사용해야 합니다.
		 */
		void ReserveDestroy(GameObject* _gameObject);

		/**
		 * \brief 컴포넌트 삭제를 예약합니다.
		 * 컴포넌트를 삭제할 때, 반드시 Component 객체를 직접 delete하지 않고 이 함수를 대신 사용해야 합니다.
		 */
		void ReserveDestroy(ComponentBase* _component);

		void Tick();

		void Render();

		void OnStartScene();

		void OnEndScene();

	private:
		void ProceedDestroy();

		void DestroyImmediate(GameObject* _gameObject, bool _deleteFromReserveQueue = true);

		void DestroyImmediate(ComponentBase* _component, bool _deleteFromReserveQueue = true);

	private:
		tstring m_Name;

		std::vector<GameObject*> m_GameObjects;

		std::unordered_set<GameObject*> m_WaitingForDestroyGameObjects;

		std::unordered_set<ComponentBase*> m_WaitingForDestroyComponents;

		std::unordered_set<ComponentBase*> m_WaitingForStartComponents;

		std::vector<IRenderableComponent*> m_Renderables;

		std::unordered_set<ComponentBase*> m_Components;

		Camera* m_MainCamera;

	public:
		inline tstring GetName() const { return m_Name; }

		inline Camera* GetMainCamera() const { return m_MainCamera; }

		inline void SetMainCamera(Camera* _camera) { m_MainCamera = _camera; }

		/**
		 * \brief (not_for_clients)
		 */
		void AddGameObject(GameObject* _gameObject);

		/*
		 * (not_for_clients) 컴포넌트를 추가합니다.
		 */
		void AddComponent(ComponentBase* _component);

	private:
		inline void AddRenderableComponent(IRenderableComponent* _component) { m_Renderables.push_back(_component); }

		inline void RemoveRenderableComponent(IRenderableComponent* _component)
		{
			if (const auto _iter = std::ranges::find(m_Renderables, _component);
				_iter != m_Renderables.end())
			{
				m_Renderables.erase(_iter);
			}
		}
	};
}
