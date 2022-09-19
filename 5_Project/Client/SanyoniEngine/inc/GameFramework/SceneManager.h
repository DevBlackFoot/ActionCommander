#pragma once

#include "SanyoniEngine_types.h"
#include "SingletonInstanceBase.h"

namespace SanyoniEngine
{
	class Scene;

	class SceneManager final
	{
		DECLARE_SINGLETON_CLASS(SceneManager)

	public:
		void Initialize();

		void ReserveLoadScene(Scene* _scene);

		void Tick();

		void Render();

	private:
		void LoadScene(Scene* _scene);

	private:
		Scene* m_ActiveScene;

		Scene* m_LoadSceneReserved;

	public:
		inline Scene* GetActiveScene() const { return m_ActiveScene; }
	};
}
