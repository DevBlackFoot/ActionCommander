#pragma once

#include "ComponentBase.h"

namespace SanyoniEngine
{
	class GameObject;
}

class Player;

class FollowPlayer :
	public SanyoniEngine::ComponentBase
{
public:
	FollowPlayer(SanyoniEngine::GameObject* _gameObject, tstring _typeName = TEXT("FollowPlayer"));

	void Tick() override;

private:
	Player* m_Player;

public:
	void SetPlayer(Player* _player) { m_Player = _player; }
};
