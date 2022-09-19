// MiniMapView.cpp: 구현 파일
//

#include "pch.h"
#include "ActionCommanderMapTool.h"
#include "MiniMapView.h"
#include "ActionCommanderMapToolDoc.h"
#include "MainFrm.h"

std::string WStringToString(const wchar_t* _string);
// CMiniMapView

IMPLEMENT_DYNCREATE(CMiniMapView, CView)

CMiniMapView::CMiniMapView()
{

}

CMiniMapView::~CMiniMapView()
{
}

BEGIN_MESSAGE_MAP(CMiniMapView, CView)
	ON_WM_CREATE()
	ON_REGISTERED_MESSAGE(AFX_WM_DRAW2D, &CMiniMapView::OnDraw2d)
END_MESSAGE_MAP()


// CMiniMapView 그리기

void CMiniMapView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: 여기에 그리기 코드를 추가합니다.
}


// CMiniMapView 진단

#ifdef _DEBUG
void CMiniMapView::AssertValid() const
{
	CView::AssertValid();
}

#ifndef _WIN32_WCE
void CMiniMapView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif
#endif //_DEBUG


// CMiniMapView 메시지 처리기

int CMiniMapView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	BOOL bUseDCRenderTarget = TRUE;
	EnableD2DSupport(TRUE, bUseDCRenderTarget);

	if (!IsD2DSupportEnabled())
		return -1;

	// TODO:  여기에 특수화된 작성 코드를 추가합니다.
	m_pBlackPen = new CD2DSolidColorBrush(GetDCRenderTarget(), D2D1::ColorF(D2D1::ColorF::Black));

	// 텍스트 표시를 위한 텍스트 포맷
	m_pTextFormat = new CD2DTextFormat(GetDCRenderTarget(), _T("고딕"), 15);

	m_pTextFormat->Get()->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING);
	m_pTextFormat->Get()->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);

	return 0;
}

afx_msg LRESULT CMiniMapView::OnDraw2d(WPARAM wParam, LPARAM lParam)
{
	m_pDCRenderTarget = (CDCRenderTarget*)lParam;
	ASSERT_VALID(m_pDCRenderTarget);


	CRect rc;
	GetClientRect(rc);

	m_pDCRenderTarget->Clear(D2D1::ColorF(D2D1::ColorF::White));

	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CActionCommanderMapToolDoc* pDoc = (CActionCommanderMapToolDoc*)(pFrame->GetActiveDocument());

	DrawTextJH(10, 10, "ObjectCount : %d", pDoc->m_ObjectCount);
	DrawTextJH(10, 30, "MonsterCount : %d", pDoc->m_MonsterCount);
	DrawTextJH(10, 50, "PlayerCount : %d", pDoc->m_PlayerCount);
	DrawTextJH(10, 70, "PortalCount : %d", pDoc->m_PortalCount);

	return TRUE;
}

// 엔진에서 떼온 format text
void CMiniMapView::DrawTextJH(int x, int y, const char* pch, ...)
{

	// 포맷 스트링 관련 버퍼.. 가변인자 처리
	char* buffer;
	int len;

	va_list ap;			// 현재 읽고 있는 번지
	va_start(ap, pch);	// ap 포인터 변수가 첫번재 가변인수를 가리키도록.

	len = _vscprintf(pch, ap) + 1;	// null문자 포함한 문자열의 길이
	buffer = new char[len];			// 동적 char 배열 생성

	vsprintf_s(buffer, len, pch, ap);
	va_end(ap);

	int nLen = MultiByteToWideChar(CP_ACP, 0, buffer, lstrlenA(buffer), NULL, NULL);

	BSTR unicodestr = SysAllocStringLen(NULL, nLen);

	::MultiByteToWideChar(CP_ACP, 0, buffer, strlen(buffer), unicodestr, nLen);

	if (unicodestr != NULL)
	{
		// 폰트별 차지하는 영역 계산..
		m_pRenderTarget->DrawTextA(WStringToString(unicodestr).c_str(), {(float)x, (float)y, (float)(x + 500), (float)(y + 40)}, 
			m_pBlackPen, m_pTextFormat);
	}

	// 다 했으면 메모리 해제
	::SysFreeString(unicodestr);

	delete[] buffer;
}

std::string WStringToString(const wchar_t* _string)
{
	const int _stringLength = static_cast<int>(std::wcslen(_string));
	const int _bufferLength = _stringLength * 2 + 1;
	char* _buffer = new char[_bufferLength];

	// WideCharToMultiByte 함수가 문자열의 끝에 자동으로 null 문자 ('\0')을 넣어주지 않습니다.
	// 따라서 문자열을 변환을 마친 후 그 뒤에다 수동으로 null문자를 넣어주어야 합니다.
	int _end = WideCharToMultiByte(CP_UTF8, 0, _string, _stringLength,
		_buffer, _bufferLength,
		nullptr, nullptr);
	_buffer[_end] = '\0';

	auto _retVal = std::string{ _buffer };

	delete[] _buffer;
	return _retVal;
}