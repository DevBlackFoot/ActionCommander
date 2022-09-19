#pragma once

// Tile과 Object의 속성을 나타내기위한 Attribute클래스
// 땅 - 0x01xx / 점프 할 수 없는 플랫폼 - 0x02xx, 점프 할 수 있는 플랫폼 - 0x03xx
enum class TileAttribute
{
	GroundLeftTop			=	0x0101,
	GroundTop				=	0x0102,
	GroundRightTop			=	0x0103,
	GroundLeft				=	0x0104,
	GroundCenter			=	0x0105,
	GroundRight				=	0x0106,
	GroundLeftBottom		=	0x0107,
	GroundBottom			=	0x0108,
	GroundRightBottom		=	0x0109,
	GroundDiaLeftTop		=	0x0110,
	GroundDiaRightTop		=	0x0111,
	GroundDiaLeftBottom		=	0x0112,
	GroundDiaRightBottom	=	0x0113,

	NotJumpPlatLeft		=	0x0201,
	NotJumpPlatCenter	=	0x0202,
	NotJumpPlatRight	=	0x0203,

	JumpPlatLeft		=	0x0301,
	JumpPlatCenter		=	0x0302,
	JumpPlatRight		=	0x0303,

	None				=	0xFFFF,
};

// 전경 - 0x01xx / 상호 작용(하트, 포탈) - 0x02xx, 몬스터 - 0x03xx, 플레이어 - 0x04xx
enum class ObjectAttribute
{
	ObjectLeaf0			=	0x0101,
	ObjectLeaf1			=	0x0102,
	ObjectLeaf2			=	0x0103,
	ObjectLeaf3			=	0x0104,
	ObjectLeaf4			=	0x0105,
	ObjectRock0			=	0x0106,
	ObjectSign0			=	0x0107,
	ObjectTree0			=	0x0108,
	ObjectTree1			=	0x0109,
	// Flip 오브젝트는 이 이후 붙이기.
	ObjectLeaf0Flip		=	0x0110,
	ObjectLeaf1Flip		=	0x0111,
	ObjectLeaf2Flip		=	0x0112,
	ObjectLeaf3Flip		=	0x0113,
	ObjectLeaf4Flip		=	0x0114,
	ObjectRock0Flip		=	0x0115,
	ObjectTree0Flip		=	0x0116,
	ObjectTree1Flip		=	0x0117,
	// 로드에서 꼬이지 않기 위해서..
	ObjectTree2			=	0x0118,
	ObjectTree2Flip		=	0x0119,
	ObjectRock1			=	0x0120,
	ObjectRock1Flip		=	0x0121,
	ObjectSign1			=	0x0122,
	ObjectSign2			=	0x0123,
	ObjectSign3			=	0x0124,

	Heart				=	0x0201,
	Portal				=	0x0202,

	Stage1Monster		=	0x0301,
	Stage2Monster0		=	0x0302,
	Stage2Monster1		=	0x0303,
	Stage3Boss			=	0x0304,

	Player				=	0x0401,

	None				=	0xFFFF,

};

// 타일과 맵에 대한 구조체 정보.

// 타일 하나하나의 정보를 가지고 있는 구조체
// 좌측 상단 정보 (left, top)
	// 해당 위치 부터 그림을 그린다.
	// 기본적으로 타일맵은 64*64 사이즈이기 때문에 해당 위치부터 그림을 그린다.
// 타일의 속성값을 나타내는 attribute
// 해당 정보는 이후 클라이언트에 붙을 때 컴포넌트화 된다.
	// 기본적으로 스프라이트 렌더러가 붙어서 이미지를 처리하고
	// 이후 이들을 엮어서 콜라이더를 생성한다. => 콜라이더 오브젝트의 속성을 파악한다.
struct Tile
{
	int left, top;
	TileAttribute tileAttribute;
};

// 몬스터와 같이 타일과 관련 없는 오브젝트와 관련된 구조체
	// 좌측 상단 정보(PosX, PosY) => 데이터 파싱을 위한 정보
		// 해당 위치에서 부터 비트맵 사이즈만큼 그림을 그린다.
		// 타일의 위에 나타나도 상관 없는 오브젝트.
		// 몬스터라면 읽을 때 다른 방법으로 처리를 해주어야할 수도.
	// 그림을 위한 Center와 비트맵 길이도 있으면 좋지앟을까.
	// 몬스터의 속성 값을 나타내는 attribute
struct Object
{
	int left, top;
	int centerX, centerY;
	float bmpWidth, bmpHeight;
	ObjectAttribute objectAttribute;
};



// 타일 맵과 관련된 구조체
	// App에 저장되어 출력되는 구조체
	// 타일 맵과 오브젝트 vector로 이루어져있다고 가정.
	// 해당 벡터를 읽으면 그 씬의 타일과 오브젝트를 읽을 수 있다.
// 맵 전체의 크기(mapX, mapY)
// 타일 벡터와 오브젝트 벡터 => 이 정보는 앱에 public으로 가지고 있도록 해도 상관 없을 듯 하다.
