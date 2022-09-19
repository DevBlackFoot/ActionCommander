#include "pch.h"
// ���� ��� ��Ŭ���
#include "SanyoniEngine.h"
#include "SanyoniEngine_types.h"
#include "JHTestApplication.h"

// ����� ���� ������Ʈ ��� ��Ŭ���
#include "AABBCollider.h"
#include "Animator.h"
#include "Camera.h"
#include "Damagable.h"
#include "GameObject.h"
#include "GameSceneFactory.h"
#include "GameTime.h"
#include "GoblinWarrior/GoblinWarrior.h"
#include "GoblinArcher/GoblinArcher.h"
#include "Health.h"
#include "Input.h"
#include "Nepen/Nepen.h"
#include "Scene.h"
#include "SceneManager.h"
#include "Screen.h"
#include "ShakeCamera.h"
#include "Sound.h"
#include "SpriteRenderer.h"
#include "Platform.h"
#include "Player.h"
#include "Resources.h"
#include "RigidBody.h"
#include "RigidBodyController.h"
#include "Transform.h"


using namespace SanyoniEngine;

void JHTestApplication::OnStart()
{
	Application::OnStart();

	Sound::Instance.InitSound();
	Sound::Instance.PlayLoop("Main_BGM.wav");

	// ���� �� �̹��� ���� ��������
	InitBitmapResource();

	// Json���� �Ľ�
	//rapidjson::GenericDocument<rapidjson::UTF8<>> _level3Json = Resources::Instance.LoadJsonFromFile(TEXT("Assets/Map/Level3.json"));

	Scene* _scene = GameSceneFactory().CreateTitleScene();
	//Scene* _scene = GameSceneFactory().CreatePatrolTestScene();
	SceneManager::Instance.ReserveLoadScene(_scene);

	const auto _font = Resources::Instance.LoadFontCollectionFromFile(TEXT("Assets/Font/hyũ����Żm-yoond1004_0.ttf"));
	m_TextFormat = Resources::Instance.CreateTextFormat(_font, 30);

	m_PrevTimeMillis = GameTime::Instance.GetGameTimeMillis();
}

void JHTestApplication::OnUpdate()
{
	Application::OnUpdate();

	if(Input::Instance.GetKeyPress(EVirtualKey::LeftControl) == true)
	{
		// ctrl �� �����Ͽ�, ���� Ű�� ���� �� ��ȯ�� �մϴ�.

		if (Input::Instance.GetKeyState(EVirtualKey::F4) & EKeyStateBit::Bit_Down)
		{
			Scene* _scene = GameSceneFactory().CreateTitleScene();
			SceneManager::Instance.ReserveLoadScene(_scene);
		}
		else if (Input::Instance.GetKeyState(EVirtualKey::F5) & EKeyStateBit::Bit_Down)
		{
			Scene* _scene = GameSceneFactory().CreateStage1Scene();
			SceneManager::Instance.ReserveLoadScene(_scene);
		}
		else if (Input::Instance.GetKeyState(EVirtualKey::F6) & EKeyStateBit::Bit_Down)
		{
			Sound::Instance.StopMusic();
			Scene* _scene = GameSceneFactory().CreateStage2Scene();
			SceneManager::Instance.ReserveLoadScene(_scene);
		}
		else if (Input::Instance.GetKeyState(EVirtualKey::F7) & EKeyStateBit::Bit_Down)
		{
			Sound::Instance.StopMusic();
			Scene* _scene = GameSceneFactory().CreateStage3Scene();
			SceneManager::Instance.ReserveLoadScene(_scene);
		}
		else if (Input::Instance.GetKeyState(EVirtualKey::F8) & EKeyStateBit::Bit_Down)
		{
			Sound::Instance.StopMusic();
			Scene* _scene = GameSceneFactory().CreateStage4Scene();
			SceneManager::Instance.ReserveLoadScene(_scene);
		}
	}
}

