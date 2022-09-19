#pragma once
#include "MonsterPage.h"
#include "TilePage.h"
#include "ForeGroundPage.h"
#include "FlipForeGroundPage.h"
#include "EtcPage.h"

// CControlProperty

class CControlProperty : public CPropertySheet
{
	DECLARE_DYNAMIC(CControlProperty)

public:
	CControlProperty(CWnd* pParentWnd);
	CControlProperty(UINT nIDCaption, CWnd* pParentWnd = nullptr, UINT iSelectPage = 0);
	CControlProperty(LPCTSTR pszCaption, CWnd* pParentWnd = nullptr, UINT iSelectPage = 0);
	virtual ~CControlProperty();

protected:
	CTilePage				m_TilePage;
	CForeGroundPage			m_ForeGroundPage;
	CFlipForeGroundPage		m_FlipForeGroundPage;
	CMonsterPage			m_MonsterPage;
	CEtcPage				m_EtcPage;
	DECLARE_MESSAGE_MAP()
};


