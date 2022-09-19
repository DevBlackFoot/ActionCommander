#pragma once
#include "ComponentBase.h"

#include "MapData.h"

namespace SanyoniEngine
{
	class GameObject;
	class Scene;
}

class BackgroundMovement :
	public ComponentBase
{
public:
	BackgroundMovement(GameObject* _gameObject, const tstring& _typeName = TEXT("BackgroundMovement"));

	void Make(MapData const* _map, Scene* _scene);

	void Tick() override;

private:
	std::vector<GameObject*> m_BGBack;
	std::vector<GameObject*> m_BGMiddle;
	std::vector<GameObject*> m_BGFront;

	GameObject* m_Field[3];

	Vector2 m_MapSize;
	int m_TileSize;
};
