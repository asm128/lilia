/// Copyright 2010-2018 - asm128
#include "llc_platform.h"

#ifndef LLC_DEBUG_H_97826389432654
#define LLC_DEBUG_H_97826389432654

#if defined(LLC_DEBUG_ENABLED)
#	if defined(LLC_ANDROID) || defined(LLC_LINUX)
#		define LLC_PLATFORM_CRT_BREAKPOINT()	do {} while(0)
#		define LLC_PLATFORM_CRT_CHECK_MEMORY()	do {} while(0)
#	elif defined(LLC_WINDOWS)
#		include <crtdbg.h>
#		define LLC_PLATFORM_CRT_BREAKPOINT		(void)_CrtDbgBreak
#		define LLC_PLATFORM_CRT_CHECK_MEMORY()	do {} while(0) // (void)_CrtCheckMemory
#	else
#		error "Platform not supported."
#	endif
#	define LLC_ERROR_PRINTF_ENABLED	
#	define LLC_WARNING_PRINTF_ENABLED	
#	define LLC_INFO_PRINTF_ENABLED	
//#	define LLC_SUCCESS_PRINTF_ENABLED	// Uncomment as needed
#	ifndef LLC_DISABLE_DEBUG_BREAK_ON_ERROR_LOG
#		define LLC_USE_DEBUG_BREAK_ON_ERROR_LOG
#	endif
#else
#	define LLC_PLATFORM_CRT_BREAKPOINT()		do {} while(0)
#	define LLC_PLATFORM_CRT_CHECK_MEMORY()		do {} while(0)
#	define LLC_ERROR_PRINTF_ENABLED	
#	define LLC_WARNING_PRINTF_ENABLED	// Uncomment as needed
//#	define LLC_INFO_PRINTF_ENABLED		// Uncomment as needed
//#	define LLC_SUCCESS_PRINTF_ENABLED	// Uncomment as needed
#endif

#endif // LLC_DEBUG_H_97826389432654
