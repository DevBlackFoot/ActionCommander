#pragma once

#include "SanyoniEngine_types.h"
#include "SingletonInstanceBase.h"
#include "CollisionBounds.h"
#include "CollisionInfo.h"
#include "EGameObjectTag.h"
#include "RaycastHitInfo.h"

namespace SanyoniEngine
{
	class ColliderBase;
	class Transform;

	struct CollisionObject;
	class CollisionCell;
	class CollisionGrid;

	struct CollisionGridBounds
	{
		CollisionGridBounds(int _minX, int _maxX, int _minY, int _maxY) :
			minX(_minX),
			maxX(_maxX),
			minY(_minY),
			maxY(_maxY)
		{
		}

		int minX;
		int maxX;
		int minY;
		int maxY;
	};

	class CollisionCell final
	{
	public:
		CollisionCell(CollisionGrid* m_Grid, IntVector2 _position);

		void AddObject(ColliderBase* _collider);

		void RemoveObject(ColliderBase* _collider);

	private:
		CollisionGrid* m_Grid;

		IntVector2 m_Position;

		std::unordered_set<ColliderBase*> m_ColliderSet;

	public:
		inline IntVector2 GetPosition() const { return m_Position; }

		inline const auto& GetColliderSet() const { return m_ColliderSet; }
	};

	class CollisionGrid final
	{
	public:
		CollisionGrid();
		~CollisionGrid();

		/**
		 * \brief (not_for_clients) 이 collision grid에 collider를 관리 대상으로 등록합니다.
		 */
		void AddCollider(ColliderBase* _collider);

		/**
		 * \brief (not_for_clients) 이 collision grid에서 더 이상 collider를 관리하지 않도록 삭제합니다.
		 */
		void RemoveCollider(ColliderBase* _collider);

		/**
		 * \brief (not_for_clients) 광역 검사를 실시합니다. <br>
		 * =모든 cell들의 지역 검사를 실시합니다.
		 */
		void ExecuteGlobalCollision();

		/**
		 * \brief (not_for_clients) 이 collider가 소속된 cell들의 정보를 갱신합니다.
		 */
		void UpdateCollider(ColliderBase* _collider);

		CollisionGridBounds GetGridBounds(const Collision::AABBBounds& _bounds);

	private:
		/**
		 * \brief 위치에 대응하는 CollisionCell 인스턴스 Map입니다.
		 */
		std::map<IntVector2, CollisionCell*> m_CellMap;

		/**
		 * \brief 관리되는 모든 collider들의 인스턴스 Set입니다.
		 */
		std::unordered_set<ColliderBase*> m_Colliders;

	public:
		CollisionCell* GetCellAt(IntVector2 _position);

		static constexpr int CELL_INTERVAL = 50;
	};

	class CollisionManager final
	{
		DECLARE_SINGLETON_CLASS(CollisionManager)

	public:
		bool Raycast(const Vector2& _start, const Vector2& _direction, float _length, RaycastHitInfo& _outHit, std::unordered_set<EGameObjectTag> _exclude = {});

		bool RaycastAll(const Vector2& _start, const Vector2& _direction, float _length, std::vector<RaycastHitInfo>& _outHits, std::unordered_set<EGameObjectTag> _exclude = {});

		bool OverlapAABB(const Collision::AABBBounds& _bounds, std::vector<ColliderBase*>& _outOverlapped, std::unordered_set<EGameObjectTag> _exclude = {});

		bool OverlapCircle(const Collision::CircleBounds& _bounds, std::vector<ColliderBase*>& _outOverlapped, std::unordered_set<EGameObjectTag> _exclude = {});

	public:
		void CollisionTick();

		void AddCollider(ColliderBase* _collider);

		void RemoveCollider(ColliderBase* _collider);

	private:
		CollisionGrid m_Grid;
	};
}
