#pragma once

#include "SanyoniEngine.h"

namespace SanyoniEngine
{
	class Core;

	class Application
	{
	public:
		Application();

		virtual ~Application();

		HRESULT Initialize(int _nCmdShow);

		/// <summary>
		/// 메인 메세지 루프 함수입니다.
		///	메세지를 번역하고 그 메세지를 윈도우 프로시저에게 dispatch합니다.
		/// </summary>
		void RunMessageLoop();

	protected:
		virtual void OnStart();

		virtual void OnUpdate();
	};
}
