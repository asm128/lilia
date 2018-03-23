/// Copyright 2017-2018 - asm128
#include "llc_runtime.h"
#include "llc_ptr.h"

#define LLC_DEFINE_APPLICATION_ENTRY_POINT(_mainClass)																																													\
static	::llc::error_t																				rtMain										(::llc::SRuntimeValues& runtimeValues);													\
		int																							main										()																					{	\
	::llc::SRuntimeValues																					runtimeValues								= {{::GetModuleHandle(NULL), 0, 0, SW_SHOW},};									\
	return ::llc::failed(::rtMain(runtimeValues)) ? EXIT_FAILURE : EXIT_SUCCESS;																																						\
}																																																										\
																																																										\
		int	WINAPI																					WinMain																																\
	(	_In_		::HINSTANCE		hInstance																																															\
	,	_In_opt_	::HINSTANCE		hPrevInstance																																														\
	,	_In_		::LPSTR			lpCmdLine																																															\
	,	_In_		::INT			nShowCmd																																															\
	)																																																									\
{																																																										\
	::llc::SRuntimeValues																					runtimeValues								=																				\
		{{	hInstance																																																					\
		,	hPrevInstance																																																				\
		,	lpCmdLine																																																					\
		,	nShowCmd																																																					\
		},};																																																							\
	return ::llc::failed(::rtMain(runtimeValues)) ? EXIT_FAILURE : EXIT_SUCCESS;																																						\
}																																																										\
																																																										\
static	::llc::error_t																				setup										(_mainClass& applicationInstance);														\
static	::llc::error_t																				cleanup										(_mainClass& applicationInstance);														\
static	::llc::error_t																				update										(_mainClass& applicationInstance, bool systemRequestedExit);							\
static	::llc::error_t																				draw										(_mainClass& applicationInstance);														\
		::llc::error_t																				rtMain										(::llc::SRuntimeValues& runtimeValues)												{	\
	{																																																									\
		_CrtSetDbgFlag(_CRTDBG_LEAK_CHECK_DF | _CRTDBG_ALLOC_MEM_DF);																																									\
		::llc::ptr_obj<_mainClass>																			applicationInstance							= {};																			\
		reterr_error_if(0 == applicationInstance.create(runtimeValues), "Failed to create application instance. Out of memory?");																										\
		info_printf("Initializing application instance.");																																												\
		llc_necall(::setup(*applicationInstance), "User reported error. Execution stopped.");																																			\
		info_printf("Application instance initialized successfully. Executing main loop...");																																			\
		while(true) {																																																					\
			::llc::error_t																						updateResult								= ::update(*applicationInstance, ::GetAsyncKeyState(VK_ESCAPE) != 0);		\
			break_info_if(1 == updateResult, "Application requested termination.");																																						\
			break_error_if(errored(updateResult), "update() returned error.");																																							\
			error_if(::draw(*applicationInstance), "Why would this ever happen?");																																						\
		}																																																								\
		info_printf("Cleaning up application instance...");																																												\
		::cleanup(*applicationInstance);																																																\
		info_printf("Application instance destroyed.");																																													\
	}																																																									\
	return 0;																																																							\
}
