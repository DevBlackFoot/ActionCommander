#pragma once

#include "SanyoniEngine_types.h"
#include "SingletonInstanceBase.h"

namespace SanyoniEngine
{
	class GameTime;
	class Input;
	class Screen;
	class Resources;
	class SceneManager;
	class CollisionManager;

	class Core final
	{
		DECLARE_SINGLETON_CLASS(Core)

	public:
		void Initialize();

		void Release();

		HRESULT Initialize(int _nCmdShow);

		void Tick();

		void PreTick();

		void PostTick();

		HRESULT CreateDeviceResources();

		void ReleaseDeviceResources();

	private:
		/// <summary>
		/// 윈도우 클래스를 등록합니다.
		/// </summary>
		ATOM RegisterWindowClass();

		/// <summary>
		/// 프로그램에서 사용할 메인 윈도우를 생성합니다.
		/// </summary>
		HRESULT InitializeWindow(int nCmdShow);

		/// <summary>
		/// 화면에 메인 윈도우를 표시합니다.
		/// </summary>
		bool ShowWindowToScreen(int _nCmdShow);

		HRESULT InitializeWICImagingFactory();

		HRESULT InitializeDWriteFactory();

	private:
		/// <summary>
		/// 프로세스 인스턴스 핸들입니다.
		/// </summary>
		HINSTANCE m_hInstance;

		/// <summary>
		/// 메인 윈도우의 핸들입니다.
		/// </summary>
		HWND m_hWnd;

		/// <summary>
		/// Direct2D 리소스를 생성할 때 사용하는 D2D1Factory 인스턴스입니다.
		/// </summary>
		ID2D1Factory* m_Direct2dFactory;

		/// <summary>
		/// 드로잉 작업을 수행하고 브러쉬같은 기기 종속적인 드로잉 리소스를 생성할 때 사용하는 RenderTarget 인스턴스입니다.
		/// </summary>
		ID2D1HwndRenderTarget* m_RenderTarget;

		IWICImagingFactory* m_WICImagingFactory;

		IDWriteFactory7* m_DWriteFactory;

		int m_InFrameCounter;

		bool m_bEnableDebug;

		bool m_bEnableDebugNoDrawBitmap;

	public:
		inline static HWND GetHWnd() { return Instance.m_hWnd; }

		inline static ID2D1HwndRenderTarget* GetRenderTarget() { return Instance.m_RenderTarget; }

		inline static IWICImagingFactory* GetWICImagingFactory() { return Instance.m_WICImagingFactory; }

		inline static auto GetDWriteFactory() { return Instance.m_DWriteFactory; }

		inline static int& GetInFrameCounterRef() { return Instance.m_InFrameCounter; }

		inline static bool GetEnableDebug() { return Instance.m_bEnableDebug; }

		inline static bool GetEnableDebugNoDrawBitmap() { return Instance.m_bEnableDebugNoDrawBitmap; }

		inline static void SetEnableDebug(bool _enable) { Instance.m_bEnableDebug = _enable; }

		inline static void SetEnableDebugNoDrawBitmap(bool _enable) { Instance.m_bEnableDebugNoDrawBitmap = _enable; }

	private:
		/// <summary>
		/// 윈도우 메세지를 처리하는 윈도우 콜백 함수입니다.
		/// </summary>
		static LRESULT CALLBACK WndProc(
			HWND hWnd,
			UINT message,
			WPARAM wParam,
			LPARAM lParam
		);

		static bool HandleMessage(
			Core* _core,
			HWND _hWnd,
			UINT _message,
			WPARAM _wParam,
			LPARAM _lParam
		);
	};
}
