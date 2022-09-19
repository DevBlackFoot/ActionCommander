#pragma once

#include <rapidjson/document.h>

#include "SanyoniEngine_types.h"
#include "SingletonInstanceBase.h"

#include "EDWrite.h"

namespace SanyoniEngine
{
	class FontCollection;
	class TextFormat;
	class AnimationClip;

	class Resources final
	{
		DECLARE_SINGLETON_CLASS(Resources)

	public:
		static rapidjson::Document LoadJsonFromFile(const tstring& _filePath);

		HRESULT LoadBitmapFromFile(Bitmap** _outBitmap, const tstring _fileUri);

		HRESULT LoadBitmapFromResource(Bitmap** _outBitmap, const tstring _resourceName,
		                               const tstring _resourceType);

		SpriteSheet* LoadSpriteSheetFromFile(const tstring& _fileUri);

		SpriteSheet* LoadSpriteSheetFromJsonFile(const tstring& _jsonFileUri);

		AnimationClip* LoadAnimationClipFromJsonFile(const tstring& _jsonFileUri);

		FontCollection* LoadFontCollectionFromFile(const tstring _fileUri);

		TextFormat* CreateTextFormat(FontCollection* _fontCollection,
		                             float _fontSize = 12.f,
		                             EFontWeight _fontWeight = EFontWeight::NORMAL,
		                             EFontStyle _fontStyle = EFontStyle::NORMAL,
		                             EFontStretch _fontStretch = EFontStretch::NORMAL,
		                             tstring _localeName = TEXT(""));

		ISprite* GetSpriteByName(const tstring& _name);

	private:
		std::unordered_map<tstring, Bitmap*> m_BitmapFromFileMap;

		std::unordered_map<tstring, Bitmap*> m_BitmapFromResourceMap;

		std::unordered_map<tstring, SpriteSheet*> m_SpriteSheetMap;

		std::unordered_map<tstring, tstring> m_JsonSpriteSheetMap;

		std::unordered_map<tstring, AnimationClip*> m_AnimationClipMap;

		std::unordered_map<tstring, FontCollection*> m_FontCollectionMap;

	};
}
