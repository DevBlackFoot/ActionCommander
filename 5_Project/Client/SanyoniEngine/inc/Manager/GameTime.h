#pragma once

#include <chrono>

#include "SingletonInstanceBase.h"

namespace SanyoniEngine
{
	class GameTime final
	{
		DECLARE_SINGLETON_CLASS(GameTime)

	public:
		void Initialize();

		void PreTick();

	private:
		std::chrono::system_clock::time_point m_StartGameTime;

		std::chrono::system_clock::time_point m_LastPreTickTime;

		std::chrono::system_clock::time_point m_CurrentPreTickTime;

	private:
		/**
		 * \brief 게임이 시작된 이후 경과된 시간입니다.
		 */
		long long m_GameTimeMillis;

		/**
		 * \brief 지난 프레임을 처리하면서 걸린 시간입니다.
		 * 이는 지난 프레임의 시작 시점과 이번 프레임의 시작 시점의 시간 차이입니다.
		 */
		long long m_DeltaTimeMillis;

	public:
		inline long long GetGameTimeMillis() const { return m_GameTimeMillis; }

		inline long long GetDeltaTimeMillis() const { return m_DeltaTimeMillis; }

		inline float GetGameTime() const { return static_cast<float>(m_GameTimeMillis) * MILLIS_TO_SECOND; }

		inline float GetDeltaTime() const { return static_cast<float>(m_DeltaTimeMillis) * MILLIS_TO_SECOND; }

		inline auto GetGameTimePoint() const { return m_CurrentPreTickTime; }

	public:
		static constexpr long long SECOND_TO_MILLIS = 1'000;
		static constexpr long long SECOND_TO_MICROS = 1'000'000;
		static constexpr long long SECOND_TO_NANOS = 1'000'000'000;
		static constexpr double MILLIS_TO_SECOND = 1. / 1'000.;
		static constexpr long long MILLIS_TO_MICROS = 1'000;
		static constexpr long long MILLIS_TO_NANOS = 1'000'000;
		static constexpr double MICROS_TO_SECOND = 1. / 1'000'000.;
		static constexpr double MICROS_TO_MILLIS = 1. / 1'000.;
		static constexpr long long MICROS_TO_NANOS = 1'000;
		static constexpr double NANOS_TO_SECOND = 1. / 1'000'000'000.;
		static constexpr double NANOS_TO_MILLIS = 1. / 1'000'000.;
		static constexpr double NANOS_TO_MICROS = 1. / 1'000.;
	};
}
