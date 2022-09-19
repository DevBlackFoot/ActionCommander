#pragma once

#include "ComponentBase.h"
#include <random>

namespace SanyoniEngine
{
	class GameObject;
}

class Player;

/**
 * \brief �⺻������ �׽�Ʈ�� ���� ������ Ŭ�����Դϴ�.
 *
 * ���� �����ؼ� ������ FollowPlayer�� �־ Ư�� ��Ȳ�� ī�޶� ��鸮�� �� �� ������ �� �� �����ϴ�.
 *
 * �⺻������ ������, ������ ����ŷ�� �ְ� (�����ѵ� ~ �ִ��ѵ����� ����ũ), ���ӽð��� �־����ϴ�.
 *
 * ����(�ִ�� ��鸮�� ����)�� ������(1�ʿ� �� �� ��鸮�°� => ���� ����)�� �Ű������� �޾Ƽ� ��� �� ������ �� �� ����.
 */
class ShakeCamera:
	public SanyoniEngine::ComponentBase
{
	// �⺻�����δ� Follow�÷��̾ ���󰣴�.
public:
	ShakeCamera(SanyoniEngine::GameObject* _gameObject, tstring _typename = TEXT("ShakeCamera"));

	void Tick() override;

private:
	Player* m_Player;
	// distribution�� ������ �����ϸ� ��鸲�� ���⸦ ���� �� �ִ�.
	std::uniform_real_distribution<float> m_XAxisDistribution;
	std::uniform_real_distribution<float> m_YAxisDistribution;

	// �� ����ŷ�� ���ӽð�.
	long long m_ShakingDurationMillis;
	// ������ ����ŷ�� ���� �ð�
	long long m_LastShakingStartTimeMillis;

	// ���� ��鸮�� �ִ°� ���� => �� ��鸮�� ���� ī�޶�
	bool m_bShake;

	// Ŭ���� �Ͽ������� ���� ����..
	bool m_bClear;

	// �� ����� �ľ��� Vector2
	Vector2 m_MapSize;

public:
	void SetPlayer(Player* _player) { m_Player = _player; }

	// ���ο� ���� ���� + ���� ������ üũ�ϴ� �Լ� + ���� �ð��� �����Ѵ�.
	void StartShake(float _xIntensity, float _yIntensity, long long _shakeDuration);

	void SetMapSize(Vector2 _mapSize) { m_MapSize = _mapSize; }

	bool GetClear() { return m_bClear; }
};

