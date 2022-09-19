#pragma once
#include "ComponentBase.h"

namespace SanyoniEngine
{
	class AnimationClip;
}

class GameOverSceneTransition :
	public ComponentBase
{
public:
	GameOverSceneTransition(GameObject* _gameObject, const tstring& _typeName = TEXT("GameOverSceneTransition"));

	void OnStart() override;

	void Tick() override;

private:
	AnimationClip* m_Clip;

	long long m_DeadTimeMillis;
};
