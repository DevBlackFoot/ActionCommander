// TilePage.cpp: 구현 파일
//

#include "pch.h"
#include "ActionCommanderMapTool.h"
#include "TilePage.h"
#include "afxdialogex.h"
#include "ResourceManager.h"

// CTilePage 대화 상자

IMPLEMENT_DYNAMIC(CTilePage, CPropertyPage)

CTilePage::CTilePage()
	: CPropertyPage(IDD_CTilePage)
{

}

CTilePage::~CTilePage()
{
}

void CTilePage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LEFTTOP, m_LeftTop);
	DDX_Control(pDX, IDC_TOP, m_Top);
	DDX_Control(pDX, IDC_RIGHTTOP, m_RightTop);
	DDX_Control(pDX, IDC_LEFT, m_Left);
	DDX_Control(pDX, IDC_CENTER, m_Center);
	DDX_Control(pDX, IDC_RIGHT, m_Right);
	DDX_Control(pDX, IDC_LEFTBOTTOM, m_LeftBottom);
	DDX_Control(pDX, IDC_BOTTOM, m_Bottom);
	DDX_Control(pDX, IDC_NJPLATLEFT, m_NJPlatLeft);
	DDX_Control(pDX, IDC_NJPLATCENTER, m_NJPlatCenter);
	DDX_Control(pDX, IDC_NJPLATRIGHT, m_NJPlatRight);
	DDX_Control(pDX, IDC_PLATLEFT, m_PlatLeft);
	DDX_Control(pDX, IDC_PLATCENTER, m_PlatCenter);
	DDX_Control(pDX, IDC_PLATRIGHT, m_PlatRight);
	DDX_Control(pDX, IDC_RIGHTBOTTOM, m_RightBottom);
	DDX_Control(pDX, IDC_DIALEFTTOP, m_DiaLeftTop);
	DDX_Control(pDX, IDC_DIARIGHTTOP, m_DiaRightTop);
	DDX_Control(pDX, IDC_DIALEFTBOTTOM, m_DiaLeftBottom);
	DDX_Control(pDX, IDC_DIARIGHTBOTTOM, m_DiaRightBottom);
}


BEGIN_MESSAGE_MAP(CTilePage, CPropertyPage)
	ON_BN_CLICKED(IDC_LEFTTOP, &CTilePage::OnBnClickedLefttop)
	ON_BN_CLICKED(IDC_TOP, &CTilePage::OnBnClickedTop)
	ON_BN_CLICKED(IDC_RIGHTTOP, &CTilePage::OnBnClickedRighttop)
	ON_BN_CLICKED(IDC_LEFT, &CTilePage::OnBnClickedLeft)
	ON_BN_CLICKED(IDC_CENTER, &CTilePage::OnBnClickedCenter)
	ON_BN_CLICKED(IDC_RIGHT, &CTilePage::OnBnClickedRight)
	ON_BN_CLICKED(IDC_LEFTBOTTOM, &CTilePage::OnBnClickedLeftbottom)
	ON_BN_CLICKED(IDC_BOTTOM, &CTilePage::OnBnClickedBottom)
	ON_BN_CLICKED(IDC_RIGHTBOTTOM, &CTilePage::OnBnClickedRightbottom)
	ON_BN_CLICKED(IDC_NJPLATLEFT, &CTilePage::OnBnClickedNjplatleft)
	ON_BN_CLICKED(IDC_NJPLATCENTER, &CTilePage::OnBnClickedNjplatcenter)
	ON_BN_CLICKED(IDC_NJPLATRIGHT, &CTilePage::OnBnClickedNjplatright)
	ON_BN_CLICKED(IDC_PLATLEFT, &CTilePage::OnBnClickedPlatleft)
	ON_BN_CLICKED(IDC_PLATCENTER, &CTilePage::OnBnClickedPlatcenter)
	ON_BN_CLICKED(IDC_PLATRIGHT, &CTilePage::OnBnClickedPlatright)
	ON_BN_CLICKED(IDC_ERASER, &CTilePage::OnBnClickedEraser)
	ON_BN_CLICKED(IDC_DIALEFTTOP, &CTilePage::OnBnClickedDialefttop)
	ON_BN_CLICKED(IDC_DIARIGHTTOP, &CTilePage::OnBnClickedDiarighttop)
	ON_BN_CLICKED(IDC_DIALEFTBOTTOM, &CTilePage::OnBnClickedDialeftbottom)
	ON_BN_CLICKED(IDC_DIARIGHTBOTTOM, &CTilePage::OnBnClickedDiarightbottom)
END_MESSAGE_MAP()


// CTilePage 메시지 처리기

void CTilePage::OnBnClickedLefttop()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	theApp.isTileCursor = true;
	theApp.m_CursorState = TileAttribute::GroundLeftTop;
}


void CTilePage::OnBnClickedTop()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	theApp.isTileCursor = true;
	theApp.m_CursorState = TileAttribute::GroundTop;
}


void CTilePage::OnBnClickedRighttop()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	theApp.isTileCursor = true;
	theApp.m_CursorState = TileAttribute::GroundRightTop;
}


void CTilePage::OnBnClickedLeft()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	theApp.isTileCursor = true;
	theApp.m_CursorState = TileAttribute::GroundLeft;
}


void CTilePage::OnBnClickedCenter()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	theApp.isTileCursor = true;
	theApp.m_CursorState = TileAttribute::GroundCenter;
}


