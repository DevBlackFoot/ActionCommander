#pragma once

#include "Application.h"

namespace SanyoniEngine
{
	class Scene;
	class GameObject;
	class TextFormat;
	struct AnimVarCollection;
}

class SanyoniTestApplication : public SanyoniEngine::Application
{
protected:
	void OnStart() override;

	void OnUpdate() override;

	Scene* m_Scene;
	GameObject* m_Circle;
	GameObject* m_Box1;
	GameObject* m_Box2;
	GameObject* m_SpriteObject;
	SpriteSheet* m_SpriteSheet;

	GameObject* m_FollowMouse;
	std::vector<GameObject*> m_Colliders;

	GameObject* m_Camera;

	GameObject* m_Player;
	GameObject* m_Platform;

	TextFormat* m_TextFormat;

	GameObject* m_OneShotEffect;
};
