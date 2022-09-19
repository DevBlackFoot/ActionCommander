
// ActionCommanderMapToolView.cpp: CActionCommanderMapToolView 클래스의 구현
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "ActionCommanderMapTool.h"
#endif

#include "ActionCommanderMapToolDoc.h"
#include "ActionCommanderMapToolView.h"
#include "ResourceManager.h"
#include "MiniMapView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CActionCommanderMapToolView

IMPLEMENT_DYNCREATE(CActionCommanderMapToolView, CScrollView)

BEGIN_MESSAGE_MAP(CActionCommanderMapToolView, CScrollView)
	// 표준 인쇄 명령입니다.
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_MOUSEMOVE()
	ON_WM_CREATE()
	ON_REGISTERED_MESSAGE(AFX_WM_DRAW2D, &CActionCommanderMapToolView::OnDraw2D)
	ON_WM_LBUTTONDOWN()
	ON_WM_DESTROY()
	ON_WM_LBUTTONUP()
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()

// CActionCommanderMapToolView 생성/소멸

CActionCommanderMapToolView::CActionCommanderMapToolView() noexcept
{
	// TODO: 여기에 생성 코드를 추가합니다.

	// 초기화 시작할 때 D2D 서포트 선언
}

int CActionCommanderMapToolView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	BOOL bUseDCRenderTarget = TRUE;
	EnableD2DSupport(TRUE, bUseDCRenderTarget);

	if (!IsD2DSupportEnabled())
		return -1;

	// TODO:  여기에 특수화된 작성 코드를 추가합니다.

	// 펜 가져오기
	m_pGridPen = new CD2DSolidColorBrush(GetRenderTarget(), D2D1::ColorF(D2D1::ColorF::Gray));
	m_pRedPen = new CD2DSolidColorBrush(GetRenderTarget(), D2D1::ColorF(D2D1::ColorF::Red));
	m_pBluePen = new CD2DSolidColorBrush(GetRenderTarget(), D2D1::ColorF(D2D1::ColorF::Cyan));
	m_pColliderPen = new CD2DSolidColorBrush(GetRenderTarget(), D2D1::ColorF(0x00FF00));

	return 0;
}

CActionCommanderMapToolView::~CActionCommanderMapToolView()
{
}

BOOL CActionCommanderMapToolView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return CView::PreCreateWindow(cs);
}

// CActionCommanderMapToolView 그리기

void CActionCommanderMapToolView::OnDraw(CDC* /*pDC*/)
{
	CActionCommanderMapToolDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 여기에 원시 데이터에 대한 그리기 코드를 추가합니다.
}

void CActionCommanderMapToolView::DrawTileMap()
{
	// 타일 맵 벡터 전체를 돌면서.. 그림을 그려준다.
	CActionCommanderMapToolDoc* pDoc = GetDocument();

	for (int y = 0; y < theApp.m_MapHeight; y++)
	{
		for (int x = 0; x < theApp.m_MapWidth; x++)
		{
			TileAttribute nowTile = pDoc->m_TileMap[theApp.m_MapWidth * y + x]->tileAttribute;
			// None인지 여부 검사
			if (nowTile == TileAttribute::None)
			{
				continue;
			}
			else
			{
				m_pDCRenderTarget->DrawBitmap(m_TileBitmap.at(nowTile),
					{ static_cast<float>(x * TILE_SIZE), static_cast<float>(y * TILE_SIZE),
					static_cast<float>((x * TILE_SIZE) + m_TileBitmap.at(nowTile)->GetSize().width),
						static_cast<float>((y * TILE_SIZE) + m_TileBitmap.at(nowTile)->GetSize().height) });
			}
		}
	}

}

void CActionCommanderMapToolView::DrawObject()
{
	// 오브젝트 맵 리스트를 돌면서 그림을 그려준다.
	CActionCommanderMapToolDoc* pDoc = GetDocument();

	for (auto iter = pDoc->m_ObjectMap.begin(); iter != pDoc->m_ObjectMap.end(); iter++)
	{
		m_pDCRenderTarget->DrawBitmap(m_ObjectBitmap.at((*iter)->objectAttribute),
			{ static_cast<float>((*iter)->left), static_cast<float>((*iter)->top),
				(*iter)->left + (*iter)->bmpWidth, (*iter)->top + (*iter)->bmpHeight }
		);

		m_pDCRenderTarget->DrawEllipse({ {static_cast<float>((*iter)->centerX), static_cast<float>((*iter)->centerY)},
		{2, 2} }, m_pBluePen, 2);
	}
}

void CActionCommanderMapToolView::DrawCollider()
{
	CActionCommanderMapToolDoc* pDoc = GetDocument();

	for (auto iter = pDoc->m_Colliders.begin(); iter != pDoc->m_Colliders.end(); iter++)
	{
		m_pDCRenderTarget->DrawRectangle(*iter, m_pColliderPen, 1.5);
	}
}

