#include "SanyoniEngine_pch.h"
#include "Core.h"

#include <wincodec.h>

#include "GameTime.h"
#include "Helper.h"
#include "Input.h"
#include "SanyoniEngine.h"

#include "Screen.h"
#include "SceneManager.h"
#include "CollisionManager.h"
#include "Resources.h"

using namespace SanyoniEngine;

IMPLEMENT_SINGLETON_CLASS(Core)

void Core::Initialize()
{
	m_hInstance = nullptr;
	m_hWnd = nullptr;
	m_Direct2dFactory = nullptr;
	m_RenderTarget = nullptr;
	m_WICImagingFactory = nullptr;
	m_DWriteFactory = nullptr;
	m_InFrameCounter = 0;
	m_bEnableDebug = false;
	m_bEnableDebugNoDrawBitmap = false;
}

void Core::Release()
{
	if (m_Direct2dFactory != nullptr)
		m_Direct2dFactory->Release();

	if (m_RenderTarget != nullptr)
		m_RenderTarget->Release();

	Screen::Instance.Release();
}

HRESULT Core::Initialize(int _nCmdShow)
{
	HRESULT _hr;

	RegisterWindowClass();

	_hr = InitializeWindow(_nCmdShow);

	if ((_hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &m_Direct2dFactory)) != S_OK)
	{
		assert(false);
		return _hr;
	}

	_hr = InitializeWICImagingFactory();

	_hr = InitializeDWriteFactory();

	// 최초의 PreTick 진입 전에 RenderTarget을 필요로 할 수도 있기 때문에
	// PreTick이 아닌 Initialize에서도 생성합니다.
	_hr = CreateDeviceResources();

	// 여러 싱글턴 매니저 클래스들을 초기화합니다.
	GameTime::Instance.Initialize();
	Input::Instance.Initialize(m_hWnd);
	Screen::Instance.Initialize();
	SceneManager::Instance.Initialize();

	_hr = ShowWindowToScreen(_nCmdShow) ? S_OK : E_FAIL;

	if (_hr != S_OK)
	{
		assert(false);
		return _hr;
	}

	return _hr;
}

void Core::Tick()
{
	Input::Instance.Tick();
	SceneManager::Instance.Tick();
	CollisionManager::Instance.CollisionTick();
}

void Core::PreTick()
{
	m_InFrameCounter = 0;

	if (Input::Instance.GetKeyPress(EVirtualKey::LeftControl))
	{
		if (Input::Instance.GetKeyState(EVirtualKey::F1) & EKeyStateBit::Bit_Down)
		{
			// ctrl + F1 키를 눌러 debug 시각화 기능 활성화 여부를 토글합니다.
			SetEnableDebug(!GetEnableDebug());
		}

		if (Input::Instance.GetKeyState(EVirtualKey::F2) & EKeyStateBit::Bit_Down)
		{
			// ctrl + F2 키를 눌러 bitmap 그리기 활성화 여부를 토글합니다.
			SetEnableDebugNoDrawBitmap(!GetEnableDebugNoDrawBitmap());
		}
	}


	HRESULT _hr;

	if ((_hr = CreateDeviceResources()) != S_OK)
	{
		assert(false);
		return;
	}

	GameTime::Instance.PreTick();
	Screen::Instance.PreRender();
}

void Core::PostTick()
{
	SceneManager::Instance.Render();
	Screen::Instance.PostRender();
}

HRESULT Core::CreateDeviceResources()
{
	HRESULT _hr = S_OK;

	if (m_RenderTarget != nullptr)
		return S_OK;

	RECT rc;
	if (GetClientRect(m_hWnd, &rc) == false)
	{
		assert(false);
		return _hr;
	}

	const D2D1_SIZE_U _size = D2D1::SizeU(
		rc.right - rc.left,
		rc.bottom - rc.top
	);

	// Direct2D RenderTarget을 생성합니다.
	if ((_hr = m_Direct2dFactory->CreateHwndRenderTarget(
			D2D1::RenderTargetProperties(),
			D2D1::HwndRenderTargetProperties(m_hWnd, _size),
			&m_RenderTarget)
	) != S_OK)
	{
		assert(false);
		return _hr;
	}

	return _hr;
}

void Core::ReleaseDeviceResources()
{
	m_RenderTarget->Release();
	m_RenderTarget = nullptr;
}

