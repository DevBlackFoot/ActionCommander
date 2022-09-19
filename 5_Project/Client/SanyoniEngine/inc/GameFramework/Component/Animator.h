#pragma once

#include "ComponentBase.h"
#include "AnimationClip.h"

namespace SanyoniEngine
{
	class GameObject;

	class Animator final :
		public ComponentBase
	{
	public:
		Animator(GameObject* _gameObject, const tstring& _typeName = TEXT("Animator"));

		void PreTick() override;

		void Tick() override;

		float GetFloat(const tstring& _typeName, const tstring& _propertyName, float _time) const;

		std::vector<AnimationEventInfo> GetEvents(float _rangeStartTime, float _rangeEndTime) const;

		float GetPositionX(float _time) const;

		float GetPositionY(float _time) const;

		float GetPositionZ(float _time) const;

		tstring GetSprite(float _time) const;

		bool HasFloat(const tstring& _typeName, const tstring& _propertyName) const;

	private:
		AnimationClip* m_CurrentClip;

		AnimationClip* m_ClipLastFrame;

		float m_ClipStartTime;

		float m_ClipTimeLastFrame;

		int m_ClipLoopCountLastFrame;

		bool m_bReachedEnd;

		Vector2 m_PositionLastFrame;

	public:
		AnimationClip* GetCurrentClip() const { return m_CurrentClip; }

		bool GetReachedEnd() const { return m_bReachedEnd; }

		void SetCurrentClip(AnimationClip* _clip);
	};
}
