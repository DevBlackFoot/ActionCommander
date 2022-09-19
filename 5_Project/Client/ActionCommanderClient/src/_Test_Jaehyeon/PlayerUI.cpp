#include "pch.h"
#include "PlayerUI.h"

#include "Camera.h"
#include "GameObject.h"
#include "Player.h"
#include "Resources.h"
#include "GameTime.h"
#include "Health.h"
#include "Screen.h"
#include "Input.h"
#include "ActionCommanderGameManager.h"
#include "SceneManager.h"
#include "ShakeCamera.h"

PlayerUI::PlayerUI(SanyoniEngine::GameObject* _gameObject, tstring _typeName) :
	ComponentBase(_gameObject, _typeName),
	m_Player(nullptr),
	m_Camera(nullptr),
	m_NowPlayerHealth(0),
	m_LastEndSkill1TimeMillis(0),
	m_LastEndSkill2TimeMillis(0),
	m_LastEndSkill3TimeMillis(0),
	m_LastEndSkill4TimeMillis(0),
	m_ChangeSkillTab(false),
	m_Skill1CoolTimeRatio(0),
	m_Skill2CoolTimeRatio(0),
	m_Skill3CoolTimeRatio(0),
	m_Skill4CoolTimeRatio(0)
{

}

void PlayerUI::OnStart()
{
	ComponentBase::OnStart();

	// 각종 리소스들을 가져온다.
	Resources::Instance.LoadSpriteSheetFromFile(TEXT("Assets/UI/PlayerHPBarUnderLeft.png"));
	Resources::Instance.LoadSpriteSheetFromFile(TEXT("Assets/UI/PlayerHPBarUnderRight.png"));
	Resources::Instance.LoadSpriteSheetFromFile(TEXT("Assets/UI/PlayerHPBar.png"));

	Resources::Instance.LoadSpriteSheetFromFile(TEXT("Assets/UI/SkillTabGrayRight.png"));
	Resources::Instance.LoadSpriteSheetFromFile(TEXT("Assets/UI/SkillTabGrayLeft.png"));
	Resources::Instance.LoadSpriteSheetFromFile(TEXT("Assets/UI/SkillTabRight.png"));
	Resources::Instance.LoadSpriteSheetFromFile(TEXT("Assets/UI/SkillTabLeft.png"));

	Resources::Instance.LoadSpriteSheetFromFile(TEXT("Assets/UI/Skill1CoolTimeBar.png"));
	Resources::Instance.LoadSpriteSheetFromFile(TEXT("Assets/UI/Skill2CoolTimeBar.png"));
	Resources::Instance.LoadSpriteSheetFromFile(TEXT("Assets/UI/Skill3CoolTimeBar.png"));
	Resources::Instance.LoadSpriteSheetFromFile(TEXT("Assets/UI/Skill4CoolTimeBar.png"));

	// 리소스를 바인딩 해준다.
	const auto _font = Resources::Instance.LoadFontCollectionFromFile(TEXT("Assets/Font/DungGeunMo.ttf"));
	m_TextFormat = Resources::Instance.CreateTextFormat(_font, 25);
}

