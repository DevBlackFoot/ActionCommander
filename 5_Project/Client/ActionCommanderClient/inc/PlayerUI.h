#pragma once

#include "SanyoniEngine_types.h"
#include "ComponentBase.h"
#include "ComponentInterfaces.h"

namespace SanyoniEngine
{
	class GameObject;
	class Camera;
	class ColliderBase;
	class TextFormat;
}

class Player;
// �÷��̾��� ���¸� ȭ�鿡 �����մϴ�.
class PlayerUI :
	public SanyoniEngine::ComponentBase,
	public SanyoniEngine::IRenderableComponent
{
public:
	PlayerUI(SanyoniEngine::GameObject* _gameObject, tstring _typeName = TEXT("PlayerUI"));

	void OnStart() override;

	void PreTick() override;

	void Tick() override;

	void PostTick() override;

	void OnRender() override;

	void SetPlayer(Player* _player) { m_Player = _player; }

	void SetCamera(Camera* _camera) { m_Camera = _camera; }

private:
	// Scene�� �����ϴ� Player�� ã�ƿ´�..? => Set Player�� ���ؼ� �����´�.
	Player* m_Player;
	Camera* m_Camera;

	int m_NowPlayerHealth;

	long long m_LastEndSkill1TimeMillis;
	long long m_LastEndSkill2TimeMillis;
	long long m_LastEndSkill3TimeMillis;
	long long m_LastEndSkill4TimeMillis;

	float m_PlayerHealthRatio;
	float m_Skill1CoolTimeRatio;
	float m_Skill2CoolTimeRatio;
	float m_Skill3CoolTimeRatio;
	float m_Skill4CoolTimeRatio;

	bool m_ChangeSkillTab;

	TextFormat* m_TextFormat;
};

