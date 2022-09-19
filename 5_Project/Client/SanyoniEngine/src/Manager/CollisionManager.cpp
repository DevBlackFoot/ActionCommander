#include "SanyoniEngine_pch.h"
#include "CollisionManager.h"

#include "ColliderBase.h"
#include "Collision.h"
#include "GameObject.h"

using namespace SanyoniEngine;

IMPLEMENT_SINGLETON_CLASS(CollisionManager)

CollisionCell::CollisionCell(CollisionGrid* _grid, IntVector2 _position)
	: m_Grid(_grid),
	  m_Position(_position),
	  m_ColliderSet()
{
}

void CollisionCell::AddObject(ColliderBase* _collider)
{
	assert(_collider != nullptr);
	assert(_collider->GetGrid() == m_Grid);

	// cell에 오브젝트를 포함한다는 정보를 추가합니다.
	m_ColliderSet.insert(_collider);

	// collider에게도 이 cell에 포함된다는 정보를 추가합니다.
	_collider->AddCell(this);
}

void CollisionCell::RemoveObject(ColliderBase* _collider)
{
	if (auto _iter = m_ColliderSet.find(_collider);
		_iter != m_ColliderSet.end())
	{
		// cell이 더이상 이 오브젝트를 포함하지 않도록 삭제합니다.
		m_ColliderSet.erase(_iter);

		// collider에게도 이 cell에 포함되지 않도록 멤버 컬렉션을 수정합니다.
		_collider->RemoveCell(this);
	}
}

CollisionGrid::CollisionGrid()
	: m_CellMap()
{
}

CollisionGrid::~CollisionGrid()
{
	// 모든 Cell들을 삭제합니다.
	for (const auto _pair : m_CellMap)
		delete _pair.second;

	// TODO: 모든 object의 grid 레퍼런스를 초기화합니다.
}

void CollisionGrid::AddCollider(ColliderBase* _collider)
{
	assert(_collider != nullptr);

	// 오브젝트가 이미 Grid에 추가된 상태인지 확인합니다.
	if (_collider->GetGrid() == this)
		return;

	m_Colliders.insert(_collider);

	_collider->SetGrid(this);

	UpdateCollider(_collider);
}

void CollisionGrid::RemoveCollider(ColliderBase* _collider)
{
	assert(_collider != nullptr);

	// 게임 오브젝트가 이 그리드상에 존재하는지 확인합니다.
	if (_collider->GetGrid() != this)
		return;

	m_Colliders.erase(_collider);

	// collider를 포함하는 모든 cell들로부터 collider 정보를 삭제합니다.
	auto _cellSet = _collider->GetCellSet();
	for (auto _cell : _cellSet)
		_cell->RemoveObject(_collider);

	// cell set을 초기화합니다.
	_collider->ClearCellSet();

	_collider->SetGrid(nullptr);
}

void CollisionGrid::ExecuteGlobalCollision()
{
	// 광역 검사에 앞서 collider의 위치 정보를 갱신합니다.
	for (auto _collider : m_Colliders)
		UpdateCollider(_collider);


	// collider들 간 충돌 검사를 실시합니다.
	for (const auto _collider : m_Colliders)
	{
		_collider->DoCollision();
	}

	// 충돌한 오브젝트들에게 Trigger 메세지를 브로드캐스트합니다.
	for (const auto _collider : m_Colliders)
	{
		auto& _collisionInfos = _collider->GetCollisionInfos();

		for (const auto _otherPair : _collisionInfos)
		{
			if (m_Colliders.contains(_otherPair.first) == false)
			{
				// 만약 other가 더 이상 collider manager에서 관리하지 않는 충돌체라면
				// 이 충돌체에 대한 처리를 하지 않고 무시합니다.
				continue;
			}


			switch (_otherPair.second)
			{
			case ECollisionState::Enter:
				_collider->GetGameObject()->BroadcastTriggerEnter(_otherPair.first);
				break;

			case ECollisionState::Stay:
				_collider->GetGameObject()->BroadcastTriggerStay(_otherPair.first);
				break;

			case ECollisionState::Exit:
				_collider->GetGameObject()->BroadcastTriggerExit(_otherPair.first);
				break;
			}
		}
	}
}