// 마우스 포지션 따라서 움직이는 커서 그리는 함수
void CActionCommanderMapToolView::DrawCursor()
{
	// 현재 선택한 bitmap 이미지 표시
	if (theApp.isTileCursor)
	{
		if (theApp.m_CursorState == TileAttribute::JumpPlatLeft ||
			theApp.m_CursorState == TileAttribute::JumpPlatCenter ||
			theApp.m_CursorState == TileAttribute::JumpPlatRight)
		{
			m_pRenderTarget->DrawBitmap(m_TileBitmap.at(theApp.m_CursorState),
				{ static_cast<float>(m_MousePos.x - TILE_SIZE / 2), static_cast<float>(m_MousePos.y - TILE_SIZE / 4),
				static_cast<float>(m_MousePos.x + TILE_SIZE / 2), static_cast<float>(m_MousePos.y + TILE_SIZE / 4) });
		}
		else if (theApp.m_CursorState != TileAttribute::None)
		{
			m_pRenderTarget->DrawBitmap(m_TileBitmap.at(theApp.m_CursorState),
				{ static_cast<float>(m_MousePos.x - TILE_SIZE / 2), static_cast<float>(m_MousePos.y - TILE_SIZE / 2),
					static_cast<float>(m_MousePos.x + TILE_SIZE / 2), static_cast<float>(m_MousePos.y + TILE_SIZE / 2) });
		}

		// 마우스가 있는 위치에 사각형 그려주기.
		CPoint _gridMouse;
		_gridMouse.x = (m_MousePos.x / TILE_SIZE) * TILE_SIZE;
		_gridMouse.y = (m_MousePos.y / TILE_SIZE) * TILE_SIZE;

		if (_gridMouse.x >= 0 && _gridMouse.y >= 0 && _gridMouse.x < theApp.m_MapWidth * TILE_SIZE && _gridMouse.y < theApp.m_MapHeight * TILE_SIZE)
		{
			m_pRenderTarget->DrawRectangle(
				{ static_cast<float>(_gridMouse.x), static_cast<float>(_gridMouse.y),
					static_cast<float>(_gridMouse.x + TILE_SIZE), static_cast<float>(_gridMouse.y + TILE_SIZE) },
				m_pRedPen, 1.5);
		}
	}
	else
	{
		if (theApp.m_ObjCursorState != ObjectAttribute::None)
		{
			m_pRenderTarget->DrawBitmap(m_ObjectBitmap.at(theApp.m_ObjCursorState),
				{ m_MousePos.x - m_ObjectBitmap.at(theApp.m_ObjCursorState)->GetSize().width / 2,
					m_MousePos.y - m_ObjectBitmap.at(theApp.m_ObjCursorState)->GetSize().height / 2,
					m_MousePos.x + m_ObjectBitmap.at(theApp.m_ObjCursorState)->GetSize().width / 2,
					m_MousePos.y + m_ObjectBitmap.at(theApp.m_ObjCursorState)->GetSize().height / 2 });
		}
		else
		{
			m_pRenderTarget->DrawRectangle(
				{ static_cast<float>(m_MousePos.x - TILE_SIZE / 2),
					static_cast<float>(m_MousePos.y - TILE_SIZE / 2),
					static_cast<float>(m_MousePos.x + TILE_SIZE / 2),
					static_cast<float>(m_MousePos.y + TILE_SIZE / 2) }, m_pRedPen);
		}
	}

}

