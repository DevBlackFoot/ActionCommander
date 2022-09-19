#pragma once

namespace SanyoniEngine
{
	/// <summary>
	/// The font weight enumeration describes common values for degree of blackness or thickness of strokes of characters in a font.
	/// Font weight values less than 1 or greater than 999 are considered to be invalid, and they are rejected by font API functions.
	/// </summary>
	enum class EFontWeight
	{
		/// <summary>
		/// Predefined font weight : Thin (100).
		/// </summary>
		THIN = 100,

		/// <summary>
		/// Predefined font weight : Extra-light (200).
		/// </summary>
		EXTRA_LIGHT = 200,

		/// <summary>
		/// Predefined font weight : Ultra-light (200).
		/// </summary>
		ULTRA_LIGHT = 200,

		/// <summary>
		/// Predefined font weight : Light (300).
		/// </summary>
		LIGHT = 300,

		/// <summary>
		/// Predefined font weight : Semi-light (350).
		/// </summary>
		SEMI_LIGHT = 350,

		/// <summary>
		/// Predefined font weight : Normal (400).
		/// </summary>
		NORMAL = 400,

		/// <summary>
		/// Predefined font weight : Regular (400).
		/// </summary>
		REGULAR = 400,

		/// <summary>
		/// Predefined font weight : Medium (500).
		/// </summary>
		MEDIUM = 500,

		/// <summary>
		/// Predefined font weight : Demi-bold (600).
		/// </summary>
		DEMI_BOLD = 600,

		/// <summary>
		/// Predefined font weight : Semi-bold (600).
		/// </summary>
		SEMI_BOLD = 600,

		/// <summary>
		/// Predefined font weight : Bold (700).
		/// </summary>
		BOLD = 700,

		/// <summary>
		/// Predefined font weight : Extra-bold (800).
		/// </summary>
		EXTRA_BOLD = 800,

		/// <summary>
		/// Predefined font weight : Ultra-bold (800).
		/// </summary>
		ULTRA_BOLD = 800,

		/// <summary>
		/// Predefined font weight : Black (900).
		/// </summary>
		BLACK = 900,

		/// <summary>
		/// Predefined font weight : Heavy (900).
		/// </summary>
		HEAVY = 900,

		/// <summary>
		/// Predefined font weight : Extra-black (950).
		/// </summary>
		EXTRA_BLACK = 950,

		/// <summary>
		/// Predefined font weight : Ultra-black (950).
		/// </summary>
		ULTRA_BLACK = 950
	};


	/// <summary>
	/// The font style enumeration describes the slope style of a font face, such as Normal, Italic or Oblique.
	/// Values other than the ones defined in the enumeration are considered to be invalid, and they are rejected by font API functions.
	/// </summary>
	enum class EFontStyle
	{
		/// <summary>
		/// Font slope style : Normal.
		/// </summary>
		NORMAL,

		/// <summary>
		/// Font slope style : Oblique.
		/// </summary>
		OBLIQUE,

		/// <summary>
		/// Font slope style : Italic.
		/// </summary>
		ITALIC
	};


	/// <summary>
	/// The font stretch enumeration describes relative change from the normal aspect ratio
	/// as specified by a font designer for the glyphs in a font.
	/// Values less than 1 or greater than 9 are considered to be invalid, and they are rejected by font API functions.
	/// </summary>
	enum class EFontStretch
	{
		/// <summary>
		/// Predefined font stretch : Not known (0).
		/// </summary>
		UNDEFINED = 0,

		/// <summary>
		/// Predefined font stretch : Ultra-condensed (1).
		/// </summary>
		ULTRA_CONDENSED = 1,

		/// <summary>
		/// Predefined font stretch : Extra-condensed (2).
		/// </summary>
		EXTRA_CONDENSED = 2,

		/// <summary>
		/// Predefined font stretch : Condensed (3).
		/// </summary>
		CONDENSED = 3,

		/// <summary>
		/// Predefined font stretch : Semi-condensed (4).
		/// </summary>
		SEMI_CONDENSED = 4,

		/// <summary>
		/// Predefined font stretch : Normal (5).
		/// </summary>
		NORMAL = 5,

		/// <summary>
		/// Predefined font stretch : Medium (5).
		/// </summary>
		MEDIUM = 5,

		/// <summary>
		/// Predefined font stretch : Semi-expanded (6).
		/// </summary>
		SEMI_EXPANDED = 6,

