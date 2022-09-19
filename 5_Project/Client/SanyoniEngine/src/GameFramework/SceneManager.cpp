#include "SanyoniEngine_pch.h"
#include "SceneManager.h"

#include "Scene.h"

using namespace SanyoniEngine;

IMPLEMENT_SINGLETON_CLASS(SceneManager)

void SceneManager::Initialize()
{
	m_ActiveScene = nullptr;
}

void SceneManager::ReserveLoadScene(Scene* _scene)
{
	m_LoadSceneReserved = _scene;
}

void SceneManager::Tick()
{
	if (m_ActiveScene != m_LoadSceneReserved)
		LoadScene(m_LoadSceneReserved);

	if (m_ActiveScene == nullptr)
		return;

	m_ActiveScene->Tick();
}

void SceneManager::Render()
{
	if (m_ActiveScene == nullptr)
		return;

	m_ActiveScene->Render();
}

void SceneManager::LoadScene(Scene* _scene)
{
	// 만약 실행중인 씬이 있다면, 씬을 종료 콜백을 호출합니다.
	if (m_ActiveScene != nullptr)
	{
		m_ActiveScene->OnEndScene();
	}

	m_ActiveScene = _scene;

	// 씬 진입 콜백을 실행합니다.
	if (_scene != nullptr)
	{
		m_ActiveScene->OnStartScene();
	}
}
