// ControlView.cpp: 구현 파일
//

#include "pch.h"
#include "ActionCommanderMapTool.h"
#include "ControlView.h"
#include "CControlProperty.h"

// CControlView

IMPLEMENT_DYNCREATE(CControlView, CFormView)

CControlView::CControlView()
	: CFormView(IDD_CControlView)
{

}

CControlView::~CControlView()
{
}

void CControlView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CControlView, CFormView)
	ON_WM_DESTROY()
END_MESSAGE_MAP()


// CControlView 진단

#ifdef _DEBUG
void CControlView::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CControlView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CControlView 메시지 처리기


void CControlView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	m_pControlProperty = new CControlProperty(this);

	m_pControlProperty->Create(this, WS_CHILD | WS_VISIBLE);

	m_pControlProperty->MoveWindow(0, 0, 300, 600, FALSE);
	m_pControlProperty->SetActivePage(0);

}


void CControlView::OnDestroy()
{
	CFormView::OnDestroy();

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	delete m_pControlProperty;
	m_pControlProperty = nullptr;
}
