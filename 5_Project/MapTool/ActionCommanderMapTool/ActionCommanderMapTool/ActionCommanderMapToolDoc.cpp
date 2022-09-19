
// ActionCommanderMapToolDoc.cpp: CActionCommanderMapToolDoc 클래스의 구현
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "ActionCommanderMapTool.h"
#endif

#include "ActionCommanderMapToolDoc.h"
#include "MapSizeDlg.h"
#include <propkey.h>
#include <string>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CActionCommanderMapToolDoc

IMPLEMENT_DYNCREATE(CActionCommanderMapToolDoc, CDocument)

BEGIN_MESSAGE_MAP(CActionCommanderMapToolDoc, CDocument)
END_MESSAGE_MAP()

// CActionCommanderMapToolDoc 생성/소멸

CActionCommanderMapToolDoc::CActionCommanderMapToolDoc() noexcept
{
	// TODO: 여기에 일회성 생성 코드를 추가합니다.

}

CActionCommanderMapToolDoc::~CActionCommanderMapToolDoc()
{
}

BOOL CActionCommanderMapToolDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: 여기에 재초기화 코드를 추가합니다.
	// SDI 문서는 이 문서를 다시 사용합니다.
	MapSizeDlg mapSizeDlg;
	mapSizeDlg.DoModal();

	// 타일맵, 오브젝트 초기화

	for (auto& i : m_TileMap)
	{
		delete i;
	}
	for (auto& i : m_ObjectMap)
	{
		delete i;
	}

	m_TileMap.clear();
	m_ObjectMap.clear();
	m_Colliders.clear();

	// 그리드 크기에 따른 벡터 초기화
		// 여기서 타일의 left, top을 실제 스크린 좌표로 치환해준다.
		// x길이만큼 y번 채운다
		// x행 y열로 가고 싶으면 => width에 y를 곱한 뒤 x를 더함.
		// y개수 만큼 세로를 가고. 가고싶은 x만큼 가로를 간다.
	for (int y = 0; y < theApp.m_MapHeight; y++)
	{
		for (int x = 0; x < theApp.m_MapWidth; x++)
		{
			Tile* temp = new Tile();
			temp->left = x * TILE_SIZE;
			temp->top = y * TILE_SIZE;
			temp->tileAttribute = TileAttribute::None;
			m_TileMap.push_back(temp);
		}
	}

	// 초기화
	m_ObjectCount = 0;
	m_MonsterCount = 0;
	m_PlayerCount = 0;
	m_PortalCount = 0;
	m_BossCount = 0;

	m_bClearEng = false;
	m_bClearKor = false;
	m_bClearName = false;


	return TRUE;
}




// CActionCommanderMapToolDoc serialization

void CActionCommanderMapToolDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: 여기에 저장 코드를 추가합니다.

		// Json에 넣기 전에 콜라이더 생성
		// 콜라이더 벡터는 이때만 사용하는 것으로.
		SetCollider();

		// Json으로 한번 만들어보자..
		// 최상위 저장 json
		rapidjson::GenericDocument<rapidjson::UTF8<>> Json;
		Json.SetObject();

		// 맵 데이터를 저장할 MapData
		rapidjson::GenericValue<rapidjson::UTF8<>> MapData;
		MapData.SetObject();

		// 맵 사이즈를 저장할 SizeArr
			// 배열이기 때문에 set array
		rapidjson::GenericValue<rapidjson::UTF8<>> SizeArr;
		SizeArr.SetArray();

		// 키 값으로 사용할 _key
		rapidjson::GenericValue<rapidjson::UTF8<>> _key;

		// 키 설정 => 값 설정 => Add 멤버 순으로 제이슨에 기록

		// 맵 사이즈 x, y 순으로 기록
		_key.SetString("MapSize");
		SizeArr.PushBack(theApp.m_MapWidth, Json.GetAllocator());
		SizeArr.PushBack(theApp.m_MapHeight, Json.GetAllocator());
		MapData.AddMember(_key, SizeArr, Json.GetAllocator());

		// 타일 맵, 오브젝트 맵 기록
		rapidjson::GenericValue<rapidjson::UTF8<>> TileMap, Colliders, ObjectMap;
		TileMap.SetArray();
		Colliders.SetArray();
		ObjectMap.SetArray();

		// 타일 맵 저장
			// 타일 하나의 배열 => [left, top, TileAttribute]
			// left, top은 타일의 좌측 상단 좌표
		for (int i = 0; i < m_TileMap.size(); i++)
		{
			rapidjson::GenericValue<rapidjson::UTF8<>> TileData;
			TileData.SetArray();

			Tile* tile = m_TileMap[i];

			TileData.PushBack(tile->left, Json.GetAllocator());
			TileData.PushBack(tile->top, Json.GetAllocator());
			TileData.PushBack(static_cast<int>(tile->tileAttribute), Json.GetAllocator());

			TileMap.PushBack(TileData, Json.GetAllocator());
		}

		_key.SetString("TileMapData");
		MapData.AddMember(_key, TileMap, Json.GetAllocator());

		// Collider Map Save
		for (auto iter = m_Colliders.begin(); iter != m_Colliders.end(); iter++)
		{
			rapidjson::GenericValue<rapidjson::UTF8<>> ColliderData;
			ColliderData.SetArray();

			CRect temp = *iter;
			ColliderData.PushBack(temp.left, Json.GetAllocator());
			ColliderData.PushBack(temp.top, Json.GetAllocator());
			ColliderData.PushBack(temp.right, Json.GetAllocator());
			ColliderData.PushBack(temp.bottom, Json.GetAllocator());

			Colliders.PushBack(ColliderData, Json.GetAllocator());
		}

		_key.SetString("Colliders");
		MapData.AddMember(_key, Colliders, Json.GetAllocator());

		_key.SetString("TileSize");
		MapData.AddMember(_key, 64, Json.GetAllocator());
		// 오브젝트 맵 저장
			// 오브젝트 하나하나의 배열 => [left, top, centerX, centerY, bmpWidth, bmpHeight, objectAtrribute]
			// left top은 실제 좌표 위치
		for (auto it = m_ObjectMap.begin(); it != m_ObjectMap.end(); it++)
		{
			rapidjson::GenericValue<rapidjson::UTF8<>> ObjectData;
			ObjectData.SetArray();

			Object* obj = *it;

			ObjectData.PushBack(obj->left, Json.GetAllocator());
			ObjectData.PushBack(obj->top, Json.GetAllocator());
			ObjectData.PushBack(obj->centerX, Json.GetAllocator());
			ObjectData.PushBack(obj->centerY, Json.GetAllocator());
			ObjectData.PushBack(obj->bmpWidth, Json.GetAllocator());
			ObjectData.PushBack(obj->bmpHeight, Json.GetAllocator());
			ObjectData.PushBack(static_cast<int>(obj->objectAttribute), Json.GetAllocator());

			ObjectMap.PushBack(ObjectData, Json.GetAllocator());
		}

		_key.SetString("ObjectMapData");
		MapData.AddMember(_key, ObjectMap, Json.GetAllocator());

		_key.SetString("MonsterCount");
		MapData.AddMember(_key, m_MonsterCount, Json.GetAllocator());

		_key.SetString("PlayerCount");
		MapData.AddMember(_key, m_PlayerCount, Json.GetAllocator());

		_key.SetString("PortalCount");
		MapData.AddMember(_key, m_PortalCount, Json.GetAllocator());

		_key.SetString("BossCount");
		MapData.AddMember(_key, m_BossCount, Json.GetAllocator());

		_key.SetString("ClearEng");
		MapData.AddMember(_key, m_bClearEng, Json.GetAllocator());

		_key.SetString("ClearKor");
		MapData.AddMember(_key, m_bClearKor, Json.GetAllocator());

		_key.SetString("ClearName");
		MapData.AddMember(_key, m_bClearName, Json.GetAllocator());

		// 파일 이름
		CString fileName = ar.m_strFileName.GetBuffer();
		fileName = fileName.Right(fileName.GetLength() - fileName.ReverseFind('\\') - 1);
		fileName = fileName.Left(fileName.ReverseFind('.'));

		std::string temp = fileName.GetBuffer();

		// Json에 저장
		_key.SetString(temp.c_str(), fileName.GetLength());
		Json.AddMember(_key, MapData, Json.GetAllocator());

		// json파일 저장
		rapidjson::GenericStringBuffer<rapidjson::UTF8<>> strbuf;
		rapidjson::Writer<rapidjson::GenericStringBuffer<rapidjson::UTF8<>>, rapidjson::UTF8<>, rapidjson::UTF8<>> writer(strbuf);
		Json.Accept(writer);

		CString str = CString(strbuf.GetString());

		ar.WriteString(str);

	}
	else
	{
		// TODO: 여기에 로딩 코드를 추가합니다.

		// 기본적인 초기화 작업
		// 타일맵, 오브젝트 초기화
		for (auto& i : m_TileMap)
		{
			delete i;
		}
		for (auto& i : m_ObjectMap)
		{
			delete i;
		}

		m_TileMap.clear();
		m_ObjectMap.clear();
		m_Colliders.clear();

		// json을 불러온다 
		CString str;
		ar.ReadString(str);

		rapidjson::GenericDocument<rapidjson::UTF8<>> Json;
		Json.Parse(str.GetBuffer());
		assert(Json.IsObject());

		InitJson(Json);

		m_ObjectCount = m_ObjectMap.size();
	}
}

