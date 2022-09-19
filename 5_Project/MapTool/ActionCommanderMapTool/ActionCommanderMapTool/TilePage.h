#pragma once


// CTilePage 대화 상자
class CActionCommanderMapToolDoc;
class CTilePage : public CPropertyPage
{
	DECLARE_DYNAMIC(CTilePage)

public:
	CTilePage();
	virtual ~CTilePage();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CTilePage };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.


	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedTopleft();
	afx_msg void OnBnClickedLefttop();
	afx_msg void OnBnClickedTop();
	afx_msg void OnBnClickedRighttop();
	afx_msg void OnBnClickedLeft();
	afx_msg void OnBnClickedCenter();
	afx_msg void OnBnClickedRight();
	afx_msg void OnBnClickedLeftbottom();
	afx_msg void OnBnClickedBottom();
	afx_msg void OnBnClickedRightbottom();
	afx_msg void OnBnClickedNjplatleft();
	afx_msg void OnBnClickedNjplatcenter();
	afx_msg void OnBnClickedNjplatright();
	afx_msg void OnBnClickedPlatleft();
	afx_msg void OnBnClickedPlatcenter();
	afx_msg void OnBnClickedPlatright();
	afx_msg void OnBnClickedEraser();
	virtual BOOL OnInitDialog();

protected:
	// 버튼 정보
	CButton m_LeftTop;
	CButton m_Top;
	CButton m_RightTop;
	CButton m_Left;
	CButton m_Center;
	CButton m_Right;
	CButton m_LeftBottom;
	CButton m_Bottom;
	CButton m_RightBottom;
	CButton m_NJPlatLeft;
	CButton m_NJPlatCenter;
	CButton m_NJPlatRight;
	CButton m_PlatLeft;
	CButton m_PlatCenter;
	CButton m_PlatRight;
public:
	afx_msg void OnBnClickedDialefttop();
	afx_msg void OnBnClickedDiarighttop();
	afx_msg void OnBnClickedDialeftbottom();
	afx_msg void OnBnClickedDiarightbottom();
	CButton m_DiaLeftTop;
	CButton m_DiaLeftBottom;
	CButton m_DiaRightTop;
	CButton m_DiaRightBottom;
};
