
// ActionCommanderMapTool.h: ActionCommanderMapTool 애플리케이션의 기본 헤더 파일
//
#pragma once

#ifndef __AFXWIN_H__
	#error "PCH에 대해 이 파일을 포함하기 전에 'pch.h'를 포함합니다."
#endif

#include "resource.h"       // 주 기호입니다.


// CActionCommanderMapToolApp:
// 이 클래스의 구현에 대해서는 ActionCommanderMapTool.cpp을(를) 참조하세요.
//

class CActionCommanderMapToolApp : public CWinApp
{
public:
	CActionCommanderMapToolApp() noexcept;


// 재정의입니다.
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// 구현입니다.
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()

// 내가 사용할 멤버 변수
public:
	// 현재의 마우스 커서 상태를 나타내는 변수
	// 이를 바탕으로 타일맵에 속성을 부여한다.
	TileAttribute m_CursorState;
	ObjectAttribute m_ObjCursorState;
	bool isTileCursor = true;

	// 전역적인 맵 사이즈를 받는 변수를 만든다.
		// 이를 바탕으로 기본적인 사이즈를 정한다.
		// 그리드의 개수를 받고 => 그리드의 개수 * 타일 크기로 전체 맵 사이즈를 정한다.
	int m_MapWidth;		// x 그리드의 개수
	int m_MapHeight;		// y 그리드의 개수
};

extern CActionCommanderMapToolApp theApp;
