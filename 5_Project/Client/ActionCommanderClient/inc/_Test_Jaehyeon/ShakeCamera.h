#pragma once

#include "ComponentBase.h"
#include <random>

namespace SanyoniEngine
{
	class GameObject;
}

class Player;

/**
 * \brief 기본적으로 테스트를 위해 생성된 클래스입니다.
 *
 * 이후 정리해서 내용은 FollowPlayer에 넣어서 특정 상황에 카메라를 흔들리게 할 수 있으면 될 것 같습니다.
 *
 * 기본적으로 세로축, 가로축 쉐이킹이 있고 (최저한도 ~ 최대한도까지 쉐이크), 지속시간이 주어집니다.
 *
 * 진폭(최대로 흔들리는 범위)과 진동수(1초에 몇 번 흔들리는가 => 진동 세기)로 매개변수를 받아서 흔들 수 있으면 될 것 같다.
 */
class ShakeCamera:
	public SanyoniEngine::ComponentBase
{
	// 기본적으로는 Follow플레이어를 따라간다.
public:
	ShakeCamera(SanyoniEngine::GameObject* _gameObject, tstring _typename = TEXT("ShakeCamera"));

	void Tick() override;

private:
	Player* m_Player;
	// distribution의 범위를 결정하면 흔들림의 세기를 정할 수 있다.
	std::uniform_real_distribution<float> m_XAxisDistribution;
	std::uniform_real_distribution<float> m_YAxisDistribution;

	// 각 셰이킹의 지속시간.
	long long m_ShakingDurationMillis;
	// 마지막 셰이킹의 시작 시간
	long long m_LastShakingStartTimeMillis;

	// 지금 흔들리고 있는가 여부 => 안 흔들리면 평상시 카메라
	bool m_bShake;

	// 클리어 하였는지에 대한 여부..
	bool m_bClear;

	// 맵 사이즈를 파악할 Vector2
	Vector2 m_MapSize;

public:
	void SetPlayer(Player* _player) { m_Player = _player; }

	// 새로운 진동 분포 + 진동 시작을 체크하는 함수 + 진동 시간을 설정한다.
	void StartShake(float _xIntensity, float _yIntensity, long long _shakeDuration);

	void SetMapSize(Vector2 _mapSize) { m_MapSize = _mapSize; }

	bool GetClear() { return m_bClear; }
};

