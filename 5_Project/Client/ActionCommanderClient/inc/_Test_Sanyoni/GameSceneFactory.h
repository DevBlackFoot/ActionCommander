#pragma once
#include <rapidjson/document.h>

#include "CollisionBounds.h"
#include "SanyoniEngine_types.h"

namespace SanyoniEngine
{
	class Scene;
}

class MapData;

class GameSceneFactory
{
public:
	Scene* CreateTitleScene();

	Scene* CreateGameOverScene();

	Scene* CreateStage1Scene();

	Scene* CreateStage2Scene();

	Scene* CreateStage3Scene();

	Scene* CreateStage4Scene();

	Scene* CreatePatrolTestScene();

	// Json에서 값에 해당하는 것들만 먼저 가져오는 함수
	static MapData* ParseJsonData(const rapidjson::GenericValue<rapidjson::UTF8<>>& _json);

	// Json 파싱 후 맵을 만드는 함수
	static void MakeMap(MapData const* _map, Scene* _scene);
};
