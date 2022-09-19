#pragma once

#include "EDWrite.h"
#include "SanyoniEngine_typedef.h"

namespace SanyoniEngine
{
	class FontCollection;

	class TextFormat
	{
	public:
		struct LineSpacingInfo
		{
			ELineSpacingMethod method;
			float lineSpacing;
			float baseLine;
		};

		struct TrimmingInfo
		{
			ETrimmingGranularity granularity;
			UINT32 delimiter;
			UINT32 delimiterCount;

			IDWriteInlineObject* trimmingSign;
		};

	private:
		TextFormat(IDWriteTextFormat3* _textFormat, FontCollection* _fontCollection);

		~TextFormat();

	private:
		IDWriteTextFormat3* m_TextFormat;

		FontCollection* m_FontCollection;

	public:
		/*
		 * getters
		 */

		IDWriteTextFormat* GetTextFormat() const { return m_TextFormat; }

		FontCollection* GetFontCollection() const { return m_FontCollection; }

		float GetFontSize() const { return m_TextFormat->GetFontSize(); }

		EFontStretch GetFontStretch() const { return static_cast<EFontStretch>(m_TextFormat->GetFontStretch()); }

		EFontStyle GetFontStyle() const { return static_cast<EFontStyle>(m_TextFormat->GetFontStyle()); }

		EFontWeight GetFontWeight() const { return static_cast<EFontWeight>(m_TextFormat->GetFontWeight()); }

		EFlowDirection GetFlowDirection() const { return static_cast<EFlowDirection>(m_TextFormat->GetFlowDirection()); }

		float GetTabStop() const { return m_TextFormat->GetIncrementalTabStop(); }

		LineSpacingInfo GetLineSpacing() const;

		EParagraphAlignment GetParagraphAlignment() const { return static_cast<EParagraphAlignment>(m_TextFormat->GetParagraphAlignment()); }

		EReadingDirection GetReadingDirection() const { return static_cast<EReadingDirection>(m_TextFormat->GetReadingDirection()); }

		ETextAlignment GetTextAlignment() const { return static_cast<ETextAlignment>(m_TextFormat->GetTextAlignment()); }

		TrimmingInfo GetTrimming() const;

		EWordWrapping GetWordWrapping() const { return static_cast<EWordWrapping>(m_TextFormat->GetWordWrapping()); }

		/*
		 * setters
		 */

		void SetFlowDirection(EFlowDirection _flowDirection) { m_TextFormat->SetFlowDirection(static_cast<DWRITE_FLOW_DIRECTION>(_flowDirection)); }

		void SetTabStop(float _tabStop) { m_TextFormat->SetIncrementalTabStop(_tabStop); }

		void SetLineSpacing(const LineSpacingInfo& _lineSpacing);

		void SetParagraphAlignment(EParagraphAlignment _alignment) { m_TextFormat->SetParagraphAlignment(static_cast<DWRITE_PARAGRAPH_ALIGNMENT>(_alignment)); }

		void SetReadingDirection(EReadingDirection _direction) { m_TextFormat->SetReadingDirection(static_cast<DWRITE_READING_DIRECTION>(_direction)); }

		void SetTextAlignment(ETextAlignment _alignment) { m_TextFormat->SetTextAlignment(static_cast<DWRITE_TEXT_ALIGNMENT>(_alignment)); }

		void SetTrimming(const TrimmingInfo& _trimming);

		void SetWordWrapping(EWordWrapping _wordWrapping) { m_TextFormat->SetWordWrapping(static_cast<DWRITE_WORD_WRAPPING>(_wordWrapping)); }

	public:
		static HRESULT Create(TextFormat** _outTextFormat,
		                      IDWriteFactory7* _dwriteFactory, FontCollection* _fontCollection,
		                      float _fontSize = 12.f,
		                      EFontWeight _fontWeight = EFontWeight::NORMAL, EFontStyle _fontStyle = EFontStyle::NORMAL, EFontStretch _fontStretch = EFontStretch::NORMAL,
		                      tstring _localeName = TEXT(""));
	};
}
