// EtcPage.cpp: 구현 파일
//

#include "pch.h"
#include "ActionCommanderMapTool.h"
#include "EtcPage.h"
#include "afxdialogex.h"
#include "ResourceManager.h"

// CEtcPage 대화 상자

IMPLEMENT_DYNAMIC(CEtcPage, CPropertyPage)

CEtcPage::CEtcPage()
	: CPropertyPage(IDD_CEtcPage)
{

}

CEtcPage::~CEtcPage()
{
}

void CEtcPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_HEART, m_Heart);
	DDX_Control(pDX, IDC_PORTAL, m_Portal);
	DDX_Control(pDX, IDC_PLAYER, m_Player);
	DDX_Control(pDX, IDC_CLEARENG, m_ClearEng);
	DDX_Control(pDX, IDC_CLEARKOR, m_ClearKor);
	DDX_Control(pDX, IDC_CLEARNAME, m_ClearName);
}


BEGIN_MESSAGE_MAP(CEtcPage, CPropertyPage)
	ON_BN_CLICKED(IDC_HEART, &CEtcPage::OnBnClickedHeart)
	ON_BN_CLICKED(IDC_PORTAL, &CEtcPage::OnBnClickedPortal)
	ON_BN_CLICKED(IDC_PLAYER, &CEtcPage::OnBnClickedPlayer)
	ON_BN_CLICKED(IDC_OBJECTERASER2, &CEtcPage::OnBnClickedObjecteraser2)
	ON_BN_CLICKED(IDC_CLEARENG, &CEtcPage::OnBnClickedCleareng)
	ON_BN_CLICKED(IDC_CLEARKOR, &CEtcPage::OnBnClickedClearkor)
	ON_BN_CLICKED(IDC_CLEARNAME, &CEtcPage::OnBnClickedClearname)
END_MESSAGE_MAP()


// CEtcPage 메시지 처리기


void CEtcPage::OnBnClickedHeart()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	theApp.isTileCursor = false;
	theApp.m_ObjCursorState = ObjectAttribute::Heart;
}


void CEtcPage::OnBnClickedPortal()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	theApp.isTileCursor = false;
	theApp.m_ObjCursorState = ObjectAttribute::Portal;
}


void CEtcPage::OnBnClickedPlayer()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	theApp.isTileCursor = false;
	theApp.m_ObjCursorState = ObjectAttribute::Player;
}

void CEtcPage::OnBnClickedCleareng()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	theApp.isTileCursor = false;
	theApp.m_ObjCursorState = ObjectAttribute::ClearEng;
}


void CEtcPage::OnBnClickedClearkor()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	theApp.isTileCursor = false;
	theApp.m_ObjCursorState = ObjectAttribute::ClearKor;
}


void CEtcPage::OnBnClickedClearname()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	theApp.isTileCursor = false;
	theApp.m_ObjCursorState = ObjectAttribute::ClearName;
}

void CEtcPage::OnBnClickedObjecteraser2()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	theApp.isTileCursor = false;
	theApp.m_ObjCursorState = ObjectAttribute::None;
}


BOOL CEtcPage::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	// 비트맵 이미지 추가
	m_Heart.SetBitmap(*CResourceManager::GetInstance()->GetObjectImage(ObjectAttribute::Heart));
	m_Portal.SetBitmap(*CResourceManager::GetInstance()->GetObjectImage(ObjectAttribute::Portal));
	m_Player.SetBitmap(*CResourceManager::GetInstance()->GetObjectImage(ObjectAttribute::Player));

	m_ClearEng.SetBitmap(*CResourceManager::GetInstance()->GetObjectImage(ObjectAttribute::ClearEng));
	m_ClearKor.SetBitmap(*CResourceManager::GetInstance()->GetObjectImage(ObjectAttribute::ClearKor));
	m_ClearName.SetBitmap(*CResourceManager::GetInstance()->GetObjectImage(ObjectAttribute::ClearName));
	
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


