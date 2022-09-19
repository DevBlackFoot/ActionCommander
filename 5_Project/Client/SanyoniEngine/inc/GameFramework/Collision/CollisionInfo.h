#pragma once

namespace SanyoniEngine
{
	class ColliderBase;

	/**
	 * \brief 어떤 오브젝트간의 충돌 상태를 표현하는 열거형입니다. <br>
	 * 가장 하위 비트는 현재 프레임의 충돌 상태를 나타내는 비트입니다. <br>
	 * 밑에서 두 번째 비트는 지난 프레임의 충돌 상태를 나타내는 비트입니다.
	 */
	enum class ECollisionState
	{
		None = 0b000,
		Enter = 0b001,
		Stay = 0b011,
		Exit = 0b010
	};

	inline bool operator&(ECollisionState _lhs, byte _rhs)
	{
		return static_cast<int>(_lhs) & _rhs;
	}
}
