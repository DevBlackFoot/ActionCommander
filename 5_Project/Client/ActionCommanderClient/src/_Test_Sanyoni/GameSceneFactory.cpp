#include "pch.h"
#include "GameSceneFactory.h"

#include "AABBCollider.h"
#include "ActionCommanderGameManager.h"
#include "BackgroundMovement.h"
#include "Camera.h"
#include "EGameObjectTag.h"
#include "GameObject.h"
#include "GameOverSceneTransition.h"
#include "Heart.h"
#include "MapData.h"
#include "Platform.h"
#include "Player.h"
#include "Resources.h"
#include "Scene.h"
#include "Sound.h"
#include "ShakeCamera.h"
#include "SpriteRenderer.h"
#include "TitleSceneTransition.h"
#include "Transform.h"
#include "GoblinArcher/GoblinArcher.h"
#include "GoblinBoss/GoblinBoss.h"
#include "GoblinWarrior/GoblinWarrior.h"
#include "Nepen/Nepen.h"
#include "PlayerUI.h"
#include "Portal.h"

using namespace SanyoniEngine;

Scene* GameSceneFactory::CreateTitleScene()
{
	Scene* _newScene = new Scene(TEXT("title"));

	GameObject* _transition = new GameObject(_newScene);
	_transition->AddComponent<TitleSceneTransition>();


	// 씬의 카메라를 생성합니다.
	const auto _cameraGO = new GameObject(_newScene);
	const auto _camera = _cameraGO->AddComponent<Camera>();
	_newScene->SetMainCamera(_camera);

	Sound::Instance.StopMusic();
	Sound::Instance.PlaySound("Title_BGM.wav");

	// 타이틀 씬 객체를 반환합니다.
	return _newScene;
}

Scene* GameSceneFactory::CreateGameOverScene()
{
	Scene* _newScene = new Scene(TEXT("gameover"));

	GameObject* _transition = new GameObject(_newScene);
	_transition->AddComponent<GameOverSceneTransition>();


	// 씬의 카메라를 생성합니다.
	const auto _cameraGO = new GameObject(_newScene);
	const auto _camera = _cameraGO->AddComponent<Camera>();
	_newScene->SetMainCamera(_camera);

	Sound::Instance.StopMusic();
	Sound::Instance.PlaySound("GameOver_BGM.wav");

	// 게임오버 씬 객체를 반환합니다.
	return _newScene;
}

Scene* GameSceneFactory::CreateStage1Scene()
{
	static rapidjson::GenericDocument<rapidjson::UTF8<>> _level1Json = Resources::LoadJsonFromFile(TEXT("Assets/Map/Level1.json"));
	static MapData* _map = ParseJsonData(_level1Json);

	Scene* _newScene = new Scene(TEXT("level_1"));


	// 배경 움직임 객체를 준비합니다.
	// note: 엔진의 한계로 인해 객체가 생성되는 순서에 따라 렌더링 순서가 결정되는데,
	// 이 때문에 맨 뒤에 그려질 배경 오브젝트를 다른 오브젝트들보다 먼저 생성해야 합니다.
	auto _bgMovementGO = new GameObject(_newScene);
	auto _bgMovement = _bgMovementGO->AddComponent<BackgroundMovement>();
	_bgMovement->Make(_map, _newScene);


	MakeMap(_map, _newScene);


	// 플레이어 객체를 준비합니다.
	GameObject* _player = new GameObject(_newScene, EGameObjectTag::Player);
	_player->GetComponent<Transform>()->SetWorldPosition(_map->playerPosition);
	_player->AddComponent<Player>();


	// 카메라를 준비합니다.
	auto _cameraGO = new GameObject(_newScene);
	auto _camera = _cameraGO->AddComponent<Camera>();
	_newScene->SetMainCamera(_camera);
	_cameraGO->AddComponent<ShakeCamera>()->SetPlayer(
		_player->GetComponent<Player>());
	_cameraGO->GetComponent<ShakeCamera>()->SetMapSize(_map->mapSize);
	const auto _gameManager = _cameraGO->AddComponent<ActionCommanderGameManager>();
	_gameManager->SetMap(_map);
	_gameManager->SetLevelIndex(1);

	// UI 객체를 준비합니다.
	GameObject* _playerUI = new GameObject(_newScene);
	_playerUI->GetComponent<Transform>()->SetWorldPosition(
		_camera->GetGameObject()->GetComponent<Transform>()->GetWorldPosition()
	);
	_playerUI->AddComponent<PlayerUI>()->SetPlayer(_player->GetComponent<Player>());
	_playerUI->GetComponent<PlayerUI>()->SetCamera(_camera);

	Sound::Instance.StopMusic();

	// 스테이지 1 씬 객체를 반환합니다.
	return _newScene;
}