// Json의 정보를 실제 화면에 적용시키는 과정.
void CActionCommanderMapToolDoc::InitJson(const rapidjson::GenericValue<rapidjson::UTF8<>>& Json)
{
	for (auto iter = Json.MemberBegin(); iter != Json.MemberEnd(); iter++)
	{
		std::string keyName = iter->name.GetString();

		switch (iter->value.GetType())
		{
			// obj타입은 한번 더 돌려준다.
			case rapidjson::kObjectType:
			{
				InitJson(iter->value.GetObj());
				break;
			}
			// Array 타입의 경우.. 안에서 돌면서 값들을 넣어준다.
			case rapidjson::kArrayType:
			{
				// 현재 이터레이터가 가리키는 지점의 시작점을 가져온다.
				auto iter2 = iter->value.GetArray().Begin();

				if (keyName == "MapSize")
				{
					theApp.m_MapWidth = iter2++->GetInt();
					theApp.m_MapHeight = iter2->GetInt();

				}
				else if (keyName == "TileMapData")
				{
					// iter2는 배열의 시작점을 가리키는 포인터 => 이게 현재 배열의 마지막이 아닐 때 까지 반복
					// 새로운 타일 맵을 만들어서 pushback 해준다.
					for (iter2; iter2 != iter->value.GetArray().End(); iter2++)
					{
						// TileMapData는 내부에도 배열로 이루어져 있기 때문에 그것을 가리키는 iter3이 필요
						auto iter3 = iter2->Begin();

						Tile* temp = new Tile();
						temp->left = iter3++->GetInt();
						temp->top = iter3++->GetInt();
						temp->tileAttribute = static_cast<TileAttribute>(iter3->GetInt());

						m_TileMap.push_back(temp);
					}
				}
				else if(keyName == "Colliders")
				{
					for(iter2; iter2 != iter->value.GetArray().End(); iter2++)
					{
						auto iter3 = iter2->Begin();

						CRect temp;
						temp.left = iter3++->GetInt();
						temp.top = iter3++->GetInt();
						temp.right = iter3++->GetInt();
						temp.bottom = iter3++->GetInt();

						m_Colliders.push_back(temp);
					}
				}
				else if (keyName == "ObjectMapData")
				{
					// 위와 비슷하게 반복. 새로운 Object를 만들어서 list에 넣어준다.
					for (iter2; iter2 != iter->value.GetArray().End(); iter2++)
					{
						auto iter3 = iter2->Begin();

						Object* tempObj = new Object;
						tempObj->left = iter3++->GetInt();
						tempObj->top = iter3++->GetInt();
						tempObj->centerX = iter3++->GetInt();
						tempObj->centerY = iter3++->GetInt();
						tempObj->bmpWidth = iter3++->GetFloat();
						tempObj->bmpHeight = iter3++->GetFloat();
						tempObj->objectAttribute = static_cast<ObjectAttribute>(iter3->GetInt());

						m_ObjectMap.push_back(tempObj);
					}

				}
				break;
			}

			case rapidjson::kNumberType:
			{
				if (keyName == "MonsterCount")
				{
					m_MonsterCount = iter->value.GetInt();

				}
				else if (keyName == "PlayerCount")
				{
					m_PlayerCount = iter->value.GetInt();

				}
				else if (keyName == "PortalCount")
				{
					m_PortalCount = iter->value.GetInt();
				}
				else if (keyName == "BossCount")
				{
					m_BossCount = iter->value.GetInt();
				}
				break;
			}

		case rapidjson::kTrueType:
				{
					if (keyName == "ClearEng")
					{
						m_bClearEng = iter->value.GetBool();
					}
					else if(keyName == "ClearKor")
					{
						m_bClearKor = iter->value.GetBool();
					}
					else if(keyName == "ClearName")
					{
						m_bClearName = iter->value.GetBool();
					}
					break;
				}

		}
	}
}