void PlayerUI::PreTick()
{
	ComponentBase::PreTick();

	// 플레이어 정보가 없으면 리턴한다.
	if (m_Player == nullptr)
		return;

	// 쿨타임 등을 계산

	m_PlayerHealthRatio = static_cast<float>(m_NowPlayerHealth)
		/ static_cast<float>(m_Player->GetGameObject()->GetComponent<Health>()->GetMaxHealth());

	// 스킬 쿨타임을 검사해줍니다.
		// 추가로 스킬 사용 여부를 확인합니다.
	long long _goneSkill1TimeMillis
		= GameTime::Instance.GetGameTimeMillis() - m_LastEndSkill1TimeMillis;
	long long _goneSkill2TimeMillis
		= GameTime::Instance.GetGameTimeMillis() - m_LastEndSkill2TimeMillis;
	long long _goneSkill3TimeMillis
		= GameTime::Instance.GetGameTimeMillis() - m_LastEndSkill3TimeMillis;
	long long _goneSkill4TimeMillis
		= GameTime::Instance.GetGameTimeMillis() - m_LastEndSkill4TimeMillis;

	const auto _camera = SceneManager::Instance.GetActiveScene()->GetMainCamera()->GetGameObject()->GetComponent<ShakeCamera>();
	if (_camera == nullptr)
	{
		return;
	}

	float _skill1CoolTime, _skill2CoolTime, _skill3CoolTime, _skill4CoolTime;

	if (_camera->GetClear())
	{
		_skill1CoolTime = 100.f;
		_skill2CoolTime = 100.f;
		_skill3CoolTime = 100.f;
		_skill4CoolTime = 100.f;
	}
	else
	{
		_skill1CoolTime = 6000.f;
		_skill2CoolTime = 15000.f;
		_skill3CoolTime = 6000.f;
		_skill4CoolTime = 9000.f;
	}

	m_Skill1CoolTimeRatio = (_skill1CoolTime - _goneSkill1TimeMillis) / _skill1CoolTime;
	m_Skill2CoolTimeRatio = (_skill2CoolTime - _goneSkill2TimeMillis) / _skill2CoolTime;
	m_Skill3CoolTimeRatio = (_skill3CoolTime - _goneSkill3TimeMillis) / _skill3CoolTime;
	m_Skill4CoolTimeRatio = (_skill4CoolTime - _goneSkill4TimeMillis) / _skill4CoolTime;

	if (_goneSkill1TimeMillis > _skill1CoolTime || m_Player->GetSkill1Able())
	{
		m_Player->SetSkill1Able(true);
		m_Skill1CoolTimeRatio = 0;
	}

	if (_goneSkill2TimeMillis > _skill2CoolTime || m_Player->GetSkill2Able())
	{
		m_Player->SetSkill2Able(true);
		m_Skill2CoolTimeRatio = 0;
	}

	if (_goneSkill3TimeMillis > _skill3CoolTime || m_Player->GetSkill3Able())
	{
		m_Player->SetSkill3Able(true);
		m_Skill3CoolTimeRatio = 0;
	}

	if (_goneSkill4TimeMillis > _skill4CoolTime || m_Player->GetSkill4Able())
	{
		m_Player->SetSkill4Able(true);
		m_Skill4CoolTimeRatio = 0;
	}

}

void PlayerUI::Tick()
{
	ComponentBase::Tick();

	if (Input::Instance.GetKeyState(EVirtualKey::Insert) & EKeyStateBit::Bit_Down)
	{
		m_ChangeSkillTab = !m_ChangeSkillTab;
	}
}

void PlayerUI::PostTick()
{
	ComponentBase::PostTick();

	// 틱 이후 변경된 수치를 가져옴.
	if (m_Player == nullptr)
		return;

	m_LastEndSkill1TimeMillis = m_Player->GetLastEndSkill1TimeMills();
	m_LastEndSkill2TimeMillis = m_Player->GetLastEndSkill2TimeMills();
	m_LastEndSkill3TimeMillis = m_Player->GetLastEndSkill3TimeMills();
	m_LastEndSkill4TimeMillis = m_Player->GetLastEndSkill4TimeMills();

	m_NowPlayerHealth = m_Player->GetGameObject()->GetComponent<Health>()->GetHealth();
}

