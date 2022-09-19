// header.h : include file for standard system include files,
// or project specific include files
//

#pragma once

#include "SanyoniEngine_pch.h"

#include "SanyoniEngine_types.h"

/*
 * Macros
 */

#ifndef Assert
#if defined( DEBUG ) || defined( _DEBUG )

#define Assert(b) OutputDebugStringA("Assert: " #b "\n");

#else

#define Assert(b)

#endif //DEBUG || _DEBUG
#endif


#ifndef HINST_THISCOMPONENT
EXTERN_C IMAGE_DOS_HEADER __ImageBase;
#define HINST_THISCOMPONENT ((HINSTANCE)&__ImageBase)
#endif