void CollisionGrid::UpdateCollider(ColliderBase* _collider)
{
	auto& _bounds = _collider->GetAABBBounds();

	// 기존의 cell set 정보를 전부 버립니다.
	auto _cellSet = _collider->GetCellSet();
	for (auto _cell : _cellSet)
		_cell->RemoveObject(_collider);

	// Grid상에서 AABBBounds와 곂치는 모든 Cell들에게 이 오브젝트의 정보를 추가합니다.
	const auto _gridBounds = GetGridBounds(_bounds);

	for (int _x = _gridBounds.minX; _x <= _gridBounds.maxX; ++_x)
	{
		for (int _y = _gridBounds.minY; _y <= _gridBounds.maxY; ++_y)
		{
			const auto _cell = GetCellAt({_x, _y});

			// cell에 이 collision object 정보를 추가합니다.
			_cell->AddObject(_collider);

			// collider에게 이 cell에 포함되어 있다는 정보를 추가합니다.
			_collider->AddCell(_cell);
		}
	}
}

CollisionGridBounds CollisionGrid::GetGridBounds(const Collision::AABBBounds& _bounds)
{
	const CollisionGridBounds _outBounds{
		static_cast<int>(_bounds.GetLeft() / CELL_INTERVAL),
		static_cast<int>(_bounds.GetRight() / CELL_INTERVAL),
		static_cast<int>(_bounds.GetTop() / CELL_INTERVAL),
		static_cast<int>(_bounds.GetBottom() / CELL_INTERVAL)
	};

	return _outBounds;
}

CollisionCell* CollisionGrid::GetCellAt(IntVector2 _position)
{
	const auto _iter = m_CellMap.find(_position);
	if (_iter != m_CellMap.end())
	{
		return _iter->second;
	}
	else
	{
		auto _newCell = new CollisionCell(this, _position);
		m_CellMap.insert({_position, _newCell});

		return _newCell;
	}
}

bool CollisionManager::Raycast(const Vector2& _start, const Vector2& _direction, float _length, RaycastHitInfo& _outHit, std::unordered_set<EGameObjectTag> _exclude)
{
	std::vector<RaycastHitInfo> _hits;
	bool _anyHit = RaycastAll(_start, _direction, _length, _hits, _exclude);

	_outHit = _anyHit ? _hits[0] : RaycastHitInfo();
	return _anyHit;
}

bool CollisionManager::RaycastAll(const Vector2& _start, const Vector2& _direction, float _length, std::vector<RaycastHitInfo>& _outHits,
                                  std::unordered_set<EGameObjectTag> _exclude)
{
	const Collision::RayBounds _rayBounds{_start, _direction, _length};
	const Collision::AABBBounds _aabbBounds{_rayBounds};

	const auto _gridBounds = m_Grid.GetGridBounds(_aabbBounds);

	std::unordered_set<ColliderBase*> _colliderSet;

	for (int y = _gridBounds.minY; y <= _gridBounds.maxY; ++y)
	{
		for (int x = _gridBounds.minX; x <= _gridBounds.maxX; ++x)
		{
			auto _cell = m_Grid.GetCellAt({x, y});
			_colliderSet.insert(
				_cell->GetColliderSet().begin(), _cell->GetColliderSet().end());
		}
	}

	std::erase_if(_colliderSet, [&_exclude](ColliderBase* _collider)
	{
		if (_collider->GetIsTrigger() == true)
		{
			// 트리거인 경우, 검사 대상에서 제외합니다.
			return true;
		}
		else if (_exclude.contains(_collider->GetGameObject()->GetTag()))
		{
			// 제외 대상인 경우도 검사 대상에서 제외합니다.
			return true;
		}

		return false;
	});

	std::vector<RaycastHitInfo> _hitInfos;

	for (const auto _collider : _colliderSet)
	{
		RaycastHitInfo _hitInfo;
		Collision::Ray::Raycast(_rayBounds, _collider, _hitInfo);

		if (_hitInfo.collider != nullptr)
			_hitInfos.push_back(_hitInfo);
	}

	if (_hitInfos.empty() == true)
		return false;


	std::sort(_hitInfos.begin(), _hitInfos.end());
	_outHits = std::forward<std::vector<RaycastHitInfo>>(_hitInfos);
	return true;
}

