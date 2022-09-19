#pragma once

#include "ObjectProperty.h"
#include "EGameObjectTag.h"
#include "Transform.h"

namespace SanyoniEngine
{
	class ComponentBase;
	class ColliderBase;
	interface ITriggerComponent;
	class Scene;

	class GameObject final
	{
	public:
		GameObject(Scene* _scene, EGameObjectTag _tag = EGameObjectTag::Default, tstring _typeName = TEXT("GameObject"));

		void SetScene(Scene* _scene);

		void SetParent(GameObject* _parent);

		void AddChild(GameObject* _newChild);

		void RemoveChild(GameObject* _child);

		/// <summary>
		/// 게임 컴포넌트가 가지고 있는 컴포넌트 중에서 입력받은 타입과 일치하거나 이 타입의 서브타입의 컴포넌트가 있는 경우
		///	해당 컴포넌트를 반환합니다.
		///	만약 해당하는 컴포넌트가 없다면 nullptr을 반환합니다.
		/// </summary>
		template <class TComponent>
		TComponent* GetComponent();

		template <class TComponent>
		TComponent* AddComponent();

		template <class TComponent>
		void RemoveComponent();

		template <class TComponent>
		void RemoveComponent(TComponent* _component);

		std::vector<ComponentBase*> GetAllComponents();

		void ReserveDestroy();

	private:
		ObjectProperty m_Property;

		bool m_bEnable;

		EGameObjectTag m_Tag;

		Scene* m_Scene;

		GameObject* m_Parent;

		std::vector<GameObject*> m_Childs;

		std::vector<ComponentBase*> m_Components;

		std::unordered_set<ITriggerComponent*> m_Triggers;

	public:
		int GetUID() const;

		tstring GetName() const;

		inline EGameObjectTag GetTag() const { return m_Tag; }

		inline bool GetEnable() const { return m_bEnable; }

		inline class Scene* GetScene() const { return m_Scene; }

		inline GameObject* GetParent() const { return m_Parent; }

		inline std::vector<GameObject*> GetChilds() const { return m_Childs; }

		inline GameObject* GetChild(size_t _index) const { return m_Childs[_index]; }

		inline const auto& GetTriggers() const { return m_Triggers; }

		inline void SetName(tstring _name) { m_Property.SetName(_name); }

		inline void SetTag(EGameObjectTag _tag) { m_Tag = _tag; }

		inline void SetEnable(bool _value) { m_bEnable = _value; }

		inline bool CompareTag(EGameObjectTag _tag) const { return m_Tag == _tag; }

		/**
		 * \brief (not_for_clients)
		 */
		void BroadcastTriggerEnter(ColliderBase* _other);

		/**
		 * \brief (not_for_clients)
		 */
		void BroadcastTriggerStay(ColliderBase* _other);

		/**
		 * \brief (not_for_clients)
		 */
		void BroadcastTriggerExit(ColliderBase* _other);

		/*
		 * 게임 오브젝트가 기본으로 가지고 있는 멤버 컴포넌트를 정의합니다.
		 * 컴포넌트 타입의 멤버를 선언하고 GetComponent와 AddComponent 템플릿 특수화 멤버 메소드를 정의합니다.
		 */
#define __DEFINE_GAME_OBJECT_GET_EMBEDDED_COMPONENT__(TComponent, ComponentMemberName) \
		private: \
			TComponent* ComponentMemberName; \
		public: \
		template <> \
		TComponent* GetComponent<class TComponent>(){ return this->ComponentMemberName; } \
		template <> \
		TComponent* AddComponent<class TComponent>() { assert(true); }

		__DEFINE_GAME_OBJECT_GET_EMBEDDED_COMPONENT__(Transform, m_Transform)
	};
}

#include "GameObject.inl"
