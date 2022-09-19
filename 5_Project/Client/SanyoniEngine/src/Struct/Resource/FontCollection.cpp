#include "SanyoniEngine_pch.h"
#include "FontCollection.h"

#include "Helper.h"

#include <atlbase.h>

using namespace SanyoniEngine;

HRESULT FontCollection::Create(IDWriteFactory7* _dwriteFactory, tstring _fontFilePath, FontCollection** _outFontCollection)
{
	*_outFontCollection = nullptr;

	IDWriteFontSetBuilder2* _fontSetBuilder;
	IDWriteFontFile* _fontFile;
	IDWriteFontSet3* _fontSet;
	IDWriteFontCollection3* _fontCollection;

	HRESULT _hr = _dwriteFactory->CreateFontSetBuilder(&_fontSetBuilder);

	if (_hr != S_OK)
	{
		assert(false);
		return _hr;
	}

	const std::wstring _fontFilePathWstring = StringHelper::ToWString(_fontFilePath);
	_hr = _dwriteFactory->CreateFontFileReference(_fontFilePathWstring.c_str(),
	                                              /* lastWriteTime*/ nullptr,
	                                              &_fontFile);
	if (_hr != S_OK)
	{
		assert(false);
		return _hr;
	}

	_hr = _fontSetBuilder->AddFontFile(_fontFile);

	if (_hr != S_OK)
	{
		assert(false);
		return _hr;
	}

	_hr = _fontSetBuilder->CreateFontSet(
		reinterpret_cast<IDWriteFontSet**>(&_fontSet));

	if (_hr != S_OK)
	{
		assert(false);
		return _hr;
	}

	_dwriteFactory->CreateFontCollectionFromFontSet(
		_fontSet, reinterpret_cast<IDWriteFontCollection1**>(&_fontCollection));

	if (_hr != S_OK)
	{
		assert(false);
		return _hr;
	}

	_fontSetBuilder->Release();
	_fontFile->Release();
	_fontSet->Release();

	*_outFontCollection = new FontCollection(_fontCollection);
	return _hr;
}

FontCollection::FontCollection(IDWriteFontCollection3* _fontCollection):
	m_FontCollection(_fontCollection)
{
	InitializeFontName();
}

FontCollection::~FontCollection()
{
	m_FontCollection->Release();
}

void FontCollection::InitializeFontName()
{
	constexpr int MAX_FONT_NAME_LENGTH = 100;

	Microsoft::WRL::ComPtr<IDWriteFontFamily> _fontFamily;
	Microsoft::WRL::ComPtr<IDWriteLocalizedStrings> _localizedFontName;
	WCHAR _fontName[MAX_FONT_NAME_LENGTH];

	m_FontCollection->GetFontFamily(0, &_fontFamily);
	_fontFamily->GetFamilyNames(&_localizedFontName);
	_localizedFontName->GetString(0, _fontName, MAX_FONT_NAME_LENGTH);

	m_FontName = StringHelper::ToTString(_fontName);
}
