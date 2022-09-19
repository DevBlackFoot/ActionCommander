#pragma once

#include "ComponentBase.h"
#include "ComponentInterfaces.h"

namespace SanyoniEngine
{
	class GameObject;
	class ISprite;

	class SpriteRenderer final :
		public ComponentBase,
		public IRenderableComponent
	{
	public:
		SpriteRenderer(GameObject* _gameObject, tstring _typeName = TEXT("SpriteRenderer"));

		void OnRender() override;

	private:
		ISprite* m_Sprite;

		bool m_Flip;

		float m_Opacity;

		bool m_bEnableFillMode;

		float m_FillAmount;

	public:
		inline ISprite* GetSprite() const { return m_Sprite; }

		inline bool GetFlip() const { return m_Flip; }

		inline float GetOpacity() const { return m_Opacity; }

		inline bool GetEnableFillMode() const { return m_bEnableFillMode; }

		inline float GetFillAmount() const { return m_FillAmount; }

		inline void SetSprite(ISprite* _sprite) { m_Sprite = _sprite; }

		inline void SetOpacity(float _opacity) { m_Opacity = _opacity; }

		inline void SetFlip(bool _flip) { m_Flip = _flip; }

		void SetEnableFillMode(bool m_b_enable_fill_mode) { m_bEnableFillMode = m_b_enable_fill_mode; }

		void SetFillAmount(float m_fill_amount) { m_FillAmount = m_fill_amount; }
	};
}
