#pragma once

#include "SanyoniEngine_pch.h"
#include "EVirtualKey.h"

namespace SanyoniEngine
{
	struct KeyMessageInfo
	{
		/// <summary>
		/// 눌린 virtual key의 식별값입니다.
		/// </summary>
		EVirtualKey vKey;

		/// <summary>
		/// 발생한 메세지가 WM_KEYDOWN이라면 true이고, WM_KEYUP이라면 false입니다.
		/// </summary>
		bool isDown;

		/// <summary>
		/// WinProc에 메세지가 수신된 time point입니다.
		/// </summary>
		std::chrono::system_clock::time_point timePoint;
	};
}