void CActionCommanderMapToolView::InitResource()
{
	if (!m_loadBitmap)
	{
		HBITMAP hBmp = *(CResourceManager::GetInstance()->GetBGImage(0));
		m_BG = new CD2DBitmap(m_pDCRenderTarget, hBmp);

		// h 비트맵에 정보를 넣어서. 추가.
#pragma region TileBitmapLoad
		// 땅 위쪽
		hBmp = *(CResourceManager::GetInstance()->GetTileImage(TileAttribute::GroundLeftTop));
		if (hBmp != nullptr)
		{
			m_TileBitmap.insert({ TileAttribute::GroundLeftTop, new CD2DBitmap(m_pDCRenderTarget, hBmp) });
		}
		hBmp = *(CResourceManager::GetInstance()->GetTileImage(TileAttribute::GroundTop));
		if (hBmp != nullptr)
		{
			m_TileBitmap.insert({ TileAttribute::GroundTop, new CD2DBitmap(m_pDCRenderTarget, hBmp) });
		}
		hBmp = *(CResourceManager::GetInstance()->GetTileImage(TileAttribute::GroundRightTop));
		if (hBmp != nullptr)
		{
			m_TileBitmap.insert({ TileAttribute::GroundRightTop, new CD2DBitmap(m_pDCRenderTarget, hBmp) });
		}
		// 땅 중간
		hBmp = *(CResourceManager::GetInstance()->GetTileImage(TileAttribute::GroundLeft));
		if (hBmp != nullptr)
		{
			m_TileBitmap.insert({ TileAttribute::GroundLeft, new CD2DBitmap(m_pDCRenderTarget, hBmp) });
		}
		hBmp = *(CResourceManager::GetInstance()->GetTileImage(TileAttribute::GroundCenter));
		if (hBmp != nullptr)
		{
			m_TileBitmap.insert({ TileAttribute::GroundCenter, new CD2DBitmap(m_pDCRenderTarget, hBmp) });
		}
		hBmp = *(CResourceManager::GetInstance()->GetTileImage(TileAttribute::GroundRight));
		if (hBmp != nullptr)
		{
			m_TileBitmap.insert({ TileAttribute::GroundRight, new CD2DBitmap(m_pDCRenderTarget, hBmp) });
		}
		// 땅 아래
		hBmp = *(CResourceManager::GetInstance()->GetTileImage(TileAttribute::GroundLeftBottom));
		if (hBmp != nullptr)
		{
			m_TileBitmap.insert({ TileAttribute::GroundLeftBottom, new CD2DBitmap(m_pDCRenderTarget, hBmp) });
		}
		hBmp = *(CResourceManager::GetInstance()->GetTileImage(TileAttribute::GroundBottom));
		if (hBmp != nullptr)
		{
			m_TileBitmap.insert({ TileAttribute::GroundBottom, new CD2DBitmap(m_pDCRenderTarget, hBmp) });
		}
		hBmp = *(CResourceManager::GetInstance()->GetTileImage(TileAttribute::GroundRightBottom));
		if (hBmp != nullptr)
		{
			m_TileBitmap.insert({ TileAttribute::GroundRightBottom, new CD2DBitmap(m_pDCRenderTarget, hBmp) });
		}
		// 땅 대각선
		hBmp = *(CResourceManager::GetInstance()->GetTileImage(TileAttribute::GroundDiaLeftTop));
		if (hBmp != nullptr)
		{
			m_TileBitmap.insert({ TileAttribute::GroundDiaLeftTop, new CD2DBitmap(m_pDCRenderTarget, hBmp) });
		}
		hBmp = *(CResourceManager::GetInstance()->GetTileImage(TileAttribute::GroundDiaRightTop));
		if (hBmp != nullptr)
		{
			m_TileBitmap.insert({ TileAttribute::GroundDiaRightTop, new CD2DBitmap(m_pDCRenderTarget, hBmp) });
		}
		hBmp = *(CResourceManager::GetInstance()->GetTileImage(TileAttribute::GroundDiaLeftBottom));
		if (hBmp != nullptr)
		{
			m_TileBitmap.insert({ TileAttribute::GroundDiaLeftBottom, new CD2DBitmap(m_pDCRenderTarget, hBmp) });
		}
		hBmp = *(CResourceManager::GetInstance()->GetTileImage(TileAttribute::GroundDiaRightBottom));
		if (hBmp != nullptr)
		{
			m_TileBitmap.insert({ TileAttribute::GroundDiaRightBottom, new CD2DBitmap(m_pDCRenderTarget, hBmp) });
		}

		// 못 넘는 플랫폼
		hBmp = *(CResourceManager::GetInstance()->GetTileImage(TileAttribute::NotJumpPlatLeft));
		if (hBmp != nullptr)
		{
			m_TileBitmap.insert({ TileAttribute::NotJumpPlatLeft, new CD2DBitmap(m_pDCRenderTarget, hBmp) });
		}
		hBmp = *(CResourceManager::GetInstance()->GetTileImage(TileAttribute::NotJumpPlatCenter));
		if (hBmp != nullptr)
		{
			m_TileBitmap.insert({ TileAttribute::NotJumpPlatCenter, new CD2DBitmap(m_pDCRenderTarget, hBmp) });
		}
		hBmp = *(CResourceManager::GetInstance()->GetTileImage(TileAttribute::NotJumpPlatRight));
		if (hBmp != nullptr)
		{
			m_TileBitmap.insert({ TileAttribute::NotJumpPlatRight, new CD2DBitmap(m_pDCRenderTarget, hBmp) });
		}
		// 넘는 플랫폼
		hBmp = *(CResourceManager::GetInstance()->GetTileImage(TileAttribute::JumpPlatLeft));
		if (hBmp != nullptr)
		{
			m_TileBitmap.insert({ TileAttribute::JumpPlatLeft, new CD2DBitmap(m_pDCRenderTarget, hBmp) });
		}
		hBmp = *(CResourceManager::GetInstance()->GetTileImage(TileAttribute::JumpPlatCenter));
		if (hBmp != nullptr)
		{
			m_TileBitmap.insert({ TileAttribute::JumpPlatCenter, new CD2DBitmap(m_pDCRenderTarget, hBmp) });
		}
		hBmp = *(CResourceManager::GetInstance()->GetTileImage(TileAttribute::JumpPlatRight));
		if (hBmp != nullptr)
		{
			m_TileBitmap.insert({ TileAttribute::JumpPlatRight, new CD2DBitmap(m_pDCRenderTarget, hBmp) });
		}
#pragma endregion

#pragma region ObjBitmapLoad
		// FGObject
		hBmp = *(CResourceManager::GetInstance()->GetObjectImage(ObjectAttribute::ObjectLeaf0));
		if (hBmp != nullptr)
		{
			m_ObjectBitmap.insert({ ObjectAttribute::ObjectLeaf0, new CD2DBitmap(m_pDCRenderTarget, hBmp) });
		}
		hBmp = *(CResourceManager::GetInstance()->GetObjectImage(ObjectAttribute::ObjectLeaf1));
		if (hBmp != nullptr)
		{
			m_ObjectBitmap.insert({ ObjectAttribute::ObjectLeaf1, new CD2DBitmap(m_pDCRenderTarget, hBmp) });
		}
		hBmp = *(CResourceManager::GetInstance()->GetObjectImage(ObjectAttribute::ObjectLeaf2));
		if (hBmp != nullptr)
		{
			m_ObjectBitmap.insert({ ObjectAttribute::ObjectLeaf2, new CD2DBitmap(m_pDCRenderTarget, hBmp) });
		}
		hBmp = *(CResourceManager::GetInstance()->GetObjectImage(ObjectAttribute::ObjectLeaf3));
		if (hBmp != nullptr)
		{
			m_ObjectBitmap.insert({ ObjectAttribute::ObjectLeaf3, new CD2DBitmap(m_pDCRenderTarget, hBmp) });
		}
		hBmp = *(CResourceManager::GetInstance()->GetObjectImage(ObjectAttribute::ObjectLeaf4));
		if (hBmp != nullptr)
		{
			m_ObjectBitmap.insert({ ObjectAttribute::ObjectLeaf4, new CD2DBitmap(m_pDCRenderTarget, hBmp) });
		}
		hBmp = *(CResourceManager::GetInstance()->GetObjectImage(ObjectAttribute::ObjectRock0));
		if (hBmp != nullptr)
		{
			m_ObjectBitmap.insert({ ObjectAttribute::ObjectRock0, new CD2DBitmap(m_pDCRenderTarget, hBmp) });
		}
		hBmp = *(CResourceManager::GetInstance()->GetObjectImage(ObjectAttribute::ObjectRock1));
		if (hBmp != nullptr)
		{
			m_ObjectBitmap.insert({ ObjectAttribute::ObjectRock1, new CD2DBitmap(m_pDCRenderTarget, hBmp) });
		}
		hBmp = *(CResourceManager::GetInstance()->GetObjectImage(ObjectAttribute::ObjectSign0));
		if (hBmp != nullptr)
		{
			m_ObjectBitmap.insert({ ObjectAttribute::ObjectSign0, new CD2DBitmap(m_pDCRenderTarget, hBmp) });
		}
		hBmp = *(CResourceManager::GetInstance()->GetObjectImage(ObjectAttribute::ObjectSign1));
		if (hBmp != nullptr)
		{
			m_ObjectBitmap.insert({ ObjectAttribute::ObjectSign1, new CD2DBitmap(m_pDCRenderTarget, hBmp) });
		}
		hBmp = *(CResourceManager::GetInstance()->GetObjectImage(ObjectAttribute::ObjectSign2));
		if (hBmp != nullptr)
		{
			m_ObjectBitmap.insert({ ObjectAttribute::ObjectSign2, new CD2DBitmap(m_pDCRenderTarget, hBmp) });
		}
		hBmp = *(CResourceManager::GetInstance()->GetObjectImage(ObjectAttribute::ObjectSign3));
		if (hBmp != nullptr)
		{
			m_ObjectBitmap.insert({ ObjectAttribute::ObjectSign3, new CD2DBitmap(m_pDCRenderTarget, hBmp) });
		}
		hBmp = *(CResourceManager::GetInstance()->GetObjectImage(ObjectAttribute::ObjectTree0));
		if (hBmp != nullptr)
		{
			m_ObjectBitmap.insert({ ObjectAttribute::ObjectTree0, new CD2DBitmap(m_pDCRenderTarget, hBmp) });
		}
		hBmp = *(CResourceManager::GetInstance()->GetObjectImage(ObjectAttribute::ObjectTree1));
		if (hBmp != nullptr)
		{
			m_ObjectBitmap.insert({ ObjectAttribute::ObjectTree1, new CD2DBitmap(m_pDCRenderTarget, hBmp) });
		}
		hBmp = *(CResourceManager::GetInstance()->GetObjectImage(ObjectAttribute::ObjectTree2));
		if (hBmp != nullptr)
		{
			m_ObjectBitmap.insert({ ObjectAttribute::ObjectTree2, new CD2DBitmap(m_pDCRenderTarget, hBmp) });
		}

		// FlipObject
		hBmp = *(CResourceManager::GetInstance()->GetObjectImage(ObjectAttribute::ObjectLeaf0Flip));
		if (hBmp != nullptr)
		{
			m_ObjectBitmap.insert({ ObjectAttribute::ObjectLeaf0Flip, new CD2DBitmap(m_pDCRenderTarget, hBmp) });
		}
		hBmp = *(CResourceManager::GetInstance()->GetObjectImage(ObjectAttribute::ObjectLeaf1Flip));
		if (hBmp != nullptr)
		{
			m_ObjectBitmap.insert({ ObjectAttribute::ObjectLeaf1Flip, new CD2DBitmap(m_pDCRenderTarget, hBmp) });
		}
		hBmp = *(CResourceManager::GetInstance()->GetObjectImage(ObjectAttribute::ObjectLeaf2Flip));
		if (hBmp != nullptr)
		{
			m_ObjectBitmap.insert({ ObjectAttribute::ObjectLeaf2Flip, new CD2DBitmap(m_pDCRenderTarget, hBmp) });
		}
		hBmp = *(CResourceManager::GetInstance()->GetObjectImage(ObjectAttribute::ObjectLeaf3Flip));
		if (hBmp != nullptr)
		{
			m_ObjectBitmap.insert({ ObjectAttribute::ObjectLeaf3Flip, new CD2DBitmap(m_pDCRenderTarget, hBmp) });
		}
		hBmp = *(CResourceManager::GetInstance()->GetObjectImage(ObjectAttribute::ObjectLeaf4Flip));
		if (hBmp != nullptr)
		{
			m_ObjectBitmap.insert({ ObjectAttribute::ObjectLeaf4Flip, new CD2DBitmap(m_pDCRenderTarget, hBmp) });
		}
		hBmp = *(CResourceManager::GetInstance()->GetObjectImage(ObjectAttribute::ObjectRock0Flip));
		if (hBmp != nullptr)
		{
			m_ObjectBitmap.insert({ ObjectAttribute::ObjectRock0Flip, new CD2DBitmap(m_pDCRenderTarget, hBmp) });
		}
		hBmp = *(CResourceManager::GetInstance()->GetObjectImage(ObjectAttribute::ObjectRock1Flip));
		if (hBmp != nullptr)
		{
			m_ObjectBitmap.insert({ ObjectAttribute::ObjectRock1Flip, new CD2DBitmap(m_pDCRenderTarget, hBmp) });
		}
		hBmp = *(CResourceManager::GetInstance()->GetObjectImage(ObjectAttribute::ObjectTree0Flip));
		if (hBmp != nullptr)
		{
			m_ObjectBitmap.insert({ ObjectAttribute::ObjectTree0Flip, new CD2DBitmap(m_pDCRenderTarget, hBmp) });
		}
		hBmp = *(CResourceManager::GetInstance()->GetObjectImage(ObjectAttribute::ObjectTree1Flip));
		if (hBmp != nullptr)
		{
			m_ObjectBitmap.insert({ ObjectAttribute::ObjectTree1Flip, new CD2DBitmap(m_pDCRenderTarget, hBmp) });
		}
		hBmp = *(CResourceManager::GetInstance()->GetObjectImage(ObjectAttribute::ObjectTree2Flip));
		if (hBmp != nullptr)
		{
			m_ObjectBitmap.insert({ ObjectAttribute::ObjectTree2Flip, new CD2DBitmap(m_pDCRenderTarget, hBmp) });
		}

		// etc 1
		hBmp = *(CResourceManager::GetInstance()->GetObjectImage(ObjectAttribute::Heart));
		if (hBmp != nullptr)
		{
			m_ObjectBitmap.insert({ ObjectAttribute::Heart, new CD2DBitmap(m_pDCRenderTarget, hBmp) });
		}
		hBmp = *(CResourceManager::GetInstance()->GetObjectImage(ObjectAttribute::Portal));
		if (hBmp != nullptr)
		{
			m_ObjectBitmap.insert({ ObjectAttribute::Portal, new CD2DBitmap(m_pDCRenderTarget, hBmp) });
		}
		hBmp = *(CResourceManager::GetInstance()->GetObjectImage(ObjectAttribute::ClearEng));
		if (hBmp != nullptr)
		{
			m_ObjectBitmap.insert({ ObjectAttribute::ClearEng, new CD2DBitmap(m_pDCRenderTarget, hBmp) });
		}
		hBmp = *(CResourceManager::GetInstance()->GetObjectImage(ObjectAttribute::ClearKor));
		if (hBmp != nullptr)
		{
			m_ObjectBitmap.insert({ ObjectAttribute::ClearKor, new CD2DBitmap(m_pDCRenderTarget, hBmp) });
		}
		hBmp = *(CResourceManager::GetInstance()->GetObjectImage(ObjectAttribute::ClearName));
		if (hBmp != nullptr)
		{
			m_ObjectBitmap.insert({ ObjectAttribute::ClearName, new CD2DBitmap(m_pDCRenderTarget, hBmp) });
		}

		// Monster
		hBmp = *(CResourceManager::GetInstance()->GetObjectImage(ObjectAttribute::Stage1Monster));
		if (hBmp != nullptr)
		{
			m_ObjectBitmap.insert({ ObjectAttribute::Stage1Monster, new CD2DBitmap(m_pDCRenderTarget, hBmp) });
		}
		hBmp = *(CResourceManager::GetInstance()->GetObjectImage(ObjectAttribute::Stage2Monster0));
		if (hBmp != nullptr)
		{
			m_ObjectBitmap.insert({ ObjectAttribute::Stage2Monster0, new CD2DBitmap(m_pDCRenderTarget, hBmp) });
		}
		hBmp = *(CResourceManager::GetInstance()->GetObjectImage(ObjectAttribute::Stage2Monster1));
		if (hBmp != nullptr)
		{
			m_ObjectBitmap.insert({ ObjectAttribute::Stage2Monster1, new CD2DBitmap(m_pDCRenderTarget, hBmp) });
		}
		hBmp = *(CResourceManager::GetInstance()->GetObjectImage(ObjectAttribute::Stage3Boss));
		if (hBmp != nullptr)
		{
			m_ObjectBitmap.insert({ ObjectAttribute::Stage3Boss, new CD2DBitmap(m_pDCRenderTarget, hBmp) });
		}

		// etc 2
		hBmp = *(CResourceManager::GetInstance()->GetObjectImage(ObjectAttribute::Player));
		if (hBmp != nullptr)
		{
			m_ObjectBitmap.insert({ ObjectAttribute::Player, new CD2DBitmap(m_pDCRenderTarget, hBmp) });
		}
#pragma endregion

		m_loadBitmap = true;
	}
}

