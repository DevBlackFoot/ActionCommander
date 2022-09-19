#pragma once


// CFlipForeGroundPage 대화 상자

class CFlipForeGroundPage : public CPropertyPage
{
	DECLARE_DYNAMIC(CFlipForeGroundPage)

public:
	CFlipForeGroundPage();
	virtual ~CFlipForeGroundPage();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CFlipForeGroundPage };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedLeaf0flip();
	afx_msg void OnBnClickedLeaf1flip();
	afx_msg void OnBnClickedLeaf2flip();
	afx_msg void OnBnClickedLeaf3flip();
	afx_msg void OnBnClickedLeaf4flip();
	afx_msg void OnBnClickedRock0flip();
	afx_msg void OnBnClickedRock1flip();
	afx_msg void OnBnClickedTree0flip();
	afx_msg void OnBnClickedTree1flip();
	afx_msg void OnBnClickedTree2flip();
	afx_msg void OnBnClickedObjecteraser3();

	CButton m_Leaf0Flip;
	CButton m_Leaf1Flip;
	CButton m_Leaf2Flip;
	CButton m_Leaf3Flip;
	CButton m_Leaf4Flip;
	CButton m_Rock0Flip;
	CButton m_Rock1Flip;
	CButton m_Tree0Flip;
	CButton m_Tree1Flip;
	CButton m_Tree2Flip;

	virtual BOOL OnInitDialog();
};