		/// <summary>
		/// Predefined font stretch : Expanded (7).
		/// </summary>
		EXPANDED = 7,

		/// <summary>
		/// Predefined font stretch : Extra-expanded (8).
		/// </summary>
		EXTRA_EXPANDED = 8,

		/// <summary>
		/// Predefined font stretch : Ultra-expanded (9).
		/// </summary>
		ULTRA_EXPANDED = 9
	};

	/**
	 * \brief Indicates the direction of how lines of text are placed relative to one another.
	 */
	enum class EFlowDirection
	{
		TopToBottom = 0,
		BottomToTop = 1,
		LeftToRight = 2,
		RightToLeft = 3
	};

	/**
	 * \brief The method used for line spacing in a text layout.
	 */
	enum class ELineSpacingMethod
	{
		Default = 0,
		Uniform = 1,
		Proportional = 2
	};

	/**
	 * \brief Specifies the alignment of paragraph text along the flow direction axis,
	 * relative to the top and bottom of the flow's layout box.
	 */
	enum class EParagraphAlignment
	{
		/**
		 * \brief The top of the text flow is aligned to the top edge of the layout box.
		 */
		Near = 0,

		/**
		 * \brief The bottom of the text flow is aligned to the bottom edge of the layout box.
		 */
		Far = 1,

		/**
		 * \brief The center of the flow is aligned to the center of the layout box.
		 */
		Center = 2
	};

	/**
	 * \brief Specifies the direction in which reading progresses.
	 * note: DWRITE_READING_DIRECTION_TOP_TO_BOTTOM and DWRITE_READING_DIRECTION_BOTTOM_TO_TOP are available in Windows 8.1 and later, only.
	 */
	enum class EReadingDirection
	{
		LeftToRight = 0,
		RightToLeft = 1,
		TopToBottom = 2,
		BottomToTop = 3
	};

	/**
	 * \brief Specifies the alignment of paragraph text along the reading direction axis,
	 * relative to the leading and trailing edge of the layout box.
	 *
	 * https://docs.microsoft.com/ko-kr/windows/win32/api/dwrite/nf-dwrite-idwritetextformat-settextalignment
	 */
	enum class ETextAlignment
	{
		/**
		 * \brief The leading edge of the paragraph text is aligned to the leading edge of the layout box.
		 */
		Leading,

		/**
		 * \brief The trailing edge of the paragraph text is aligned to the trailing edge of the layout box.
		 */
		Trailing,

		/**
		 * \brief The center of the paragraph text is aligned to the center of the layout box.
		 */
		Center,

		/**
		 * \brief Align text to the leading side, and also justify text to fill the lines.
		 */
		Justified
	};

	/**
	 * \brief Specifies the text granularity used to trim text overflowing the layout box.
	 */
	enum class ETrimmingGranularity
	{
		/**
		 * \brief No trimming occurs. Text flows beyond the layout width.
		 */
		None,

		/**
		 * \brief Trimming occurs at a character cluster boundary.
		 */
		Character,

		/**
		 * \brief Trimming occurs at a word boundary.
		 */
		Word
	};

	/**
	 * \brief Specifies the word wrapping to be used in a particular multiline paragraph.
	 *
	 * note: DWRITE_WORD_WRAPPING_EMERGENCY_BREAK, DWRITE_WORD_WRAPPING_WHOLE _WORD,
	 * and DWRITE_WORD_WRAPPING_CHARACTER are available in Windows 8.1 and later, only.
	 *
	 * https://docs.microsoft.com/en-us/windows/win32/api/dwrite/ne-dwrite-dwrite_word_wrapping
	 */
	enum class EWordWrapping
	{
		/**
		 * \brief Indicates that words are broken across lines to avoid text overflowing the layout box.
		 */
		Wrap = 0,

		/**
		 * \brief Indicates that words are kept within the same line even when it overflows the layout box.
		 * This option is often used with scrolling to reveal overflow text.
		 */
		NoWrap = 1,

		/**
		 * \brief Words are broken across lines to avoid text overflowing the layout box.
		 * Emergency wrapping occurs if the word is larger than the maximum width.
		 */
		EmergencyBreak = 2,

		/**
		 * \brief When emergency wrapping, only wrap whole words,
		 * never breaking words when the layout width is too small for even a single word.
		 */
		WholeWord = 3,

		/**
		 * \brief Wrap between any valid character clusters.
		 */
		Character = 4
	};
}