Scene* GameSceneFactory::CreateStage2Scene()
{
	// 스테이지 2 씬 객체를 반환합니다.
	static rapidjson::GenericDocument<rapidjson::UTF8<>> _level2Json = Resources::LoadJsonFromFile(TEXT("Assets/Map/Level2.json"));
	static MapData* _map = ParseJsonData(_level2Json);

	Scene* _newScene = new Scene(TEXT("level_2"));


	// 배경 움직임 객체를 준비합니다.
	// note: 엔진의 한계로 인해 객체가 생성되는 순서에 따라 렌더링 순서가 결정되는데,
	// 이 때문에 맨 뒤에 그려질 배경 오브젝트를 다른 오브젝트들보다 먼저 생성해야 합니다.
	auto _bgMovementGO = new GameObject(_newScene);
	auto _bgMovement = _bgMovementGO->AddComponent<BackgroundMovement>();
	_bgMovement->Make(_map, _newScene);


	MakeMap(_map, _newScene);


	// 플레이어 객체를 준비합니다.
	GameObject* _player = new GameObject(_newScene, EGameObjectTag::Player);
	_player->GetComponent<Transform>()->SetWorldPosition(_map->playerPosition);
	_player->AddComponent<Player>();


	// 카메라를 준비합니다.
	auto _cameraGO = new GameObject(_newScene);
	auto _camera = _cameraGO->AddComponent<Camera>();
	_newScene->SetMainCamera(_camera);
	_cameraGO->AddComponent<ShakeCamera>()->SetPlayer(
		_player->GetComponent<Player>());
	_cameraGO->GetComponent<ShakeCamera>()->SetMapSize(_map->mapSize);
	const auto _gameManager = _cameraGO->AddComponent<ActionCommanderGameManager>();
	_gameManager->SetMap(_map);
	_gameManager->SetLevelIndex(2);

	// UI 객체를 준비합니다.
	GameObject* _playerUI = new GameObject(_newScene);
	_playerUI->GetComponent<Transform>()->SetWorldPosition(
		_camera->GetGameObject()->GetComponent<Transform>()->GetWorldPosition()
	);
	_playerUI->AddComponent<PlayerUI>()->SetPlayer(_player->GetComponent<Player>());
	_playerUI->GetComponent<PlayerUI>()->SetCamera(_camera);


	return _newScene;
}

Scene* GameSceneFactory::CreateStage3Scene()
{
	// 스테이지 3 씬 객체를 반환합니다.
	static rapidjson::GenericDocument<rapidjson::UTF8<>> _level3Json = Resources::LoadJsonFromFile(TEXT("Assets/Map/Level3.json"));
	static MapData* _map = ParseJsonData(_level3Json);

	Scene* _newScene = new Scene(TEXT("level_3"));


	// 배경 움직임 객체를 준비합니다.
	// note: 엔진의 한계로 인해 객체가 생성되는 순서에 따라 렌더링 순서가 결정되는데,
	// 이 때문에 맨 뒤에 그려질 배경 오브젝트를 다른 오브젝트들보다 먼저 생성해야 합니다.
	auto _bgMovementGO = new GameObject(_newScene);
	auto _bgMovement = _bgMovementGO->AddComponent<BackgroundMovement>();
	_bgMovement->Make(_map, _newScene);


	MakeMap(_map, _newScene);


	// 플레이어 객체를 준비합니다.
	GameObject* _player = new GameObject(_newScene, EGameObjectTag::Player);
	_player->GetComponent<Transform>()->SetWorldPosition(_map->playerPosition);
	_player->AddComponent<Player>();


	// 카메라를 준비합니다.
	auto _cameraGO = new GameObject(_newScene);
	auto _camera = _cameraGO->AddComponent<Camera>();
	_newScene->SetMainCamera(_camera);
	_cameraGO->AddComponent<ShakeCamera>()->SetPlayer(
		_player->GetComponent<Player>());
	_cameraGO->GetComponent<ShakeCamera>()->SetMapSize(_map->mapSize);
	const auto _gameManager = _cameraGO->AddComponent<ActionCommanderGameManager>();
	_gameManager->SetMap(_map);
	_gameManager->SetLevelIndex(3);

	// UI 객체를 준비합니다.
	GameObject* _playerUI = new GameObject(_newScene);
	_playerUI->AddComponent<PlayerUI>()->SetPlayer(_player->GetComponent<Player>());
	_playerUI->GetComponent<PlayerUI>()->SetCamera(_camera);

	return _newScene;
}

