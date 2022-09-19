// CControlProperty.cpp: 구현 파일
//

#include "pch.h"
#include "ActionCommanderMapTool.h"
#include "CControlProperty.h"


// CControlProperty

IMPLEMENT_DYNAMIC(CControlProperty, CPropertySheet)

CControlProperty::CControlProperty(CWnd* pParentWnd)
{
	AddPage(&m_TilePage);
	AddPage(&m_ForeGroundPage);
	AddPage(&m_FlipForeGroundPage);
	AddPage(&m_MonsterPage);
	AddPage(&m_EtcPage);
}

CControlProperty::CControlProperty(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(nIDCaption, pParentWnd, iSelectPage)
{

}

CControlProperty::CControlProperty(LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(pszCaption, pParentWnd, iSelectPage)
{

}

CControlProperty::~CControlProperty()
{
}


BEGIN_MESSAGE_MAP(CControlProperty, CPropertySheet)
END_MESSAGE_MAP()


// CControlProperty 메시지 처리기
