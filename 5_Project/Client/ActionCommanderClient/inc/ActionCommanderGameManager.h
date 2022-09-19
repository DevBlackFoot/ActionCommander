#pragma once
#include "ComponentBase.h"
#include "MapData.h"

struct MapData;

class ActionCommanderGameManager :
	public ComponentBase
{
public:
	ActionCommanderGameManager(GameObject* _gameObject, const tstring& _typeName = TEXT("ActionCommanderGameManager"));

	bool IsStageCleared() const { return m_RemainsMonsterCount <= 0; }

	void MonsterDead() { --m_RemainsMonsterCount; }

	void GoNextLevel();

	void GoGameOverLevel();

private:
	MapData* m_Map;

	int m_RemainsMonsterCount;

	int m_LevelIndex;

public:
	MapData* GetMap() const { return m_Map; }

	int GetLevelIndex() const { return m_LevelIndex; }

	void SetMap(MapData* _map);

	void SetLevelIndex(int _level) { m_LevelIndex = _level; }

	int GetRemainsMonsterCount() const { return m_RemainsMonsterCount; }
};
