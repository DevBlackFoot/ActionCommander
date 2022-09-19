#pragma once

#include "ComponentBase.h"
#include "ComponentInterfaces.h"
#include "SanyoniEngine_types.h"

namespace SanyoniEngine
{
	class GameObject;

	class Camera final :
		public ComponentBase,
		public IRenderableComponent
	{
	public:
		Camera(GameObject* _gameObject, tstring _typeName = TEXT("Camera"));

		Matrix3x2 GetCameraWorldTM() const;

		void Update();

		void OnRender() override;

	private:
		bool m_bDebug;

		Vector2 m_LeftTopWorldPosition;

		Vector2 m_RightBottomWorldPosition;

	public:
		inline void SetDebug(bool _bDebug) { m_bDebug = _bDebug; }

		inline const Vector2& GetLeftTopWorldPosition()
		{
			Update();
			return m_LeftTopWorldPosition;
		}

		inline const Vector2& GetRightBottomWorldPosition()
		{
			Update();
			return m_RightBottomWorldPosition;
		}
	};
}
