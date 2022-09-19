#pragma once
#include "ComponentBase.h"

namespace SanyoniEngine
{
	class GameObject;

	class SpriteFlicker :
		public ComponentBase
	{
	public:
		SpriteFlicker(GameObject* _gameObject, tstring _typeName = TEXT("SpriteFlicker"));

		void OnChangeEnable(bool _newEnable) override;

		void Tick() override;

		void Start(long long _duration);

	private:
		long long m_IntervalTimeMillis;

		std::optional<long long> m_DurationMillis;

		long long m_StartFlickerTimeMillis;

		long long m_LastFlickerTimeMillis;

	public:
		void SetDurationMillis(long long _duration) { m_DurationMillis = _duration; }
	};
}
