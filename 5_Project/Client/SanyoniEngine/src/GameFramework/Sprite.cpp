#include "SanyoniEngine_pch.h"
#include "Sprite.h"

#include "Resources.h"

using namespace SanyoniEngine;

Sprite::Sprite(SpriteSheet* _sheet, const tstring& _name, const Rect& _rect, const Vector2& _pivot) :
	m_Sheet(_sheet),
	m_Name(_name),
	m_Info{m_Sheet->GetBitmap(), _rect, _pivot}
{
}

const SpriteInfo& Sprite::GetSpriteInfo() const
{
	return m_Info;
}

SpriteSheet::~SpriteSheet()
{
	// 이 SpriteSheet에 속해있는 모든 Sprite 객체를 삭제합니다.
	for (const auto _pair : m_SpritesMap)
	{
		delete _pair.second;
	}

	// 비트맵을 해제합니다.
	m_Bitmap->Release();
}

Sprite* SpriteSheet::Slice(const tstring& _name, const Rect& _rect, const Vector2& _pivot)
{
	// 스프라이트의 시작점과 끝점 값이 유효한지 확인합니다.
	assert(_rect.left < _rect.right && _rect.top < _rect.bottom);

	Sprite* _newSprite = new Sprite(this, _name, _rect, _pivot);

	m_SpritesMap.emplace(_name, _newSprite);

	return _newSprite;
}

const SpriteInfo& SpriteSheet::GetSpriteInfo() const
{
	return m_Info;
}

SpriteSheet::SpriteSheet(const tstring& _bitmapName, Bitmap* _bitmap, const Vector2& _pivot) :
	m_BitmapName(_bitmapName),
	m_Bitmap(_bitmap),
	m_Info{_bitmap, {0, 0, _bitmap->GetSize().width, _bitmap->GetSize().height}, _pivot}
{
}

Sprite* SpriteSheet::GetSpriteByName(tstring _name) const
{
	// 해당 이름의 스프라이트가 있는지 검사합니다.
	const auto _iter = m_SpritesMap.find(_name);
	if (_iter == m_SpritesMap.end())
		return nullptr;

	return _iter->second;
}

HRESULT SpriteSheet::LoadFromFile(SpriteSheet** _outSheet, const tstring& _fileUri)
{
	assert(_outSheet != nullptr);

	Bitmap* _bitmap;
	const HRESULT _hr = Resources::Instance.LoadBitmapFromFile(&_bitmap, _fileUri);

	*_outSheet = new SpriteSheet(_fileUri, _bitmap);
	return _hr;
}

HRESULT SpriteSheet::LoadFromJsonFile(SpriteSheet** _outSheet, const tstring& _jsonFileUri)
{
	assert(_outSheet != nullptr);

	const auto _json = Resources::LoadJsonFromFile(_jsonFileUri);

	assert(_json.HasMember("sheetName"));
	assert(_json.HasMember("spriteInfos"));

	const tstring _bitmapPath = StringHelper::ToWString(_json["sheetName"].GetString());

	Bitmap* _bitmap;
	HRESULT _hr;

	if ((_hr = Resources::Instance.LoadBitmapFromFile(&_bitmap, _bitmapPath)) != S_OK)
	{
		assert(false);
		return _hr;
	}

	SpriteSheet* _newSheet = new SpriteSheet(_bitmapPath, _bitmap);

	const auto _spriteInfos = _json["spriteInfos"].GetArray();

	for (const auto& _sprite : _spriteInfos)
	{
		assert(_sprite.HasMember("name"));
		assert(_sprite.HasMember("rect"));
		assert(_sprite.HasMember("pivot"));


		const tstring _spriteName = StringHelper::ToWString(_sprite["name"].GetString());

		const Rect _spriteRect{
			static_cast<float>(_sprite["rect"]["left"].GetInt()),
			static_cast<float>(_sprite["rect"]["top"].GetInt()),
			static_cast<float>(_sprite["rect"]["right"].GetInt()),
			static_cast<float>(_sprite["rect"]["bottom"].GetInt())
		};

		const Vector2 _spritePivot{
			_sprite["pivot"]["x"].GetFloat(),
			_sprite["pivot"]["y"].GetFloat()
		};


		_newSheet->Slice(_spriteName, _spriteRect, _spritePivot);
	}

	*_outSheet = _newSheet;
	return S_OK;
}