void CTilePage::OnBnClickedRight()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	theApp.isTileCursor = true;
	theApp.m_CursorState = TileAttribute::GroundRight;
}


void CTilePage::OnBnClickedLeftbottom()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	theApp.isTileCursor = true;
	theApp.m_CursorState = TileAttribute::GroundLeftBottom;
}


void CTilePage::OnBnClickedBottom()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	theApp.isTileCursor = true;
	theApp.m_CursorState = TileAttribute::GroundBottom;
}


void CTilePage::OnBnClickedRightbottom()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	theApp.isTileCursor = true;
	theApp.m_CursorState = TileAttribute::GroundRightBottom;
}


void CTilePage::OnBnClickedNjplatleft()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	theApp.isTileCursor = true;
	theApp.m_CursorState = TileAttribute::NotJumpPlatLeft;
}


void CTilePage::OnBnClickedNjplatcenter()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	theApp.isTileCursor = true;
	theApp.m_CursorState = TileAttribute::NotJumpPlatCenter;
}


void CTilePage::OnBnClickedNjplatright()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	theApp.isTileCursor = true;
	theApp.m_CursorState = TileAttribute::NotJumpPlatRight;
}


void CTilePage::OnBnClickedPlatleft()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	theApp.isTileCursor = true;
	theApp.m_CursorState = TileAttribute::JumpPlatLeft;
}


void CTilePage::OnBnClickedPlatcenter()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	theApp.isTileCursor = true;
	theApp.m_CursorState = TileAttribute::JumpPlatCenter;
}


void CTilePage::OnBnClickedPlatright()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	theApp.isTileCursor = true;
	theApp.m_CursorState = TileAttribute::JumpPlatRight;
}


void CTilePage::OnBnClickedDialefttop()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	theApp.isTileCursor = true;
	theApp.m_CursorState = TileAttribute::GroundDiaLeftTop;
}


void CTilePage::OnBnClickedDiarighttop()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	theApp.isTileCursor = true;
	theApp.m_CursorState = TileAttribute::GroundDiaRightTop;
}


void CTilePage::OnBnClickedDialeftbottom()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	theApp.isTileCursor = true;
	theApp.m_CursorState = TileAttribute::GroundDiaLeftBottom;
}


void CTilePage::OnBnClickedDiarightbottom()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	theApp.isTileCursor = true;
	theApp.m_CursorState = TileAttribute::GroundDiaRightBottom;
}


void CTilePage::OnBnClickedEraser()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	theApp.isTileCursor = true;
	theApp.m_CursorState = TileAttribute::None;
}

BOOL CTilePage::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	// 비트맵 이미지를 가져와서 추가해준다.
	m_LeftTop.SetBitmap(*CResourceManager::GetInstance()->GetTileImage(TileAttribute::GroundLeftTop));
	m_Left.SetBitmap(*CResourceManager::GetInstance()->GetTileImage(TileAttribute::GroundLeft));
	m_LeftBottom.SetBitmap(*CResourceManager::GetInstance()->GetTileImage(TileAttribute::GroundLeftBottom));
	m_Top.SetBitmap(*CResourceManager::GetInstance()->GetTileImage(TileAttribute::GroundTop));
	m_Center.SetBitmap(*CResourceManager::GetInstance()->GetTileImage(TileAttribute::GroundCenter));
	m_Bottom.SetBitmap(*CResourceManager::GetInstance()->GetTileImage(TileAttribute::GroundBottom));
	m_RightTop.SetBitmap(*CResourceManager::GetInstance()->GetTileImage(TileAttribute::GroundRightTop));
	m_Right.SetBitmap(*CResourceManager::GetInstance()->GetTileImage(TileAttribute::GroundRight));
	m_RightBottom.SetBitmap(*CResourceManager::GetInstance()->GetTileImage(TileAttribute::GroundRightBottom));

	m_NJPlatLeft.SetBitmap(*CResourceManager::GetInstance()->GetTileImage(TileAttribute::NotJumpPlatLeft));
	m_NJPlatCenter.SetBitmap(*CResourceManager::GetInstance()->GetTileImage(TileAttribute::NotJumpPlatCenter));
	m_NJPlatRight.SetBitmap(*CResourceManager::GetInstance()->GetTileImage(TileAttribute::NotJumpPlatRight));

	m_PlatLeft.SetBitmap(*CResourceManager::GetInstance()->GetTileImage(TileAttribute::JumpPlatLeft));
	m_PlatCenter.SetBitmap(*CResourceManager::GetInstance()->GetTileImage(TileAttribute::JumpPlatCenter));
	m_PlatRight.SetBitmap(*CResourceManager::GetInstance()->GetTileImage(TileAttribute::JumpPlatRight));

	m_DiaLeftTop.SetBitmap(*CResourceManager::GetInstance()->GetTileImage(TileAttribute::GroundDiaLeftTop));
	m_DiaLeftBottom.SetBitmap(*CResourceManager::GetInstance()->GetTileImage(TileAttribute::GroundDiaLeftBottom));
	m_DiaRightTop.SetBitmap(*CResourceManager::GetInstance()->GetTileImage(TileAttribute::GroundDiaRightTop));
	m_DiaRightBottom.SetBitmap(*CResourceManager::GetInstance()->GetTileImage(TileAttribute::GroundDiaRightBottom));

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

