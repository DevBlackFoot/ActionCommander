#pragma once
#include "SanyoniEngine_typedef.h"

namespace SanyoniEngine
{
	class FontCollection final
	{
	private:
		FontCollection(IDWriteFontCollection3* _fontCollection);

		virtual ~FontCollection();

	private:
		void InitializeFontName();

	private:
		IDWriteFontCollection3* m_FontCollection;

		tstring m_FontName;

	public:
		auto GetFontCollection() const { return m_FontCollection; }

		tstring GetFontName() const { return m_FontName; }

	public:
		static HRESULT Create(IDWriteFactory7* _dwriteFactory, tstring _fontFilePath, FontCollection** _outFontCollection);
	};
}