// CActionCommanderMapToolView 인쇄

BOOL CActionCommanderMapToolView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 기본적인 준비
	return DoPreparePrinting(pInfo);
}

void CActionCommanderMapToolView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄하기 전에 추가 초기화 작업을 추가합니다.
}

void CActionCommanderMapToolView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄 후 정리 작업을 추가합니다.
}


// CActionCommanderMapToolView 진단

#ifdef _DEBUG
void CActionCommanderMapToolView::AssertValid() const
{
	CView::AssertValid();
}

void CActionCommanderMapToolView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CActionCommanderMapToolDoc* CActionCommanderMapToolView::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CActionCommanderMapToolDoc)));
	return (CActionCommanderMapToolDoc*)m_pDocument;
}
#endif //_DEBUG


// CActionCommanderMapToolView 메시지 처리기


void CActionCommanderMapToolView::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CView::OnMouseMove(nFlags, point);

	m_MousePos.x = point.x + m_ScrollPos.x;
	m_MousePos.y = point.y + m_ScrollPos.y;

	Invalidate(FALSE);
}


afx_msg LRESULT CActionCommanderMapToolView::OnDraw2D(WPARAM wParam, LPARAM lParam)
{
	// lParam을 가져와서 초기화
	m_pDCRenderTarget = (CDCRenderTarget*)lParam;
	ASSERT_KINDOF(CDCRenderTarget, m_pDCRenderTarget);

	InitResource();

	// 스크롤 위치를 얻어와서 수정?
	m_ScrollPos = GetScrollPosition();

	m_pDCRenderTarget->Clear(D2D1::ColorF(D2D1::ColorF::White));

	m_pDCRenderTarget->SetTransform(D2D1::Matrix3x2F::Translation(-m_ScrollPos.x, -m_ScrollPos.y));

	m_pDCRenderTarget->DrawBitmap(m_BG,
		{ 0, 0, static_cast<float>(theApp.m_MapWidth * TILE_SIZE),
			static_cast<float>(theApp.m_MapHeight * TILE_SIZE) });

	// 타일 맵일 때만 그리드 그리기 => 아닐때는 그려주지 않는다.
	if (theApp.isTileCursor)
	{
		// 그리드
		for (int y = 0; y < theApp.m_MapHeight; y++)
		{
			m_pDCRenderTarget->DrawLine({ 0, static_cast<float>(TILE_SIZE * y) },
				{ static_cast<float>(theApp.m_MapWidth * TILE_SIZE),  static_cast<float>(TILE_SIZE * y) }, m_pGridPen);
		}
		for (int x = 0; x < theApp.m_MapWidth; x++)
		{
			m_pDCRenderTarget->DrawLine({ static_cast<float>(TILE_SIZE * x), 0 },
				{ static_cast<float>(TILE_SIZE * x), static_cast<float>(theApp.m_MapHeight * TILE_SIZE) }, m_pGridPen);
		}
	}

	// 타일 그려주기
	DrawTileMap();
	// 콜라이더 그려주기
	DrawCollider();
	// 전경 오브젝트는 타일 이후에 그려주기
	DrawObject();
	// 커서는 항상 마지막에
	DrawCursor();

	m_pRenderTarget->EndDraw();

	return TRUE;
}


