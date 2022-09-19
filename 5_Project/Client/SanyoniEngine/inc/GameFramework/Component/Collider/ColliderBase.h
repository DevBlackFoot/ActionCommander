#pragma once

#include "SanyoniEngine_types.h"
#include "ComponentBase.h"
#include "ComponentInterfaces.h"
#include "CollisionBounds.h"
#include "CollisionInfo.h"

namespace SanyoniEngine
{
	namespace Collision
	{
		class BoundsBase;
	}

	class GameObject;
	class ColliderBase;
	class Transform;
	class CollisionGrid;
	class CollisionCell;

	class ColliderBase abstract
		: public ComponentBase,
		  public IRenderableComponent
	{
	public:
		ColliderBase(GameObject* _gameObject, tstring _typeName);

		virtual ~ColliderBase();

		ECollisionState DoSimpleCollision(ColliderBase* _other);

		void DoCollision();

		void PreTick() final;

		void OnRender() override;

		/**
		 * \brief (not_for_clients) dirty가 활성화된 경우, bounds와 collision grid를 갱신합니다.
		 */
		virtual void UpdateBounds() abstract;

	protected:
		virtual bool SimpleCollision(ColliderBase* _other) abstract;

		/**
		 * \brief 게임 오브젝트의 위치를 기준으로 collider의 중심이 어느 정도 떨어진 곳일지에 대한 오프셋 값입니다. (px 단위)
		 * e.g1 이 값이 (0, 0)일 때 게임 오브젝트의 위치와 collider의 중심점이 같습니다.
		 * e.g2 이 값이 (0, -50)일 때 게임 오브젝트의 위치로부터 collider의 중심점이 50px 위로 이동합니다.
		 */
		Vector2 m_Offset;

		bool m_bIsTrigger;

		/**
		 * \brief 멤버의 값이 변경되어 Bounds가 갱신될 필요가 있는지에 대한 여부입니다.
		 */
		bool m_bDirty;

		/**
		 * \brief 이 collider를 감싸는 AABBBounds 입니다.
		 */
		Collision::AABBBounds m_AABBBounds;

		/**
		 * \brief Debug Draw 활성화 여부입니다.
		 */
		bool m_bDebug;

		/**
		 * \brief Debug Draw시 transform된 도형을 그리는 데 사용될 외곽선의 색상입니다.
		 */
		Color m_DebugTransformedColor;

		/**
		 * \brief Debug Draw시 실제 충돌 판정하는 영역, 즉 Bounds를 그리는 데 사용될 외곽선의 색상입니다.
		 */
		Color m_DebugBoundsColor;

	private:
		/**
		 * \brief 이번 프레임에 이미 이 충돌체와 서로 충돌 검사가 실시된 충돌체들과 그에 대응되는 상태 Map입니다. <br>
		 * 이 컬렉션에 포함된 Collider는 이번 프레임에서 더 이상 충돌 검사 하지 않도록
		 * 필터링하기 위한 용도로 사용됩니다.
		 */
		std::unordered_map<ColliderBase*, ECollisionState> m_CollisionStateThisFrame;

		/**
		 * \brief 이번 프레임에 이미 이 충돌체와 서로 충돌 검사가 실시되었으며,
		 * 충돌이 일어난 충돌체들의 Set입니다.
		 */
		std::unordered_set<ColliderBase*> m_CollidedThisFrame;

		/**
		 * \brief 지난 프레임에 이 충돌체와 서로 충돌이 일어난 충돌체들의 Set입니다.
		 */
		std::unordered_set<ColliderBase*> m_CollidedLastFrame;

		/**
		 * \brief 저번 프레임에서는 충돌했지만 이번 프레임에서 아직 서로 충돌 검사가 실시되지 않은 collider들의 Set입니다.
		 * 이번 프레임에서 충돌 처리가 모두 완료되었을 때까지 이 컬렉션에 남아있는 collider들은 이번 프레임에서 exit된 collider입니다.
		 */
		std::unordered_set<ColliderBase*> m_NotCheckedYet;

		/**
		 * \brief 이 collider가 속해있는 collision grid의 인스턴스입니다.
		 */
		CollisionGrid* m_Grid;

		/**
		 * \brief 이번 프레임에 이 collider가 속해있는 collision cell들의 set 입니다.
		 */
		std::unordered_set<CollisionCell*> m_CellSet;

	public:
		inline const Vector2& GetOffset() const { return m_Offset; }

		inline bool GetIsTrigger() const { return m_bIsTrigger; }

		inline const Collision::AABBBounds& GetAABBBounds()
		{
			UpdateBounds();
			return m_AABBBounds;
		}

		inline Color GetDebugTransformedColor() const { return m_DebugTransformedColor; }

		inline Color GetDebugBoundsColor() const { return m_DebugBoundsColor; }

		inline void SetOffset(const Vector2& _offset) { m_Offset = _offset; }

		inline void SetIsTrigger(bool _trigger) { m_bIsTrigger = _trigger; }

		inline void SetDebug(bool _debug) { m_bDebug = _debug; }

		inline void SetDebugTransformedColor(Color _color) { m_DebugTransformedColor = _color; }

		inline void SetDebugBoundsColor(Color _color) { m_DebugBoundsColor = _color; }

	public:
		/**
		 * \brief (not_for_clients) 이 collider가 속해있는 collision grid를 반환합니다.
		 */
		CollisionGrid* GetGrid() const { return m_Grid; }

		auto& GetCollisionInfos() const { return m_CollisionStateThisFrame; }

		/**
		 * \brief (not_for_clients) 이번 프레임에 이 오브젝트와 이미 충돌 검사를 실시했다는 정보를 등록합니다.
		 */
		void AddAlreadyChecked(ColliderBase* _collider, ECollisionState _state)
		{
			m_CollisionStateThisFrame.insert({_collider, _state});
			m_NotCheckedYet.erase(_collider);
		}

		/**
		 * \brief (not_for_clients) 이번 프레임에 이 오브젝트와 이미 충돌 검사를 실시했으며 충돌이 일어났다는 정보를 등록합니다.
		 */
		void AddCollided(ColliderBase* _collider) { m_CollidedThisFrame.insert(_collider); }

		auto GetCollidedLastFrame() const { return m_CollidedLastFrame; };

		/**
		 * \brief (not_for_clients) 이 collider가 속해있는 collision grid를 지정합니다.
		 */
		void SetGrid(CollisionGrid* _grid) { m_Grid = _grid; }

		void AddCell(CollisionCell* _cell) { m_CellSet.insert(_cell); }

		void RemoveCell(CollisionCell* _cell) { m_CellSet.erase(_cell); }

		auto GetCellSet() const { return m_CellSet; }

		void ClearCellSet() { m_CellSet.clear(); }

		inline void ApplyNotCheckedYet()
		{
			for (auto _collider : m_NotCheckedYet)
			{
				if ((m_bIsTrigger == true && _collider->m_bIsTrigger == true)
					|| (m_bIsTrigger == false && _collider->m_bIsTrigger == false))
				{
					// 둘 중 하나의 충돌체만 trigger일 때에만 충돌 검사를 진행합니다.
					// 만약 둘 다 trigger이거나, 둘 다 trigger가 아닌 경우 건너뜁니다.

					continue;
				}

				m_CollisionStateThisFrame.insert({_collider, ECollisionState::Exit});
			}
		}
	};
}
