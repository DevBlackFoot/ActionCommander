#include "pch.h"
#include "TitleSceneTransition.h"

#include "Animator.h"
#include "GameObject.h"
#include "GameSceneFactory.h"
#include "GameTime.h"
#include "Resources.h"
#include "SpriteRenderer.h"

using namespace SanyoniEngine;

TitleSceneTransition::TitleSceneTransition(GameObject* _gameObject, const tstring& _typeName) :
	ComponentBase(_gameObject, _typeName),
	m_TitleAnimClip(nullptr),
	m_bWaitingForDelay(false),
	m_ClipEndTimeMillis(0)
{
	GetGameObject()->GetComponent<Transform>()->SetWorldPosition(Vector2::Zero);
	GetGameObject()->AddComponent<SpriteRenderer>();
	GetGameObject()->AddComponent<Animator>();
}

void TitleSceneTransition::OnStart()
{
	m_TitleAnimClip = Resources::Instance.LoadAnimationClipFromJsonFile(TEXT("Assets/Animation/Title/Title.json"));


	Resources::Instance.LoadSpriteSheetFromJsonFile(TEXT("Assets/SpriteSheet/Title/Title1.json"));
	Resources::Instance.LoadSpriteSheetFromJsonFile(TEXT("Assets/SpriteSheet/Title/Title2.json"));
	Resources::Instance.LoadSpriteSheetFromJsonFile(TEXT("Assets/SpriteSheet/Title/Title3.json"));
	Resources::Instance.LoadSpriteSheetFromJsonFile(TEXT("Assets/SpriteSheet/Title/Title4.json"));
	Resources::Instance.LoadSpriteSheetFromJsonFile(TEXT("Assets/SpriteSheet/Title/Title5.json"));
	Resources::Instance.LoadSpriteSheetFromJsonFile(TEXT("Assets/SpriteSheet/Title/Title6.json"));
	Resources::Instance.LoadSpriteSheetFromJsonFile(TEXT("Assets/SpriteSheet/Title/Title7.json"));

	GetGameObject()->GetComponent<Animator>()->SetCurrentClip(m_TitleAnimClip);
}

void TitleSceneTransition::Tick()
{
	ComponentBase::Tick();

	const auto _animator = GetGameObject()->GetComponent<Animator>();
	if (_animator == nullptr)
		return;

	if (_animator->GetCurrentClip() == nullptr)
		return;

	if (_animator->GetReachedEnd() == true)
	{
		if (m_bWaitingForDelay == false)
		{
			m_bWaitingForDelay = true;
			m_ClipEndTimeMillis = GameTime::Instance.GetGameTimeMillis();
		}


		if (GameTime::Instance.GetGameTimeMillis() - m_ClipEndTimeMillis > 2000)
		{
			// 타이틀 애니메이션이 모두 재생된 뒤 2초 뒤에 씬을 전환합니다.
			SceneManager::Instance.ReserveLoadScene(
				GameSceneFactory().CreateStage1Scene()
			);
		}
	}
}