void CActionCommanderMapToolView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CView::OnLButtonDown(nFlags, point);
	// 마우스를 클릭했을 때 -> 해당하는 타일을 그려준다..
		// doc에 Vector를 만들어서
	// 버튼 클릭에서는 그 배열만 바꿔주고. 실제로 그리는건 위의 onDraw에서 모든 처리를 해준다.

	CActionCommanderMapToolDoc* pDoc = GetDocument();

	// 타일 맵일 때
	if (theApp.isTileCursor)
	{
		// 클릭한 위치의 마우스 좌표를 그리드 값으로 변경 + 해당 포인트에 스크롤 변수 더해줘서 위치 보정
		CPoint _gridMouse;
		_gridMouse.x = (point.x + m_ScrollPos.x) / TILE_SIZE;
		_gridMouse.y = (point.y + m_ScrollPos.y) / TILE_SIZE;

		// 해당 위치의 벡터 값을 변경
		pDoc->m_TileMap[theApp.m_MapWidth * _gridMouse.y + _gridMouse.x]->tileAttribute = theApp.m_CursorState;
	}
	else
	{
		// none이 아닐 때
		if (theApp.m_ObjCursorState != ObjectAttribute::None)
		{
			if (pDoc->m_PlayerCount > 0 && theApp.m_ObjCursorState == ObjectAttribute::Player)
			{
				// 플레이어가 하나라도 있으면.
				auto iter = pDoc->m_ObjectMap.begin();
				for (iter; iter != pDoc->m_ObjectMap.end(); iter++)
				{
					if ((*iter)->objectAttribute == ObjectAttribute::Player)
						break;
				}
				delete (*iter);
				pDoc->m_ObjectMap.erase(iter);

				pDoc->m_PlayerCount--;
			}

			if (pDoc->m_PortalCount > 0 && theApp.m_ObjCursorState == ObjectAttribute::Portal)
			{
				// 포탈 하나라도 있으면.
				auto iter = pDoc->m_ObjectMap.begin();
				for (iter; iter != pDoc->m_ObjectMap.end(); iter++)
				{
					if ((*iter)->objectAttribute == ObjectAttribute::Portal)
						break;
				}
				delete (*iter);
				pDoc->m_ObjectMap.erase(iter);

				pDoc->m_PortalCount--;
			}

			if (pDoc->m_bClearEng && theApp.m_ObjCursorState == ObjectAttribute::ClearEng)
			{
				// 클리어 하나라도 있으면.
				auto iter = pDoc->m_ObjectMap.begin();
				for (iter; iter != pDoc->m_ObjectMap.end(); iter++)
				{
					if ((*iter)->objectAttribute == ObjectAttribute::ClearEng)
						break;
				}
				delete (*iter);
				pDoc->m_ObjectMap.erase(iter);

				pDoc->m_bClearEng = false;
			}
			if (pDoc->m_bClearKor && theApp.m_ObjCursorState == ObjectAttribute::ClearKor)
			{
				// 클리어 하나라도 있으면.
				auto iter = pDoc->m_ObjectMap.begin();
				for (iter; iter != pDoc->m_ObjectMap.end(); iter++)
				{
					if ((*iter)->objectAttribute == ObjectAttribute::ClearKor)
						break;
				}
				delete (*iter);
				pDoc->m_ObjectMap.erase(iter);

				pDoc->m_bClearKor = false;
			}
			if (pDoc->m_bClearName && theApp.m_ObjCursorState == ObjectAttribute::ClearName)
			{
				// 클리어 하나라도 있으면.
				auto iter = pDoc->m_ObjectMap.begin();
				for (iter; iter != pDoc->m_ObjectMap.end(); iter++)
				{
					if ((*iter)->objectAttribute == ObjectAttribute::ClearName)
						break;
				}
				delete (*iter);
				pDoc->m_ObjectMap.erase(iter);

				pDoc->m_bClearName = false;
			}

			if (pDoc->m_BossCount > 0 && theApp.m_ObjCursorState == ObjectAttribute::Stage3Boss)
			{
				// 포탈 하나라도 있으면.
				auto iter = pDoc->m_ObjectMap.begin();
				for (iter; iter != pDoc->m_ObjectMap.end(); iter++)
				{
					if ((*iter)->objectAttribute == ObjectAttribute::Stage3Boss)
						break;
				}
				delete (*iter);
				pDoc->m_ObjectMap.erase(iter);

				pDoc->m_MonsterCount--;
				pDoc->m_BossCount--;
			}

			// 새로운 오브젝트를 만들어서 vector에 추가.
			Object* tempObj = new Object;
			// 마우스를 클릭한 지점이 center
			tempObj->centerX = point.x + m_ScrollPos.x;
			tempObj->centerY = point.y + m_ScrollPos.y;
			// 비트맵의 크기를 object에 넣어줌.
			tempObj->bmpWidth = m_ObjectBitmap.at(theApp.m_ObjCursorState)->GetSize().width;
			tempObj->bmpHeight = m_ObjectBitmap.at(theApp.m_ObjCursorState)->GetSize().height;
			// 해당 크기를 바탕으로 좌측상단을 구해준다.
			tempObj->left = tempObj->centerX - (tempObj->bmpWidth / 2);
			tempObj->top = tempObj->centerY - (tempObj->bmpHeight / 2);
			tempObj->objectAttribute = theApp.m_ObjCursorState;

			// 벡터에 추가
			pDoc->m_ObjectMap.push_back(tempObj);

			pDoc->m_ObjectCount = pDoc->m_ObjectMap.size();

			// obj의 종류에 따라.. Count를 다르게
			switch (theApp.m_ObjCursorState)
			{
				case ObjectAttribute::Stage1Monster:
				case ObjectAttribute::Stage2Monster0:
				case ObjectAttribute::Stage2Monster1:
					pDoc->m_MonsterCount++;
					break;
				case ObjectAttribute::Stage3Boss:
				{
					pDoc->m_MonsterCount++;
					pDoc->m_BossCount++;
				}
				break;

				case ObjectAttribute::Player:
					pDoc->m_PlayerCount++;
					break;

				case ObjectAttribute::Portal:
					pDoc->m_PortalCount++;
					break;

				case ObjectAttribute::ClearEng:
					pDoc->m_bClearEng = true;
					break;

				case ObjectAttribute::ClearKor:
					pDoc->m_bClearKor = true;
					break;
				case ObjectAttribute::ClearName:
					pDoc->m_bClearName = true;
					break;
			}

		}
		else
		{
			// none 범위 안에 center가 있는 친구들 list에서 삭제..!
			// 범위를 찾기 위한 max min 지정
			float minX = point.x - TILE_SIZE / 2 + m_ScrollPos.x;
			float maxX = point.x + TILE_SIZE / 2 + m_ScrollPos.x;
			float minY = point.y - TILE_SIZE / 2 + m_ScrollPos.y;
			float maxY = point.y + TILE_SIZE / 2 + m_ScrollPos.y;

			auto iter = pDoc->m_ObjectMap.begin();
			bool isFind = false;
			for (iter; iter != pDoc->m_ObjectMap.end(); iter++)
			{
				if (
					((*iter)->centerX >= minX) && ((*iter)->centerX <= maxX)
					&&
					((*iter)->centerY >= minY) && ((*iter)->centerY <= maxY)
					)
				{
					// 없애기 전에 스위치.
					switch ((*iter)->objectAttribute)
					{
						case ObjectAttribute::Stage1Monster:
						case ObjectAttribute::Stage2Monster0:
						case ObjectAttribute::Stage2Monster1:
							pDoc->m_MonsterCount--;
							break;

						case ObjectAttribute::Player:
							pDoc->m_PlayerCount--;
							break;

						case ObjectAttribute::Portal:
							pDoc->m_PortalCount--;
							break;
					}
					isFind = true;
					break;
				}
			}
			if (isFind)
			{
				delete (*iter);
				pDoc->m_ObjectMap.erase(iter);
			}

			pDoc->m_ObjectCount = pDoc->m_ObjectMap.size();
		}
	}

	pDoc->UpdateAllViews(this);
}


