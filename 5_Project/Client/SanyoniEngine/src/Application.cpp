#include "SanyoniEngine_pch.h"
#include "Application.h"

#include "Core.h"

using namespace SanyoniEngine;

Application::Application()
{
}

Application::~Application()
{
	Core::Instance.Release();
}

HRESULT Application::Initialize(int _nCmdShow)
{
	HRESULT _hr;

	if (SUCCEEDED(_hr = Core::Instance.Initialize(_nCmdShow)) == false)
		return _hr;

	return S_OK;
}

void Application::RunMessageLoop()
{
	MSG _msg;

	OnStart();

	while (true)
	{
		if (PeekMessage(&_msg, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&_msg);
			DispatchMessage(&_msg);

			if (_msg.message == WM_QUIT)
				break;
		}
		else
		{
			Core::Instance.PreTick();
			OnUpdate();
			Core::Instance.Tick();
			Core::Instance.PostTick();
		}
	}
}

void Application::OnStart()
{
}

void Application::OnUpdate()
{
}
