// MapSizeDlg.cpp: 구현 파일
//

#include "pch.h"
#include "ActionCommanderMapTool.h"
#include "MapSizeDlg.h"
#include "afxdialogex.h"


// MapSizeDlg 대화 상자

IMPLEMENT_DYNAMIC(MapSizeDlg, CDialogEx)

MapSizeDlg::MapSizeDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MapSizeDlg, pParent)
{

}

MapSizeDlg::~MapSizeDlg()
{
}

void MapSizeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDITWIDTH, m_EditWidth);
	DDX_Control(pDX, IDC_EDITHEIGHT, m_EditHeight);
	DDX_Control(pDX, IDC_SPINWIDTH, m_SpinWidth);
	DDX_Control(pDX, IDC_SPINHEIGHT, m_SpinHeight);

	m_SpinWidth.SetRange(1, 100);
	m_SpinWidth.SetPos(0);
	m_SpinHeight.SetRange(1, 50);
	m_SpinHeight.SetPos(0);

	m_EditWidth.SetWindowText(_T("1"));
	m_EditHeight.SetWindowText(_T("1"));
}


BEGIN_MESSAGE_MAP(MapSizeDlg, CDialogEx)
	ON_EN_CHANGE(IDC_EDITWIDTH, &MapSizeDlg::OnEnChangeEditwidth)
	ON_EN_CHANGE(IDC_EDITHEIGHT, &MapSizeDlg::OnEnChangeEditheight)
	ON_BN_CLICKED(IDOK, &MapSizeDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &MapSizeDlg::OnBnClickedCancel)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPINWIDTH, &MapSizeDlg::OnDeltaposSpinwidth)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPINHEIGHT, &MapSizeDlg::OnDeltaposSpinheight)
END_MESSAGE_MAP()


// MapSizeDlg 메시지 처리기

void MapSizeDlg::OnEnChangeEditwidth()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString strTemp;

	m_EditWidth.GetWindowText(strTemp);
	m_DlgWidth = _ttoi(strTemp.GetBuffer());

	// 범위 제한
	if (m_DlgWidth > 100)
	{
		m_DlgWidth = 100;
		strTemp.Format(_T("%d"), m_DlgWidth);
		m_EditWidth.SetWindowText(strTemp);
	}
	else if (m_DlgWidth < 1)
	{
		m_DlgWidth = 1;
		strTemp.Format(_T("%d"), m_DlgWidth);
		m_EditWidth.SetWindowText(strTemp);
	}
}


void MapSizeDlg::OnEnChangeEditheight()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString strTemp;

	m_EditHeight.GetWindowText(strTemp);
	m_DlgHeight = _ttoi(strTemp.GetBuffer());

	// 범위 제한
	if (m_DlgHeight > 50)

	{
		m_DlgHeight = 50;
		strTemp.Format(_T("%d"), m_DlgHeight);
		m_EditHeight.SetWindowText(strTemp);
	}
	else if (m_DlgHeight < 1)
	{
		m_DlgHeight = 1;
		strTemp.Format(_T("%d"), m_DlgHeight);
		m_EditHeight.SetWindowText(strTemp);
	}
}


void MapSizeDlg::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	// 에디트의 정보를 가져와서 넣기.
	CString strTemp;
	
	m_EditWidth.GetWindowText(strTemp);
	m_DlgWidth = _ttoi(strTemp.GetBuffer());

	theApp.m_MapWidth = m_DlgWidth;

	m_EditHeight.GetWindowText(strTemp);
	m_DlgHeight = _ttoi(strTemp.GetBuffer());

	theApp.m_MapHeight = m_DlgHeight;

	CDialogEx::OnOK();
}


void MapSizeDlg::OnBnClickedCancel()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	exit(0);
}

void MapSizeDlg::OnDeltaposSpinwidth(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString strTemp;

	int iVal = pNMUpDown->iPos + pNMUpDown->iDelta;

	if (iVal >= 0 && iVal <= 100)
	{
		strTemp.Format(_T("%d"), iVal);
		m_EditWidth.SetWindowText(strTemp);
	}
	*pResult = 0;
}


void MapSizeDlg::OnDeltaposSpinheight(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString strTemp;

	int iVal = pNMUpDown->iPos + pNMUpDown->iDelta;

	// 범위 제한
	if (iVal >= 0 && iVal <= 50)
	{
		strTemp.Format(_T("%d"), iVal);
		m_EditHeight.SetWindowText(strTemp);
	}
	*pResult = 0;
}