// 콜라이더 벡터에 콜라이더를 넣는 역할
void CActionCommanderMapToolDoc::SetCollider()
{
	// 사각형 여부를 확인하기 위한 플래그 배열..
	std::vector<int> _rectFlag;
	// 땅 타일의 속성을 파악하기 위한 Ground배열
	std::vector<int> _tileAttributeArr;

	// 리스트 한번 지워주기 => 초기화를 해줘야 예전 데이터가 사라진다.
	m_Colliders.clear();

	for (int y = 0; y < theApp.m_MapHeight; y++)
	{
		for (int x = 0; x < theApp.m_MapWidth; x++)
		{
			// 해당 위치가 none이 아니면. 땅의 속성을 부여
			if (m_TileMap[y * theApp.m_MapWidth + x]->tileAttribute != TileAttribute::None)
			{
				_tileAttributeArr.push_back(static_cast<int>(m_TileMap[y * theApp.m_MapWidth + x]->tileAttribute) >> 8);
			}
			else
			{
				_tileAttributeArr.push_back(0);
			}

			_rectFlag.push_back(0);
		}
	}

	// 가로로 묶고, 세로로 묶는다.
	// 가로로 먼저 묶어본다.
		// for문을 돌면서 시작점을 찾고, 옆의 인덱스가 같거나, 맵의 끝이 아니면 계속 옆으로 간다.
		// 시작점의 x그리드 => left, 최종 지점의 x그리드 + 1 => right.
		// 시작점의 y => top. 최종 지점의 y그리드 +1 => bottom // 일단 가로로만 묶기 때문에 그냥 시작지점 +1 이라 생각해도 무방
	for (int y = 0; y < theApp.m_MapHeight; y++)
	{
		for (int x = 0; x < theApp.m_MapWidth; x++)
		{
			// 시작 점을 찾는다.
				// 해당 지점이 비어있지 않으면서, 사각형 체크가 되어있지 않을때
			if (_tileAttributeArr[y * theApp.m_MapWidth + x] != 0 && _rectFlag[y * theApp.m_MapWidth + x] == 0)
			{
				CRect temp = { x, y, x + 1, y + 1 };
				// 최종 지점을 찾는다.
					// 현재 x의 위치가 끝 부분이면. _rectFlag만 체크 해준다
				if (x + 1 == theApp.m_MapWidth)
				{
					_rectFlag[y * theApp.m_MapWidth + x] = 1;
				}
				else
				{
					// while문을 돌면서 체크해준다.
					int count = x;
					while (
						_tileAttributeArr[y * theApp.m_MapWidth + count]
						== _tileAttributeArr[y * theApp.m_MapWidth + count + 1])
					{
						// 현재 진입한 지점의 _rectFlag를 1로 바꿔준다.
						_rectFlag[y * theApp.m_MapWidth + count] = 1;

						count++;
						// 추가한 count 값이 맵 끝이면, 종료시켜준다.
						if (count + 1 == theApp.m_MapWidth)
						{
							break;
						}
					}

					// count의 위치는 맵의 끝점 or 달라지는 지점
					_rectFlag[y * theApp.m_MapWidth + count] = 1;
					// CRect를 바꿔준다.
					temp.right = count + 1;
				}

				// 콜라이더 사이즈 계산 후 vector에 넣어주기 => 얇은 플랫폼이 아니면 64를 곱해준다.
				if (_tileAttributeArr[y * theApp.m_MapWidth + x] != 3)
				{
					temp.left *= TILE_SIZE;
					temp.top *= TILE_SIZE;
					temp.right *= TILE_SIZE;
					temp.bottom *= TILE_SIZE;

					m_Colliders.push_back(temp);
				}
				else
				{
					temp.left *= TILE_SIZE;
					temp.top *= TILE_SIZE;
					temp.right *= TILE_SIZE;
					temp.bottom = (temp.bottom * TILE_SIZE) - (TILE_SIZE / 2);

					m_Colliders.push_back(temp);
				}
			}
		}
	}

	// 세로로 묶어 준다.
	// list의 시작부터 전체 비교..
		// 현재 검사하는 Rect의 LeftBottom 값과 검사하는 Rect의 LeftTop 값이 같으면서
		// 현재 검사하는 Rect의 RightBottom 값과 검사하는 Rect의 RightTop 값이 같으면
		// 두 Rect의 LeftTop과 RightBottom을 합한 Rect를 만들고 원래 rect는 지워준다.
			// 검사 순서상 y축에서 위에 있는 콜라이더가 먼저 list에 들어가기 때문에 가능
		// 기준 Rect의 Left, Top. 검사 Rect의 Right Bottom을 가져온다.
	
	for(auto iter = m_Colliders.begin(); iter != m_Colliders.end(); iter++)
	{
		// 시작부터 끝까지 재검사를 할 iter2
		for(auto iter2 = m_Colliders.begin(); iter2 != m_Colliders.end(); iter2++)
		{
			if (
				((*iter).left == (*iter2).left && (*iter).bottom == (*iter2).top)
				&&
				((*iter).right == (*iter2).right && (*iter).bottom == (*iter2).top)
				)
			{
				CRect temp = { (*iter).left, (*iter).top, (*iter2).right, (*iter2).bottom };
				m_Colliders.push_back(temp);

				iter = m_Colliders.erase(iter);
				if(iter == iter2)
				{
					iter2 = m_Colliders.erase(iter2);
					iter = iter2;
				}
				else
				{
					iter2 = m_Colliders.erase(iter2);
				}
			}
		}
	}
}

#ifdef SHARED_HANDLERS

// 축소판 그림을 지원합니다.
void CActionCommanderMapToolDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// 문서의 데이터를 그리려면 이 코드를 수정하십시오.
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT)GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// 검색 처리기를 지원합니다.
void CActionCommanderMapToolDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// 문서의 데이터에서 검색 콘텐츠를 설정합니다.
	// 콘텐츠 부분은 ";"로 구분되어야 합니다.

	// 예: strSearchContent = _T("point;rectangle;circle;ole object;");
	SetSearchContent(strSearchContent);
}

void CActionCommanderMapToolDoc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl* pChunk = nullptr;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != nullptr)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// CActionCommanderMapToolDoc 진단

#ifdef _DEBUG
void CActionCommanderMapToolDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CActionCommanderMapToolDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

// CActionCommanderMapToolDoc 명령