bool CollisionManager::OverlapAABB(const Collision::AABBBounds& _bounds, std::vector<ColliderBase*>& _outOverlapped, std::unordered_set<EGameObjectTag> _exclude)
{
	_outOverlapped.clear();

	const auto _gridBounds = m_Grid.GetGridBounds(_bounds);

	std::unordered_set<ColliderBase*> _colliderSet;

	for (int y = _gridBounds.minY; y <= _gridBounds.maxY; ++y)
	{
		for (int x = _gridBounds.minX; x <= _gridBounds.maxX; ++x)
		{
			auto _cell = m_Grid.GetCellAt({x, y});
			_colliderSet.insert(
				_cell->GetColliderSet().begin(), _cell->GetColliderSet().end());
		}
	}


	std::erase_if(_colliderSet, [&_exclude](ColliderBase* _collider)
	{
		if (_collider->GetIsTrigger() == true)
		{
			// 트리거인 경우, 검사 대상에서 제외합니다.
			return true;
		}
		else if (_exclude.contains(_collider->GetGameObject()->GetTag()))
		{
			// 제외 대상인 경우도 검사 대상에서 제외합니다.
			return true;
		}

		return false;
	});


	for (const auto _collider : _colliderSet)
	{
		const bool _collision = Collision::AABB::Overlap(_bounds, _collider);
		if (_collision == true)
			_outOverlapped.push_back(_collider);
	}

	if (_outOverlapped.empty() == true)
		return false;

	return true;
}

bool CollisionManager::OverlapCircle(const Collision::CircleBounds& _bounds, std::vector<ColliderBase*>& _outOverlapped, std::unordered_set<EGameObjectTag> _exclude)
{
	_outOverlapped.clear();

	const auto _gridBounds = m_Grid.GetGridBounds(Collision::AABBBounds(_bounds));

	std::unordered_set<ColliderBase*> _colliderSet;

	for (int y = _gridBounds.minY; y <= _gridBounds.maxY; ++y)
	{
		for (int x = _gridBounds.minX; x <= _gridBounds.maxX; ++x)
		{
			auto _cell = m_Grid.GetCellAt({x, y});
			_colliderSet.insert(
				_cell->GetColliderSet().begin(), _cell->GetColliderSet().end());
		}
	}


	std::erase_if(_colliderSet, [&_exclude](ColliderBase* _collider)
	{
		if (_collider->GetIsTrigger() == true)
		{
			// 트리거인 경우, 검사 대상에서 제외합니다.
			return true;
		}
		else if (_exclude.contains(_collider->GetGameObject()->GetTag()))
		{
			// 제외 대상인 경우도 검사 대상에서 제외합니다.
			return true;
		}

		return false;
	});


	for (const auto _collider : _colliderSet)
	{
		const bool _collision = Collision::AABB::Overlap(_bounds, _collider);
		if (_collision == true)
			_outOverlapped.push_back(_collider);
	}

	if (_outOverlapped.empty() == true)
		return false;

	return true;
}

void CollisionManager::CollisionTick()
{
	m_Grid.ExecuteGlobalCollision();
}

void CollisionManager::AddCollider(ColliderBase* _collider)
{
	m_Grid.AddCollider(_collider);
}

void CollisionManager::RemoveCollider(ColliderBase* _collider)
{
	m_Grid.RemoveCollider(_collider);
}
