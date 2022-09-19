#include "SanyoniEngine_pch.h"
#include "TextFormat.h"

#include "FontCollection.h"
#include "Helper.h"

using namespace SanyoniEngine;

TextFormat::TextFormat(IDWriteTextFormat3* _textFormat, FontCollection* _fontCollection) :
	m_TextFormat(_textFormat),
	m_FontCollection(_fontCollection)
{
}

TextFormat::~TextFormat()
{
	m_TextFormat->Release();
}

TextFormat::LineSpacingInfo TextFormat::GetLineSpacing() const
{
	DWRITE_LINE_SPACING_METHOD _method;
	float _lineSpacing;
	float _baseLine;

	if (m_TextFormat->GetLineSpacing(&_method, &_lineSpacing, &_baseLine) != S_OK)
	{
		assert(false);
	}

	return {static_cast<ELineSpacingMethod>(_method), _lineSpacing, _baseLine};
}

TextFormat::TrimmingInfo TextFormat::GetTrimming() const
{
	DWRITE_TRIMMING _trimmingOptions;
	IDWriteInlineObject* _trimmingSign;

	if (m_TextFormat->GetTrimming(&_trimmingOptions, &_trimmingSign) != S_OK)
	{
		assert(false);
	}

	return TrimmingInfo{
		static_cast<ETrimmingGranularity>(_trimmingOptions.granularity),
		_trimmingOptions.delimiter, _trimmingOptions.delimiterCount,
		_trimmingSign
	};
}

void TextFormat::SetLineSpacing(const LineSpacingInfo& _lineSpacing)
{
	m_TextFormat->SetLineSpacing(
		static_cast<DWRITE_LINE_SPACING_METHOD>(_lineSpacing.method),
		_lineSpacing.lineSpacing,
		_lineSpacing.baseLine
	);
}

void TextFormat::SetTrimming(const TrimmingInfo& _trimming)
{
	DWRITE_TRIMMING _dwriteTrimming = DWRITE_TRIMMING{
		static_cast<DWRITE_TRIMMING_GRANULARITY>(_trimming.granularity),
		_trimming.delimiter, _trimming.delimiterCount
	};
	m_TextFormat->SetTrimming(&_dwriteTrimming, _trimming.trimmingSign);
}

HRESULT TextFormat::Create(TextFormat** _outTextFormat, IDWriteFactory7* _dwriteFactory, FontCollection* _fontCollection,
                           float _fontSize, EFontWeight _fontWeight, EFontStyle _fontStyle, EFontStretch _fontStretch, tstring _localeName)
{
	assert(_outTextFormat != nullptr);
	assert(_dwriteFactory != nullptr);
	assert(_fontCollection != nullptr);

	*_outTextFormat = nullptr;

	IDWriteTextFormat3* _textFormat;

	HRESULT _hr = _dwriteFactory->CreateTextFormat(
		StringHelper::ToWString(_fontCollection->GetFontName()).c_str(),
		_fontCollection->GetFontCollection(),
		static_cast<DWRITE_FONT_WEIGHT>(_fontWeight),
		static_cast<DWRITE_FONT_STYLE>(_fontStyle),
		static_cast<DWRITE_FONT_STRETCH>(_fontStretch),
		_fontSize,
		StringHelper::ToWString(_localeName).c_str(),
		reinterpret_cast<IDWriteTextFormat**>(&_textFormat)
	);

	if (_hr != S_OK)
	{
		assert(false);
		return _hr;
	}

	// TODO: text format의 속성을 하드코딩하고 있습니다.
	_textFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING);
	_textFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_NEAR);
	_textFormat->SetWordWrapping(DWRITE_WORD_WRAPPING_NO_WRAP);

	*_outTextFormat = new TextFormat{_textFormat, _fontCollection};

	return _hr;
}
