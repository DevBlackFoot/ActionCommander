#pragma once

#include <basetsd.h>

/*
 * Char and String
 */
#include <tchar.h>
#include <xstring>

using tchar = TCHAR;

#if defined(UNICODE)
using tstring = std::wstring;
#else
using tstring = std::string;
#endif

/*
 * Direct2D
 */
#include <dcommon.h>
#include <d2d1_1.h>

using IBrush = ID2D1Brush;
using SolidColorBrush = ID2D1SolidColorBrush;
using BitmapBrush = ID2D1BitmapBrush;
using LinearGradientBrush = ID2D1LinearGradientBrush;
using RadialGradientBrush = ID2D1RadialGradientBrush;

using Bitmap = ID2D1Bitmap;
