#pragma once

// Standard base includes, defines that indicate our current platform, etc.

#include <stddef.h>
#include <stdlib.h>
#include <string>

using namespace std;
// Which platform we are on?
// Windows (regular win32)
 
#if _MSC_VER

#define SUPPORT_D3D11 1
#define SUPPORT_D3D12 0//@TODO: enable by default? comment this out if you don't have D3D12 header/library files
#define SUPPORT_OPENGL 0

#elif defined(__APPLE__)
	#if defined(__arm__) || defined(__arm64__)
		#define SUPPORT_OPENGL 1
	#else
		#define SUPPORT_OPENGL 1
	#endif
#elif defined(__EMSCRIPTEN__)
	#define SUPPORT_OPENGL 1
#else
	#error "Unknown platform!"
#endif
 

// COM-like Release macro
#ifndef SAFE_RELEASE
	#define SAFE_RELEASE(a) if (a) { a->Release(); a = NULL; }
#endif

 