ATOM Core::RegisterWindowClass()
{
	// Register the window class.
	WNDCLASSEX wcex = {sizeof(WNDCLASSEX)};
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = Core::WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = sizeof(LONG_PTR);
	wcex.hInstance = HINST_THISCOMPONENT;
	wcex.hbrBackground = NULL;
	wcex.lpszMenuName = NULL;
	wcex.hCursor = LoadCursor(NULL, IDI_APPLICATION);
	wcex.lpszClassName = _TEXT("SimpleDX2DExample");

	const ATOM _result = RegisterClassEx(&wcex);
	return _result;
}

HRESULT Core::InitializeWindow(int nCmdShow)
{
	// Create the window.
	m_hWnd = CreateWindow(
		TEXT("SimpleDX2DExample"),
		TEXT("Action Commander"),
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		static_cast<UINT>(1280),
		static_cast<UINT>(720),
		NULL,
		NULL,
		HINST_THISCOMPONENT,
		this
	);

	if (m_hWnd == nullptr)
	{
		assert(false);
		return E_FAIL;
	}

	return S_OK;
}

bool Core::ShowWindowToScreen(int _nCmdShow)
{
	// ShowWindow 함수의 반환값은 함수의 성공 여부와 관계 없습니다.
	ShowWindow(m_hWnd, _nCmdShow);

	if (UpdateWindow(m_hWnd) == false)
		return false;

	return true;
}

HRESULT Core::InitializeWICImagingFactory()
{
	// CoInitialize(nullptr);

	HRESULT _hr;
	if ((_hr = CoCreateInstance(
		CLSID_WICImagingFactory,
		nullptr,
		CLSCTX_INPROC_SERVER,
		IID_IWICImagingFactory,
		reinterpret_cast<void**>(&m_WICImagingFactory)
	)) != S_OK)
	{
		assert(false);
		return _hr;
	}

	return _hr;
}

HRESULT Core::InitializeDWriteFactory()
{
	HRESULT _hr = DWriteCreateFactory(
		DWRITE_FACTORY_TYPE_SHARED,
		__uuidof(m_DWriteFactory),
		reinterpret_cast<IUnknown**>(&m_DWriteFactory)
	);

	if (_hr != S_OK)
	{
		assert(false);
		return _hr;
	}

	return _hr;
}

LRESULT Core::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (message == WM_CREATE)
	{
		LPCREATESTRUCT pcs = (LPCREATESTRUCT)lParam;
		Core* _core = (Core*)pcs->lpCreateParams;

		::SetWindowLongPtr(
			hWnd,
			GWLP_USERDATA,
			reinterpret_cast<LONG_PTR>(_core)
		);

		return 0;
	}

	Core* _core = reinterpret_cast<Core*>(static_cast<LONG_PTR>(
		::GetWindowLongPtr(
			hWnd,
			GWLP_USERDATA
		)));

	if (_core)
	{
		if (HandleMessage(_core, hWnd, message, wParam, lParam) == true)
			return 0;

		switch (message)
		{
		case WM_DISPLAYCHANGE:
			{
				InvalidateRect(hWnd, nullptr, false);
			}
			return 0;

		case WM_PAINT:
			{
				PAINTSTRUCT _ps;
				HDC _hDC = BeginPaint(_core->m_hWnd, &_ps);
				EndPaint(_core->m_hWnd, &_ps);
				ValidateRect(hWnd, nullptr);
			}
			return 0;

		case WM_DESTROY:
			{
				PostQuitMessage(0);
			}
			return 0;
		}
	}

	return DefWindowProc(hWnd, message, wParam, lParam);
}

bool Core::HandleMessage(Core* _core, HWND _hWnd, UINT _message, WPARAM _wParam, LPARAM _lParam)
{
	switch (_message)
	{
	case WM_SIZE:
		{
			UINT width = LOWORD(_lParam);
			UINT height = HIWORD(_lParam);
			Screen::Instance.OnResize(width, height);
		}
		return true;

	case WM_KEYDOWN:
	case WM_KEYUP:
		{
			Input::Instance.PushKeyMessage({
				static_cast<EVirtualKey>(_wParam),
				_message == WM_KEYDOWN ? true : false,
				std::chrono::system_clock::now()
			});
		}
		return true;

	case WM_MOUSEWHEEL:
		{
			const int _delta = GET_WHEEL_DELTA_WPARAM(_wParam);
			const POINTS _pt = MAKEPOINTS(_lParam);
			Input::Instance.PushMouseWheelMessage({_delta, Vector2(_pt.x, _pt.y)});
		}
		break;
	}

	return false;
}