void CActionCommanderMapToolView::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CScrollView::OnLButtonUp(nFlags, point);

	// 기본적으로 드래그 확인 용도로만 사용.
}

void CActionCommanderMapToolView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

	// 스크롤바 사이즈 지정.
	CSize imgSize;
	imgSize.cx = theApp.m_MapWidth * TILE_SIZE;
	imgSize.cy = theApp.m_MapHeight * TILE_SIZE;
	SetScrollSizes(MM_TEXT, imgSize);

	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

}


void CActionCommanderMapToolView::OnDestroy()
{
	CScrollView::OnDestroy();

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.

	if (m_pGridPen != nullptr)
	{
		m_pGridPen->Get()->Release();
		m_pGridPen = nullptr;
	}
	if (m_pRedPen != nullptr)
	{
		m_pRedPen->Get()->Release();
		m_pRedPen = nullptr;
	}
}


void CActionCommanderMapToolView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CScrollView::OnKeyDown(nChar, nRepCnt, nFlags);

	m_ScrollPos = GetScrollPosition();

	switch (nChar)
	{
		case VK_LEFT:
		case 0x41:
			SendMessage(WM_HSCROLL, SB_LINELEFT, 0);
			break;
		case VK_RIGHT:
		case 0x44:
			SendMessage(WM_HSCROLL, SB_LINERIGHT, 0);
			break;
		case VK_UP:
		case 0x57:
			SendMessage(WM_VSCROLL, SB_LINEUP, 0);
			break;
		case VK_DOWN:
		case 0x53:
			SendMessage(WM_VSCROLL, SB_LINEDOWN, 0);
			break;
	}
}

