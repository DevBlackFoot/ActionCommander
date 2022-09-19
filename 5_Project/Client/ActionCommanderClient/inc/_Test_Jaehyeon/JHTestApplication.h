#pragma once

#include "Application.h"

namespace SanyoniEngine
{
	class Scene;
	class GameObject;
	class TextFormat;
	class SpriteAnimationClip;
}

class CommandQueue;

enum class CommandState
{
	Idle,
	Run,
	Jump,
	Dash,
};

class JHTestApplication : public SanyoniEngine::Application
{
protected:
	void OnStart() override;

	void OnUpdate() override;

	// 예비 몬스터
	GameObject* m_DummyMonster;

	TextFormat* m_TextFormat;

	void InitBitmapResource();

	long long m_PrevTimeMillis;
};

