#pragma once
#include "ComponentBase.h"

namespace SanyoniEngine
{
	class AnimationClip;
}

class TitleSceneTransition :
	public ComponentBase
{
public:
	TitleSceneTransition(GameObject* _gameObject, const tstring& _typeName = TEXT("TitleSceneTransition"));

	void OnStart() override;

	void Tick() override;

private:
	AnimationClip* m_TitleAnimClip{};

	bool m_bWaitingForDelay{};

	long long m_ClipEndTimeMillis{};
};
