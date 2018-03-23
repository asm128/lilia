/// Copyright 2016-2018 - asm128
#include "llc_platform.h"
#if defined(LLC_WINDOWS)
#	include <Windows.h>
#endif

#ifndef LLC_RUNTIME_H_28903749028374
#define LLC_RUNTIME_H_28903749028374

namespace llc 
{
#if defined(LLC_WINDOWS)
	struct SWindowsEntryPointArgs {
				HINSTANCE														hInstance							;
				HINSTANCE														hPrevInstance						;
				LPSTR															lpCmdLine							;
				int																nShowCmd							;
	};
#endif

	struct SRuntimeValuesDetail {	
#if defined(LLC_ANDROID)
				ANativeActivity													* Activity							= nullptr;
				void															* SavedState						= nullptr;
				size_t															SavedStateSize						= 0;
#elif defined(LLC_WINDOWS)
				SWindowsEntryPointArgs											EntryPointArgs						;
#else
#	error "Not implemented."
#endif
	};

	struct SRuntimeValues {
				SRuntimeValuesDetail											PlatformDetail						;
	};
}

#endif // LLC_RUNTIME_H_28903749028374