void PlayerUI::OnRender()
{
	// 왼쪽 얼굴
	if (m_Player == nullptr)
		return;

	Vector2 _screenSize = Screen::Instance.GetSize();

	const auto _camera = SceneManager::Instance.GetActiveScene()->GetMainCamera()->GetGameObject()->GetComponent<ShakeCamera>();
	if (_camera == nullptr)
		return;

	if (m_Player->GetLookDirection() == -1)
	{
		// HP 바
		Screen::Instance.DrawBitmapScreenSpace(
			Resources::Instance.GetSpriteByName(
				TEXT("Assets/UI/PlayerHPBarUnderLeft.png"))->GetSpriteInfo(),
			Matrix3x2::MakeTranslateMatrix({ 191, 50 }));

		if (!_camera->GetClear())
		{
			if (m_ChangeSkillTab)
			{
				Screen::Instance.DrawBitmapScreenSpace(
					Resources::Instance.GetSpriteByName(
						TEXT("Assets/UI/SkillTabLeft.png"))->GetSpriteInfo(),
					Matrix3x2::MakeTranslateMatrix({ 180, _screenSize.y - 100 }));
			}
			else
			{

				Screen::Instance.DrawBitmapScreenSpace(
					Resources::Instance.GetSpriteByName(
						TEXT("Assets/UI/SkillTabGrayLeft.png"))->GetSpriteInfo(),
					Matrix3x2::MakeTranslateMatrix({ 180, _screenSize.y - 100 }));
			}
		}
	}
	else
	{
		// HP 바
		Screen::Instance.DrawBitmapScreenSpace(
			Resources::Instance.GetSpriteByName(
				TEXT("Assets/UI/PlayerHPBarUnderRight.png"))->GetSpriteInfo(),
			Matrix3x2::MakeTranslateMatrix({ 180, 50 }));

		if (!_camera->GetClear())
		{
			if (m_ChangeSkillTab)
			{
				Screen::Instance.DrawBitmapScreenSpace(
					Resources::Instance.GetSpriteByName(
						TEXT("Assets/UI/SkillTabRight.png"))->GetSpriteInfo(),
					Matrix3x2::MakeTranslateMatrix({ 180, _screenSize.y - 100 }));
			}
			else
			{

				Screen::Instance.DrawBitmapScreenSpace(
					Resources::Instance.GetSpriteByName(
						TEXT("Assets/UI/SkillTabGrayRight.png"))->GetSpriteInfo(),
					Matrix3x2::MakeTranslateMatrix({ 180, _screenSize.y - 100 }));
			}
		}
	}

	Screen::Instance.DrawBitmapFillScreenSpace(
		m_PlayerHealthRatio, Resources::Instance.GetSpriteByName(
			TEXT("Assets/UI/PlayerHPBar.png"))->GetSpriteInfo(),
		Matrix3x2::MakeTranslateMatrix({ 225, 65 }));

	if (!_camera->GetClear())
	{
		Screen::Instance.DrawBitmapFillScreenSpace(
			m_Skill4CoolTimeRatio, Resources::Instance.GetSpriteByName(
				TEXT("Assets/UI/Skill1CoolTimeBar.png"))->GetSpriteInfo(),
			Matrix3x2::MakeTranslateMatrix({ 249, _screenSize.y - 100 }), false, 0.85f);

		Screen::Instance.DrawBitmapFillScreenSpace(
			m_Skill1CoolTimeRatio, Resources::Instance.GetSpriteByName(
				TEXT("Assets/UI/Skill2CoolTimeBar.png"))->GetSpriteInfo(),
			Matrix3x2::MakeTranslateMatrix({ 249, _screenSize.y - 100 }), false, 0.85f);

		Screen::Instance.DrawBitmapFillScreenSpace(
			m_Skill3CoolTimeRatio, Resources::Instance.GetSpriteByName(
				TEXT("Assets/UI/Skill3CoolTimeBar.png"))->GetSpriteInfo(),
			Matrix3x2::MakeTranslateMatrix({ 249, _screenSize.y - 100 }), false, 0.85f);

		Screen::Instance.DrawBitmapFillScreenSpace(
			m_Skill2CoolTimeRatio, Resources::Instance.GetSpriteByName(
				TEXT("Assets/UI/Skill4CoolTimeBar.png"))->GetSpriteInfo(),
			Matrix3x2::MakeTranslateMatrix({ 249, _screenSize.y - 100 }), false, 0.85f);
	}

	if (m_Camera == nullptr)
		return;

	const auto _gameManger = m_Camera->GetGameObject()->GetComponent<ActionCommanderGameManager>();
	if (_gameManger == nullptr)
		return;

	Screen::Instance.DrawTextScreenSpace(
		StringHelper::Format(TEXT("LEVEL %d"), _gameManger->GetLevelIndex()),
		m_TextFormat, { _screenSize.x / 2, 35 }, Color::White
	);

	if (_gameManger->GetRemainsMonsterCount() > 0)
	{
		Screen::Instance.DrawTextScreenSpace(
			StringHelper::Format(TEXT("Monster Count\n%d"), _gameManger->GetRemainsMonsterCount()),
			m_TextFormat, { _screenSize.x - 220, 35 }, Color::White
		);
	}
	else
	{
		Screen::Instance.DrawTextScreenSpace(TEXT("STAGE CLEAR!\n GO TO RIGHT"),
			m_TextFormat, { _screenSize.x - 220, 35 }, Color::White
		);
	}
}
