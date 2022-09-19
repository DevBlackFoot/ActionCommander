#include "pch.h"
#include <Windows.h>

#include "Application.h"
#include "GameApplication.h"
#include "SanyoniTestApplication.h"
#include "JHTestApplication.h"

using namespace SanyoniEngine;

/// <summary>
/// 메인 윈도우의 메세지를 처리하는 콜백 메소드입니다.
/// </summary>
LRESULT CALLBACK WndProc(HWND _hWnd, UINT _message, WPARAM _wParam, LPARAM _lParam);

int APIENTRY _tWinMain(_In_ HINSTANCE _hInstance,
					   _In_opt_ HINSTANCE _hPrevInstance,
					   _In_ LPTSTR _lpCmdLine,
					   _In_ int _nCmdShow)
{
	UNREFERENCED_PARAMETER(_hPrevInstance);
	UNREFERENCED_PARAMETER(_lpCmdLine);

	// HeapSetInformation을 사용하여
	// heap manager가 프로세스에서 사용되는 힙에서 오류를 감지하는 경우,
	// 프로세스를 종료하도록 명시합니다.
	// 정말 드문 경우에 HeapSetInformation이 실패하더라도 계속 실행되기를 원하기 때문에
	// 반환 값을 무시합니다.
	HeapSetInformation(nullptr, HeapEnableTerminationOnCorruption, nullptr, 0);

	if (SUCCEEDED(CoInitialize(nullptr)) == false)
		return 1;

	// 실제 인스턴스를 생성하는 클래스를
	// 사용할 Application 클래스로 교체하세요.
	Application* app

#ifdef _TEST_SANYONI
		= new SanyoniTestApplication();
#elif _TEST_JH
		= new JHTestApplication();
#else
		= new JHTestApplication();
#endif

	if (SUCCEEDED(app->Initialize(_nCmdShow)) == false)
		return 1;

	app->RunMessageLoop();

	delete app;

	CoUninitialize();
	return 0;
}