Scene* GameSceneFactory::CreateStage4Scene()
{
	// 스테이지 4 씬 객체를 반환합니다.
	static rapidjson::GenericDocument<rapidjson::UTF8<>> _level4Json = Resources::LoadJsonFromFile(TEXT("Assets/Map/Level4.json"));
	static MapData* _map = ParseJsonData(_level4Json);

	Scene* _newScene = new Scene(TEXT("level_4"));


	// 배경 움직임 객체를 준비합니다.
	// note: 엔진의 한계로 인해 객체가 생성되는 순서에 따라 렌더링 순서가 결정되는데,
	// 이 때문에 맨 뒤에 그려질 배경 오브젝트를 다른 오브젝트들보다 먼저 생성해야 합니다.
	auto _bgMovementGO = new GameObject(_newScene);
	auto _bgMovement = _bgMovementGO->AddComponent<BackgroundMovement>();
	_bgMovement->Make(_map, _newScene);


	MakeMap(_map, _newScene);


	// 플레이어 객체를 준비합니다.
	GameObject* _player = new GameObject(_newScene, EGameObjectTag::Player);
	_player->GetComponent<Transform>()->SetWorldPosition(_map->playerPosition);
	_player->AddComponent<Player>();


	// 카메라를 준비합니다.
	auto _cameraGO = new GameObject(_newScene);
	auto _camera = _cameraGO->AddComponent<Camera>();
	_newScene->SetMainCamera(_camera);
	_cameraGO->AddComponent<ShakeCamera>()->SetPlayer(
		_player->GetComponent<Player>());
	_cameraGO->GetComponent<ShakeCamera>()->SetMapSize(_map->mapSize);
	const auto _gameManager = _cameraGO->AddComponent<ActionCommanderGameManager>();
	_gameManager->SetMap(_map);
	_gameManager->SetLevelIndex(4);

	// UI 객체를 준비합니다.
	GameObject* _playerUI = new GameObject(_newScene);
	_playerUI->GetComponent<Transform>()->SetWorldPosition(
		_camera->GetGameObject()->GetComponent<Transform>()->GetWorldPosition()
	);
	_playerUI->AddComponent<PlayerUI>()->SetPlayer(_player->GetComponent<Player>());
	_playerUI->GetComponent<PlayerUI>()->SetCamera(_camera);

	return _newScene;
}

