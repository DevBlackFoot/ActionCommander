#include "pch.h"
#include "ActionCommanderGameManager.h"

#include "GameSceneFactory.h"
#include "SceneManager.h"

ActionCommanderGameManager::ActionCommanderGameManager(GameObject* _gameObject, const tstring& _typeName) :
	ComponentBase(_gameObject, _typeName)
{
}

void ActionCommanderGameManager::GoNextLevel()
{
	++m_LevelIndex;

	switch (m_LevelIndex)
	{
	case 1:
		SceneManager::Instance.ReserveLoadScene(
			GameSceneFactory().CreateStage1Scene()
		);
		break;
	case 2:
		SceneManager::Instance.ReserveLoadScene(
			GameSceneFactory().CreateStage2Scene()
		);
		break;

	case 3:
		SceneManager::Instance.ReserveLoadScene(
			GameSceneFactory().CreateStage3Scene()
		);
		break;

	case 4:
		SceneManager::Instance.ReserveLoadScene(
			GameSceneFactory().CreateStage4Scene()
		);
		break;

	}
}

void ActionCommanderGameManager::GoGameOverLevel()
{
	SceneManager::Instance.ReserveLoadScene(
		GameSceneFactory().CreateGameOverScene()
	);
}

void ActionCommanderGameManager::SetMap(MapData* _map)
{
	m_Map = _map;
	m_RemainsMonsterCount = _map->monsterCount;
}
