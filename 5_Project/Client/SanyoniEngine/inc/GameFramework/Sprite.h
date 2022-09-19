#pragma once

#include <rapidjson/document.h>

#include "SanyoniEngine_types.h"

namespace SanyoniEngine
{
	class Sprite;
	class SpriteSheet;

	struct SpriteInfo
	{
		Bitmap* const bitmap;
		Rect rect;
		Vector2 pivot;
	};

	class ISprite abstract
	{
	public:
		virtual const SpriteInfo& GetSpriteInfo() const abstract;
	};

	class Sprite final :
		public ISprite
	{
	public:
		Sprite(SpriteSheet* _sheet, const tstring& _name, const Rect& _rect, const Vector2& _pivot = Vector2::Center);

		const SpriteInfo& GetSpriteInfo() const override;

	private:
		tstring m_Name;

		SpriteSheet* const m_Sheet;

		SpriteInfo m_Info;

	public:
		tstring GetName() const { return m_Name; }
	};

	class SpriteSheet final
		: public ISprite
	{
	public:
		~SpriteSheet();

		Sprite* Slice(const tstring& _name, const Rect& _rect, const Vector2& _pivot);

		const SpriteInfo& GetSpriteInfo() const override;

	private:
		SpriteSheet(const tstring& _bitmapName, Bitmap* _bitmap, const Vector2& _pivot = Vector2::Center);

	private:
		const tstring m_BitmapName;

		Bitmap* const m_Bitmap;

		SpriteInfo m_Info;

		std::map<tstring, Sprite*> m_SpritesMap;

	public:
		inline tstring GetBitmapFileName() const { return m_BitmapName; }

		inline Bitmap* GetBitmap() const { return m_Bitmap; }

		Sprite* GetSpriteByName(tstring _name) const;

	public:
		static HRESULT LoadFromFile(SpriteSheet** _outSheet, const tstring& _fileUri);

		static HRESULT LoadFromJsonFile(SpriteSheet** _outSheet, const tstring& _jsonFileUri);
	};
}