Scene* GameSceneFactory::CreatePatrolTestScene()
{
	// 테스트 스테이지 씬 객체를 반환합니다.
	static rapidjson::GenericDocument<rapidjson::UTF8<>> _level4Json = Resources::LoadJsonFromFile(TEXT("Assets/Map/AITest.json"));
	static MapData* _map = ParseJsonData(_level4Json);

	Scene* _newScene = new Scene(TEXT("PatrolScene"));


	// 배경 움직임 객체를 준비합니다.
	// note: 엔진의 한계로 인해 객체가 생성되는 순서에 따라 렌더링 순서가 결정되는데,
	// 이 때문에 맨 뒤에 그려질 배경 오브젝트를 다른 오브젝트들보다 먼저 생성해야 합니다.
	auto _bgMovementGO = new GameObject(_newScene);
	auto _bgMovement = _bgMovementGO->AddComponent<BackgroundMovement>();
	_bgMovement->Make(_map, _newScene);


	MakeMap(_map, _newScene);


	// 플레이어 객체를 준비합니다.
	GameObject* _player = new GameObject(_newScene, EGameObjectTag::Player);
	_player->GetComponent<Transform>()->SetWorldPosition(_map->playerPosition);
	_player->AddComponent<Player>();


	// 카메라를 준비합니다.
	auto _cameraGO = new GameObject(_newScene);
	auto _camera = _cameraGO->AddComponent<Camera>();
	_newScene->SetMainCamera(_camera);
	_cameraGO->AddComponent<ShakeCamera>()->SetPlayer(
		_player->GetComponent<Player>());
	_cameraGO->GetComponent<ShakeCamera>()->SetMapSize(_map->mapSize);
	_cameraGO->AddComponent<ActionCommanderGameManager>()->SetMap(_map);

	// UI 객체를 준비합니다.
	GameObject* _playerUI = new GameObject(_newScene);
	_playerUI->AddComponent<PlayerUI>()->SetPlayer(_player->GetComponent<Player>());

	return _newScene;
}

MapData* GameSceneFactory::ParseJsonData(const rapidjson::GenericValue<rapidjson::UTF8<>>& _json)
{
	MapData* _outMap = new MapData();

	const auto& _realJson = _json.MemberBegin()->value;

	for (auto _iter = _realJson.MemberBegin(); _iter != _realJson.MemberEnd(); _iter++)
	{
		std::string _keyName = _iter->name.GetString();

		if (_keyName == "MapSize")
		{
			const auto& _mapSizeIter = _iter->value.GetArray().begin();

			_outMap->mapSize.x = _mapSizeIter->GetInt() * 64;
			_outMap->mapSize.y = (_mapSizeIter + 1)->GetInt() * 64;
		}
		else if (_keyName == "MonsterCount")
		{
			_outMap->monsterCount = _iter->value.GetInt();
		}
		else if (_keyName == "TileSize")
		{
			_outMap->tileSize = _iter->value.GetInt();
		}
		else if (_keyName == "TileMapData")
		{
			for (auto _tileArrayIter = _iter->value.GetArray().Begin();
				_tileArrayIter != _iter->value.GetArray().End();
				++_tileArrayIter)
			{
				const auto _tileIter = _tileArrayIter->GetArray().Begin();

				MapData::TileData _newTile;

				_newTile.left = _tileIter->GetInt(); // left
				_newTile.top = (_tileIter + 1)->GetInt(); // top
				_newTile.attribute = static_cast<TileAttribute>(
					(_tileIter + 2)->GetInt()
					);

				_outMap->tiles.push_back(_newTile);
			}
		}
		else if (_keyName == "ObjectMapData")
		{
			for (auto _objectArrayIter = _iter->value.GetArray().Begin();
				_objectArrayIter != _iter->value.GetArray().End();
				++_objectArrayIter)
			{
				const auto _objectIter = _objectArrayIter->GetArray().Begin();

				MapData::ObjectData _newObject;

				_newObject.left = _objectIter->GetInt();
				_newObject.top = (_objectIter + 1)->GetInt();
				_newObject.centerX = (_objectIter + 2)->GetInt();
				_newObject.centerY = (_objectIter + 3)->GetInt();
				_newObject.bmpWidth = (_objectIter + 4)->GetFloat();
				_newObject.bmpHeight = (_objectIter + 5)->GetFloat();
				_newObject.attribute = static_cast<ObjectAttribute>(
					(_objectIter + 6)->GetInt()
					);


				if (_newObject.attribute == ObjectAttribute::Player)
				{
					// 플레이어의 위치는 기본 위치보다 살짝 띄워진 위치로 초기화합니다.
					// 플레이어가 게임을 시작하자마자 바닥을 뚫고 내려가는 것을 방지하기 위함입니다.
					_outMap->playerPosition = { static_cast<float>(_newObject.centerX), static_cast<float>(_newObject.centerY - 20) };
				}

				_outMap->objects.push_back(_newObject);
			}
		}
		else if (_keyName == "Colliders")
		{
			for (auto _colliderArrayIter = _iter->value.GetArray().Begin();
				_colliderArrayIter != _iter->value.GetArray().End();
				++_colliderArrayIter)
			{
				const auto _colliderIter = _colliderArrayIter->GetArray().Begin();

				Collision::AABBBounds _newBounds{
					static_cast<float>(_colliderIter->GetInt()),
					static_cast<float>((_colliderIter + 1)->GetInt()),
					static_cast<float>((_colliderIter + 2)->GetInt()),
					static_cast<float>((_colliderIter + 3)->GetInt())
				};

				_outMap->colliders.push_back(_newBounds);
			}
		}
	}

	return _outMap;
}

