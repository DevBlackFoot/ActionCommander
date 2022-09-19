
// MainFrm.h: CMainFrame 클래스의 인터페이스
//

#pragma once

// 메인 프레임에서 사용할 view들
#include "ActionCommanderMapToolView.h"
#include "MiniMapView.h"
#include "ControlView.h"

class CMainFrame : public CFrameWnd
{
	
protected: // serialization에서만 만들어집니다.
	CMainFrame() noexcept;
	DECLARE_DYNCREATE(CMainFrame)

// 특성입니다.
public:

// 작업입니다.
public:
	// 뷰를 나눕니다.
	CSplitterWnd	m_MainSplitter;		// 메인으로 나눠줄 스플릿 윈도우
	CSplitterWnd	m_SubSplitter;		// 서브로 나눠줄 스플릿 윈되우

	CActionCommanderMapToolView*	m_pMainView;		// 메인뷰로 사용할 액션 커맨더 맵뷰
	CMiniMapView*					m_pMinimapView;		// 서브뷰 1로 사용할 미니맵 뷰
	CControlView*					m_pControlView;		// 서브뷰 2로 사용할 컨트롤 뷰


// 재정의입니다.
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// 구현입니다.
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // 컨트롤 모음이 포함된 멤버입니다.
	CToolBar          m_wndToolBar;
	CStatusBar        m_wndStatusBar;

// 생성된 메시지 맵 함수
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	DECLARE_MESSAGE_MAP()

	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
};