void JHTestApplication::InitBitmapResource()
{
	// Ÿ�ϵ��� ��������Ʈ��Ʈ�� �־��ش� => ���� ¥���� ��������Ʈ��Ʈ = ��������Ʈ
	Resources::Instance.LoadSpriteSheetFromFile(TEXT("Assets/Map/Tiles/top_L.png"));
	Resources::Instance.LoadSpriteSheetFromFile(TEXT("Assets/Map/Tiles/top.png"));
	Resources::Instance.LoadSpriteSheetFromFile(TEXT("Assets/Map/Tiles/top_R.png"));
	Resources::Instance.LoadSpriteSheetFromFile(TEXT("Assets/Map/Tiles/middle_L.png"));
	Resources::Instance.LoadSpriteSheetFromFile(TEXT("Assets/Map/Tiles/middle.png"));
	Resources::Instance.LoadSpriteSheetFromFile(TEXT("Assets/Map/Tiles/middle_R.png"));
	Resources::Instance.LoadSpriteSheetFromFile(TEXT("Assets/Map/Tiles/bottom_L.png"));
	Resources::Instance.LoadSpriteSheetFromFile(TEXT("Assets/Map/Tiles/bottom.png"));
	Resources::Instance.LoadSpriteSheetFromFile(TEXT("Assets/Map/Tiles/bottom_R.png"));

	Resources::Instance.LoadSpriteSheetFromFile(TEXT("Assets/Map/Tiles/diaLT.png"));
	Resources::Instance.LoadSpriteSheetFromFile(TEXT("Assets/Map/Tiles/diaLB.png"));
	Resources::Instance.LoadSpriteSheetFromFile(TEXT("Assets/Map/Tiles/diaRT.png"));
	Resources::Instance.LoadSpriteSheetFromFile(TEXT("Assets/Map/Tiles/diaRB.png"));

	Resources::Instance.LoadSpriteSheetFromFile(TEXT("Assets/Map/Tiles/H_l.png"));
	Resources::Instance.LoadSpriteSheetFromFile(TEXT("Assets/Map/Tiles/H_m.png"));
	Resources::Instance.LoadSpriteSheetFromFile(TEXT("Assets/Map/Tiles/H_r.png"));

	Resources::Instance.LoadSpriteSheetFromFile(TEXT("Assets/Map/Tiles/L_l.png"));
	Resources::Instance.LoadSpriteSheetFromFile(TEXT("Assets/Map/Tiles/L_m.png"));
	Resources::Instance.LoadSpriteSheetFromFile(TEXT("Assets/Map/Tiles/L_r.png"));

	// Object���� ��������Ʈ ��Ʈ�� �־��ش�.
	// FG
	Resources::Instance.LoadSpriteSheetFromFile(TEXT("Assets/Map/Objects/leaf1.png"));
	Resources::Instance.LoadSpriteSheetFromFile(TEXT("Assets/Map/Objects/leaf2.png"));
	Resources::Instance.LoadSpriteSheetFromFile(TEXT("Assets/Map/Objects/leaf3.png"));
	Resources::Instance.LoadSpriteSheetFromFile(TEXT("Assets/Map/Objects/leaf4.png"));
	Resources::Instance.LoadSpriteSheetFromFile(TEXT("Assets/Map/Objects/leaf5.png"));
	Resources::Instance.LoadSpriteSheetFromFile(TEXT("Assets/Map/Objects/rock.png"));
	Resources::Instance.LoadSpriteSheetFromFile(TEXT("Assets/Map/Objects/rock2.png"));
	Resources::Instance.LoadSpriteSheetFromFile(TEXT("Assets/Map/Objects/sign.png"));
	Resources::Instance.LoadSpriteSheetFromFile(TEXT("Assets/Map/Objects/sign2.png"));
	Resources::Instance.LoadSpriteSheetFromFile(TEXT("Assets/Map/Objects/sign3.png"));
	Resources::Instance.LoadSpriteSheetFromFile(TEXT("Assets/Map/Objects/sign4.png"));
	Resources::Instance.LoadSpriteSheetFromFile(TEXT("Assets/Map/Objects/tree.png"));
	Resources::Instance.LoadSpriteSheetFromFile(TEXT("Assets/Map/Objects/tree2.png"));
	Resources::Instance.LoadSpriteSheetFromFile(TEXT("Assets/Map/Objects/tree3.png"));
	// FG Flip
	Resources::Instance.LoadSpriteSheetFromFile(TEXT("Assets/Map/Objects/leaf1flip.png"));
	Resources::Instance.LoadSpriteSheetFromFile(TEXT("Assets/Map/Objects/leaf2flip.png"));
	Resources::Instance.LoadSpriteSheetFromFile(TEXT("Assets/Map/Objects/leaf3flip.png"));
	Resources::Instance.LoadSpriteSheetFromFile(TEXT("Assets/Map/Objects/leaf4flip.png"));
	Resources::Instance.LoadSpriteSheetFromFile(TEXT("Assets/Map/Objects/leaf5flip.png"));
	Resources::Instance.LoadSpriteSheetFromFile(TEXT("Assets/Map/Objects/rockflip.png"));
	Resources::Instance.LoadSpriteSheetFromFile(TEXT("Assets/Map/Objects/rock2flip.png"));
	Resources::Instance.LoadSpriteSheetFromFile(TEXT("Assets/Map/Objects/treeflip.png"));
	Resources::Instance.LoadSpriteSheetFromFile(TEXT("Assets/Map/Objects/tree2flip.png"));
	Resources::Instance.LoadSpriteSheetFromFile(TEXT("Assets/Map/Objects/tree3flip.png"));

	Resources::Instance.LoadSpriteSheetFromFile(TEXT("Assets/Map/Objects/heartItem.png"));

	// ETC
	Resources::Instance.LoadSpriteSheetFromFile(TEXT("Assets/Map/Objects/Clear_Eng.png"));
	Resources::Instance.LoadSpriteSheetFromFile(TEXT("Assets/Map/Objects/Clear_Kor.png"));
	Resources::Instance.LoadSpriteSheetFromFile(TEXT("Assets/Map/Objects/Clear_Name.png"));

	// BG
	Resources::Instance.LoadSpriteSheetFromFile(TEXT("Assets/Map/BG/background.png"));
	Resources::Instance.LoadSpriteSheetFromFile(TEXT("Assets/Map/BG/backgroundFrontTree.png"));
	Resources::Instance.LoadSpriteSheetFromFile(TEXT("Assets/Map/BG/backgroundBackTree.png"));
}