void GameSceneFactory::MakeMap(MapData const* _map, Scene* _scene)
{
	static std::unordered_map<TileAttribute, tstring> _tileSpriteMap{
		{
			// grounds
			{TileAttribute::GroundLeftTop, TEXT("Assets/Map/Tiles/top_L.png")},
			{TileAttribute::GroundTop, TEXT("Assets/Map/Tiles/top.png")},
			{TileAttribute::GroundRightTop, TEXT("Assets/Map/Tiles/top_R.png")},
			{TileAttribute::GroundLeft, TEXT("Assets/Map/Tiles/middle_L.png")},
			{TileAttribute::GroundCenter, TEXT("Assets/Map/Tiles/middle.png")},
			{TileAttribute::GroundRight, TEXT("Assets/Map/Tiles/middle_R.png")},
			{TileAttribute::GroundLeftBottom, TEXT("Assets/Map/Tiles/bottom_L.png")},
			{TileAttribute::GroundBottom, TEXT("Assets/Map/Tiles/bottom.png")},
			{TileAttribute::GroundRightBottom,TEXT("Assets/Map/Tiles/bottom_R.png")},
			{TileAttribute::GroundDiaLeftTop,TEXT("Assets/Map/Tiles/diaLT.png")},
			{TileAttribute::GroundDiaRightTop,TEXT("Assets/Map/Tiles/diaRT.png")},
			{TileAttribute::GroundDiaLeftBottom,TEXT("Assets/Map/Tiles/diaLB.png")},
			{TileAttribute::GroundDiaRightBottom,TEXT("Assets/Map/Tiles/diaRB.png")},
			// down jump not allowed
			{TileAttribute::NotJumpPlatLeft,TEXT("Assets/Map/Tiles/H_l.png")},
			{TileAttribute::NotJumpPlatCenter,TEXT("Assets/Map/Tiles/H_m.png")},
			{TileAttribute::NotJumpPlatRight,TEXT("Assets/Map/Tiles/H_r.png")},
			// down jump allowed
			{TileAttribute::JumpPlatLeft,TEXT("Assets/Map/Tiles/L_l.png")},
			{TileAttribute::JumpPlatCenter,TEXT("Assets/Map/Tiles/L_m.png")},
			{TileAttribute::JumpPlatRight,TEXT("Assets/Map/Tiles/L_r.png")},
		}
	};

	static std::unordered_map<TileAttribute, EGameObjectTag> _tileTagMap{
		{
			// down jump not allowed
			{TileAttribute::NotJumpPlatLeft, EGameObjectTag::Platform},
			{TileAttribute::NotJumpPlatCenter, EGameObjectTag::Platform},
			{TileAttribute::NotJumpPlatRight, EGameObjectTag::Platform},
			// down jump allowed
			{TileAttribute::JumpPlatLeft, EGameObjectTag::Platform},
			{TileAttribute::JumpPlatCenter, EGameObjectTag::Platform},
			{TileAttribute::JumpPlatRight, EGameObjectTag::Platform},
		}
	};

	static std::unordered_map<ObjectAttribute, tstring> _objectSpriteMap{
		{
			{ObjectAttribute::ObjectLeaf0,TEXT("Assets/Map/Objects/leaf1.png")},
			{ObjectAttribute::ObjectLeaf1,TEXT("Assets/Map/Objects/leaf2.png")},
			{ObjectAttribute::ObjectLeaf2,TEXT("Assets/Map/Objects/leaf3.png")},
			{ObjectAttribute::ObjectLeaf3,TEXT("Assets/Map/Objects/leaf4.png")},
			{ObjectAttribute::ObjectLeaf4,TEXT("Assets/Map/Objects/leaf5.png")},
			{ObjectAttribute::ObjectRock0,TEXT("Assets/Map/Objects/rock.png")},
			{ObjectAttribute::ObjectSign0,TEXT("Assets/Map/Objects/sign.png")},
			{ObjectAttribute::ObjectTree0,TEXT("Assets/Map/Objects/tree.png")},
			{ObjectAttribute::ObjectTree1,TEXT("Assets/Map/Objects/tree2.png")},
			// flip objects
			{ObjectAttribute::ObjectLeaf0Flip,TEXT("Assets/Map/Objects/leaf1flip.png")},
			{ObjectAttribute::ObjectLeaf1Flip,TEXT("Assets/Map/Objects/leaf2flip.png")},
			{ObjectAttribute::ObjectLeaf2Flip,TEXT("Assets/Map/Objects/leaf3flip.png")},
			{ObjectAttribute::ObjectLeaf3Flip,TEXT("Assets/Map/Objects/leaf4flip.png")},
			{ObjectAttribute::ObjectLeaf4Flip,TEXT("Assets/Map/Objects/leaf5flip.png")},
			{ObjectAttribute::ObjectRock0Flip,TEXT("Assets/Map/Objects/rockflip.png")},
			{ObjectAttribute::ObjectTree0Flip,TEXT("Assets/Map/Objects/treeflip.png")},
			{ObjectAttribute::ObjectTree1Flip,TEXT("Assets/Map/Objects/tree2flip.png")},
			// Additional foreground
			{ObjectAttribute::ObjectTree2,TEXT("Assets/Map/Objects/tree3.png")},
			{ObjectAttribute::ObjectTree2Flip,TEXT("Assets/Map/Objects/tree3flip.png")},
			{ObjectAttribute::ObjectRock1,TEXT("Assets/Map/Objects/rock2.png")},
			{ObjectAttribute::ObjectRock1Flip,TEXT("Assets/Map/Objects/rock2flip.png")},
			{ObjectAttribute::ObjectSign1,TEXT("Assets/Map/Objects/sign2.png")},
			{ObjectAttribute::ObjectSign2,TEXT("Assets/Map/Objects/sign3.png")},
			{ObjectAttribute::ObjectSign3,TEXT("Assets/Map/Objects/sign4.png")},
			// etc
			{ObjectAttribute::Heart,TEXT("Assets/Map/Objects/heartItem.png")},
			{ObjectAttribute::Portal,TEXT("Assets/Map/Objects/portal.png")},
			{ObjectAttribute::ClearEng,TEXT("Assets/Map/Objects/Clear_Eng.png")},
			{ObjectAttribute::ClearKor,TEXT("Assets/Map/Objects/Clear_Kor.png")},
			{ObjectAttribute::ClearName,TEXT("Assets/Map/Objects/Clear_Name.png")},

		}
	};

	for (const auto _tile : _map->tiles)
	{
		if (_tile.attribute == TileAttribute::None)
		{
			// 빈 타일인 경우 무시합니다.
			continue;
		}

		GameObject* _go = new GameObject(_scene);

		// 타일 오브젝트의 world position을 설정합니다.

		_go->GetComponent<Transform>()->SetWorldPosition({
			static_cast<float>(_tile.left + _map->tileSize / 2),
			static_cast<float>(_tile.top + _map->tileSize / 2)
			});

		if ((static_cast<int>(_tile.attribute) & 0x0300) == 0x0300)
		{
			// 예외적으로 점프 가능한 작은 타일의 경우, 위치를 직접 조정합니다.
			_go->GetComponent<Transform>()->SetWorldPosition({
				static_cast<float>(_tile.left + _map->tileSize / 2),
				static_cast<float>(_tile.top + _map->tileSize / 4)
				});
		}

		// 타일 오브젝트의 Sprite를 설정합니다.

		if (const auto _sprite = _tileSpriteMap.find(_tile.attribute);
			_sprite != _tileSpriteMap.end())
		{
			_go->AddComponent<SpriteRenderer>()->SetSprite(
				Resources::Instance.GetSpriteByName(_sprite->second)
			);
		}

		// 타일 오브젝트의 Tag를 설정합니다.

		if (const auto _tag = _tileTagMap.find(_tile.attribute);
			_tag != _tileTagMap.end())
		{
			// 별도의 태그가 지정되어 있는 attribute인 경우에 이쪽이 실행됩니다.
			_go->SetTag(_tag->second);
		}
		else
		{
			// 그렇지 않은 타일 오브젝트는 Wall 태그로 설정됩니다.
			_go->SetTag(EGameObjectTag::Wall);
		}
	}

	for (const auto& _collider : _map->colliders)
	{
		GameObject* _go = new GameObject(_scene);
		_go->GetComponent<Transform>()->SetWorldPosition(_collider.GetCenter());
		_go->AddComponent<AABBCollider>()->SetWidth(_collider.GetWidth());
		_go->GetComponent<AABBCollider>()->SetHeight(_collider.GetHeight());

		if (_collider.GetHeight() == _map->tileSize / 2)
		{
			// 점프 가능한 작은 타일의 경우, 플랫폼 속성을 설정합니다.

			_go->AddComponent<Platform>()->SetAllowJumpThrough(true);
			_go->GetComponent<Platform>()->SetAllowJumpDown(true);
		}
		else
		{
			_go->AddComponent<Platform>()->SetAllowJumpThrough(false);
			_go->GetComponent<Platform>()->SetAllowJumpDown(false);
		}
	}

	for (const auto& _obj : _map->objects)
	{
		GameObject* _go = new GameObject(_scene);
		_go->GetComponent<Transform>()->SetWorldPosition({ static_cast<float>(_obj.centerX), static_cast<float>(_obj.centerY) });
		_go->AddComponent<SpriteRenderer>();

		// 오브젝트의 sprite를 설정합니다.

		if (const auto _sprite = _objectSpriteMap.find(_obj.attribute);
			_sprite != _objectSpriteMap.end())
		{
			if (_obj.attribute != ObjectAttribute::Portal)
			{
				_go->AddComponent<SpriteRenderer>()->SetSprite(
					Resources::Instance.GetSpriteByName(_sprite->second)
				);
			}
		}

		// 예외적으로 player와 몬스터들 등 특수한 오브젝트들의 생성에 대해서는
		// 컴포넌트를 추가하는 등의 추가 작업을 해줍니다.

		if (_obj.attribute == ObjectAttribute::Player)
		{
		}
		else if (_obj.attribute == ObjectAttribute::Stage1Monster)
		{
			// nepen
			_go->AddComponent<Nepen>()->SetLookDirection(-1);
			_go->GetComponent<Transform>()->SetWorldPosition({ static_cast<float>(_obj.centerX), static_cast<float>(_obj.centerY - 64) });
		}
		else if (_obj.attribute == ObjectAttribute::Stage2Monster0)
		{
			// goblin warrior
			_go->AddComponent<GoblinWarrior>()->SetLookDirection(-1);
			_go->GetComponent<Transform>()->SetWorldPosition({ static_cast<float>(_obj.centerX), static_cast<float>(_obj.centerY - 64) });
		}
		else if (_obj.attribute == ObjectAttribute::Stage2Monster1)
		{
			// goblin archer
			_go->AddComponent<GoblinArcher>()->SetLookDirection(-1);
			_go->GetComponent<Transform>()->SetWorldPosition({ static_cast<float>(_obj.centerX), static_cast<float>(_obj.centerY - 64) });
		}
		else if (_obj.attribute == ObjectAttribute::Stage3Boss)
		{
			// goblin boss
			_go->AddComponent<GoblinBoss>()->SetLookDirection(1);
			_go->GetComponent<Transform>()->SetWorldPosition({ static_cast<float>(_obj.centerX), static_cast<float>(_obj.centerY - 64) });
		}
		else if (_obj.attribute == ObjectAttribute::Portal)
		{
			// portal
			_go->AddComponent<Portal>();
		}
		else if (_obj.attribute == ObjectAttribute::Heart)
		{
			// heart
			_go->AddComponent<Heart>();
		}
	}
}
