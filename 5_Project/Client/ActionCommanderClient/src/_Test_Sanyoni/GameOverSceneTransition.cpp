#include "pch.h"
#include "GameOverSceneTransition.h"

#include "Animator.h"
#include "GameObject.h"
#include "GameSceneFactory.h"
#include "Input.h"
#include "Resources.h"
#include "SpriteRenderer.h"
#include "Transform.h"
#include "GameTime.h"

GameOverSceneTransition::GameOverSceneTransition(GameObject* _gameObject, const tstring& _typeName) :
	ComponentBase(_gameObject, _typeName)
{
	GetGameObject()->GetComponent<Transform>()->SetWorldPosition(Vector2::Zero);
	GetGameObject()->AddComponent<SpriteRenderer>();
	GetGameObject()->AddComponent<Animator>();
}

void GameOverSceneTransition::OnStart()
{
	m_Clip = Resources::Instance.LoadAnimationClipFromJsonFile(TEXT("Assets/Animation/GameOver/GameOver.json"));


	Resources::Instance.LoadSpriteSheetFromJsonFile(TEXT("Assets/SpriteSheet/GameOver/GameOver.json"));

	GetGameObject()->GetComponent<Animator>()->SetCurrentClip(m_Clip);

	m_DeadTimeMillis = GameTime::Instance.GetGameTimeMillis();
}

void GameOverSceneTransition::Tick()
{
	ComponentBase::Tick();

	// 일정 시간 동안 키입력 막기
	if(GameTime::Instance.GetGameTimeMillis() - m_DeadTimeMillis < 3000)
	{
		return;
	}
	
	if (Input::Instance.GetKeyState(EVirtualKey::Key_X) & EKeyStateBit::Bit_Down)
	{
		// 플레이어 조작 키 중 아무 키나 눌러 스테이지 1로 돌아갑니다.
		SceneManager::Instance.ReserveLoadScene(
			GameSceneFactory().CreateStage1Scene()
		);
	}
}
