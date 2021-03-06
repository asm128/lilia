#include "llc_error.h"
#include "llc_debug.h"
#include "llc_size.h"

#include <cstdio>

#ifndef LLC_LOG_H_8927349654687654365
#define LLC_LOG_H_8927349654687654365

//#define LLC_NULLIFY_ALL_CONDITIONAL_ERROR_CHECKING
#if !defined(DEBUG) && !defined(_DEBUG)
#	if defined(LLC_NULLIFY_ALL_CONDITIONAL_ERROR_CHECKING) 
#		define	LLC_NULLIFY_NECALL
#		define	LLC_NULLIFY_CONDITIONAL_RETERR
#		define	LLC_NULLIFY_CONDITIONAL_LOG
#		define	LLC_NULLIFY_CONDITIONAL_THROW
#	else
#		//ifndef LLC_NULLIFY_NECALL
#		//	define LLC_NULLIFY_NECALL
#		//endif
#		//ifndef LLC_NULLIFY_CONDITIONAL_RETERR
#		//	define LLC_NULLIFY_CONDITIONAL_RETERR
#		//endif
#		//ifndef LLC_NULLIFY_CONDITIONAL_LOG
#		//	define LLC_NULLIFY_CONDITIONAL_LOG
#		//endif
#		//ifndef LLC_NULLIFY_CONDITIONAL_THROW
#		//	define LLC_NULLIFY_CONDITIONAL_THROWF
#		//endif
#	endif
#endif

namespace llc
{
	void															_llc_print_system_errors			(const char* prefix, uint32_t prefixLen);
	void															_base_debug_print					(const char* prefix, uint32_t prefixLen);

#define base_debug_print(prefix, prefixLen)	::llc::_base_debug_print(prefix, (uint32_t)prefixLen)

	template<const size_t _sizePrefix, typename... TArgs>		
	void															_llc_debug_printf					(int severity, const char (&prefix)[_sizePrefix], uint32_t prefixLength, const char* format, const TArgs... args)			{
		base_debug_print(prefix, prefixLength);
		char																customDynamicString	[8192]			= {0};
		const size_t														stringLength						= sprintf_s(customDynamicString, format, args...);
		base_debug_print(customDynamicString, (int)stringLength);
		base_debug_print("\n", 1);
		if(2 >= severity)
			::llc::_llc_print_system_errors(prefix, prefixLength);
	}
}

#define debug_printf(severity, severityStr, format, ...)																																	\
	do { 																																													\
		static constexpr const char											prefixFormat	[]								= ":%u:" severityStr ":" __FILE__ "(%u){" __FUNCTION__ "}:";	\
		static char															prefixString	[::llc::size(prefixFormat) + 8]	= {}; 															\
		static const int 													prefixLength									= ::sprintf_s(prefixString, prefixFormat, severity, __LINE__);	\
		::llc::_llc_debug_printf(severity, prefixString, prefixLength == -1 ? 0 : prefixLength, format, __VA_ARGS__);																		\
	} while(0)

#if defined (LLC_ERROR_PRINTF_ENABLED)
#	define error_printf(format, ...)								do { debug_printf(1, "error"	, format, __VA_ARGS__); LLC_PLATFORM_CRT_BREAKPOINT(); } while(0)
#else
#	define error_printf(format, ...)								do { __VA_ARGS__; LLC_PLATFORM_CRT_BREAKPOINT(); } while(0)
#endif	

#if defined (LLC_WARNING_PRINTF_ENABLED)
#	define warning_printf(format, ...)								debug_printf(2, "warning"	, format, __VA_ARGS__)
#else
#	define warning_printf(format, ...)								do { __VA_ARGS__; } while(0)
#endif	

#if defined (LLC_INFO_PRINTF_ENABLED)
#	define info_printf(format, ...)									debug_printf(3, "info"		, format, __VA_ARGS__)
#else
#	define info_printf(format, ...)									do { __VA_ARGS__; } while(0)
#endif	

#if defined (LLC_SUCCESS_PRINTF_ENABLED)
#	define success_printf(format, ...)								debug_printf(4, "info"		, format, __VA_ARGS__)
#else
#	define success_printf(format, ...)								do { __VA_ARGS__; } while(0)
#endif	

#ifndef LLC_NULLIFY_CONDITIONAL_THROW
#define throw_if(condition, exception, format, ...)				if(condition) { error_printf	(format, __VA_ARGS__); base_debug_print("Condition: " #condition, (uint32_t)-1); throw(exception);	}
#else
#pragma warning(disable:4552)	// this is required because "condition" may have no side effect.
#pragma warning(disable:4553)	// this is required because "condition" may have no side effect.
#define throw_if(condition, exception, format, ...)				do{ condition; } while(0)
#endif

#ifndef LLC_NULLIFY_CONDITIONAL_LOG
#define error_if(condition, format, ...)						if(condition) { error_printf	(format, __VA_ARGS__); base_debug_print("Condition: " #condition, (uint32_t)-1); 						}
#define warn_if(condition, format, ...)							if(condition) { warning_printf	(format, __VA_ARGS__); base_debug_print("Condition: " #condition, (uint32_t)-1); 						}
#define info_if(condition, format, ...)							if(condition) { info_printf		(format, __VA_ARGS__); base_debug_print("Condition: " #condition, (uint32_t)-1); 						}
#else
#pragma warning(disable:4552)	// this is required because "condition" may have no side effect.
#pragma warning(disable:4553)	// this is required because "condition" may have no side effect.
#define error_if(condition, format, ...)						do{ condition; } while(0)
#define warn_if(condition, format, ...)							do{ condition; } while(0)
#define info_if(condition, format, ...)							do{ condition; } while(0)
#endif

#define ret_error_if(condition, format, ...)					if(condition) { error_printf	(format, __VA_ARGS__); return;			}
#define ret_warn_if(condition, format, ...)						if(condition) { warning_printf	(format, __VA_ARGS__); return;			}
#define ret_info_if(condition, format, ...)						if(condition) { info_printf		(format, __VA_ARGS__); return;			}

#define break_error_if(condition, format, ...)					if(condition) { error_printf	(format, __VA_ARGS__); break;			}
#define break_warn_if(condition, format, ...)					if(condition) { warning_printf	(format, __VA_ARGS__); break;			}
#define break_info_if(condition, format, ...)					if(condition) { info_printf		(format, __VA_ARGS__); break;			}

#define continue_error_if(condition, format, ...)				if(condition) { error_printf	(format, __VA_ARGS__); continue;		}
#define continue_warn_if(condition, format, ...)				if(condition) { warning_printf	(format, __VA_ARGS__); continue;		}
#define continue_info_if(condition, format, ...)				if(condition) { info_printf		(format, __VA_ARGS__); continue;		}

#define retval_error_if(retVal, condition, format, ...)			if(condition) { error_printf	(format, __VA_ARGS__); return retVal;	}
#define retval_warn_if(retVal, condition, format, ...)			if(condition) { warning_printf	(format, __VA_ARGS__); return retVal;	}
#define retval_info_if(retVal, condition, format, ...)			if(condition) { info_printf		(format, __VA_ARGS__); return retVal;	}

#define retnul_error_if(condition, format, ...)					retval_error_if	( 0, condition, format, __VA_ARGS__)
#define retnul_warn_if(condition, format, ...)					retval_warn_if	( 0, condition, format, __VA_ARGS__)
#define retnul_info_if(condition, format, ...)					retval_info_if	( 0, condition, format, __VA_ARGS__)

#ifndef LLC_NULLIFY_CONDITIONAL_RETERR
#define reterr_error_if(condition, format, ...)					retval_error_if	(-1, condition, format, __VA_ARGS__)
#define reterr_warn_if(condition, format, ...)					retval_warn_if	(-1, condition, format, __VA_ARGS__)
#define reterr_info_if(condition, format, ...)					retval_info_if	(-1, condition, format, __VA_ARGS__)
#else
#pragma warning(disable:4552)	// this is required because "condition" may have no side effect.
#pragma warning(disable:4553)	// this is required because "condition" may have no side effect.
#define reterr_error_if(condition, format, ...)					do{ condition; } while(0)
#define reterr_warn_if(condition, format, ...)					do{ condition; } while(0)
#define reterr_info_if(condition, format, ...)					do{ condition; } while(0)
#endif

#define retwarn_error_if(condition, format, ...)				retval_error_if	( 1, condition, format, __VA_ARGS__)
#define retwarn_warn_if(condition, format, ...)					retval_warn_if	( 1, condition, format, __VA_ARGS__)
#define retwarn_info_if(condition, format, ...)					retval_info_if	( 1, condition, format, __VA_ARGS__)


#if defined (LLC_ERROR_PRINTF_ENABLED)
// Non-propagable retval_error call.
#	define llc_rve_ecall(retVal, llcl_call, format, ...) do {																														\
		::llc::error_t llc_errCall_ = (llcl_call);  																																\
		if(llc_errCall_ < 0) {																																						\
			debug_printf(0, "error", "%s: 0x%X.", #llcl_call, llc_errCall_);																										\
			error_printf(format, __VA_ARGS__); 																																		\
			return retVal; 																																							\
		}																																											\
		else {																																										\
			success_printf("%s: Success (0x%X).", #llcl_call, llc_errCall_);																										\
		}																																											\
	} while(0)

// Non-propagable retval_error error-warning call.
#	define llc_rve_ewcall(retVal, llcl_call, format, ...) do {																														\
		if(::llc::error_t llc_errCall_ = (llcl_call)) { 																															\
			if(llc_errCall_ < 0) {																																					\
				debug_printf(0, "error", "%s: 0x%X.", #llcl_call, llc_errCall_);																									\
				error_printf(format, __VA_ARGS__); 																																	\
				return retval; 																																						\
			}																																										\
			else {																																									\
				warning_printf("%s: 0x%X.", #llcl_call, llc_errCall_);																												\
			}																																										\
		}																																											\
		else {																																										\
			success_printf("%s: Success (0x%X).", #llcl_call, llc_errCall_);																										\
		}																																											\
	} while(0)

//
#	define llc_rv_hrcall(retVal, hr_call) do {																																		\
		::HRESULT errCall_ = (hr_call);  																																			\
		if FAILED(errCall_) {																																						\
			debug_printf(0, "error", "%s: (0x%X) : '%s'.", #hr_call, errCall_, ::llc::getWindowsErrorAsString(errCall_).c_str());													\
			return retVal; 																																							\
		}																																											\
		else {																																										\
			success_printf("%s: Success (0x%X).", #hr_call, errCall_);																												\
		}																																											\
	} while(0)

//
#	define llc_rve_hrcall(retVal, hr_call, format, ...) do {																														\
		::HRESULT errCall_ = (hr_call);  																																			\
		if FAILED(errCall_) {																																						\
			debug_printf(0, "error", "%s: (0x%X) : '%s' - " format, #hr_call, errCall_, ::llc::getWindowsErrorAsString(errCall_).c_str(), __VA_ARGS__);								\
			return retVal; 																																							\
		}																																											\
		else {																																										\
			success_printf("%s: Success (0x%X).", #hr_call, errCall_);																												\
		}																																											\
	} while(0)

// --------------------------------------------------------------------
// Propagable retval_error call.
#	define llc_pecall(llcl_call, ...) do {																																			\
		::llc::error_t llc_errCall_ = (llcl_call);  																																\
		if(llc_errCall_ < 0) {																																						\
			debug_printf(0, "error", "%s: 0x%X", #llcl_call, llc_errCall_);																											\
			error_printf(__VA_ARGS__); 																																				\
			return llc_errCall_; 																																					\
		}																																											\
		else {																																										\
			success_printf("%s: Success (0x%X).", #llcl_call, llc_errCall_);																										\
		}																																											\
	} while(0)

// Propagable retval_error error-warning call.
#	define llc_pewcall(llcl_call, ...) do {																																			\
		if(::llc::error_t llc_errCall_ = (llcl_call)) { 																															\
			if(llc_errCall_ < 0) {																																					\
				debug_printf(0, "error", "%s: 0x%X", #llcl_call, llc_errCall_);																										\
				error_printf(__VA_ARGS__); 																																			\
				return llc_errCall_; 																																				\
			}																																										\
			else {																																									\
				warning_printf("%s: 0x%X.", #llcl_call, llc_errCall_);																												\
			}																																										\
		}																																											\
		else {																																										\
			success_printf("%s: Success (0x%X).", #llcl_call, llc_errCall_);																										\
		}																																											\
	} while(0)

#else
#	define llc_rve_ecall(retVal, llcl_call, ...) do {																																\
		if(::llc::failed(llcl_call))  																																				\
			return retval; 																																							\
	} while(0)

#	define llc_rve_hrcall(retVal, llcl_call, ...) do {																																\
		if(FAILED(llcl_call))  																																						\
			return retval; 																																							\
	} while(0)

#	define llc_rv_hrcall(retVal, llcl_call) do {																																	\
		if(FAILED(llcl_call))  																																						\
			return retval; 																																							\
	} while(0)

#	define llc_pecall(llcl_call, ...) do {																																			\
		::llc::error_t llc_errCall_ = (llcl_call);																																	\
		if(::llc::failed(llc_errCall_)) 																																			\
			return llc_errCall_; 																																					\
	} while(0)

#	define llc_rve_ewcall											llc_rve_ecall	// Non-propagable retval_error error-warning call.
#	define llc_pewcall												llc_pecall			// Propagable retval_error error-warning call.
#endif

#ifndef LLC_NULLIFY_NECALL
#define llc_necall(llcl_call, ...)								llc_rve_ecall (-1, llcl_call, __VA_ARGS__)	// Non-propagable error call.
#define llc_newcall(llcl_call, ...)								llc_rve_ewcall(-1, llcl_call, __VA_ARGS__)	// Non-propagable error-warning call.
#define llc_hrcall(hr_call)										llc_rv_hrcall (-1, hr_call)					// HRESULT call.
#define llc_hrecall(hr_call, ...)								llc_rve_hrcall(-1, hr_call, __VA_ARGS__)	// HRESULT call.
#else
#define llc_necall(llcl_call, ...)								do{llcl_call; } while(0) // Non-propagable error call.
#define llc_newcall(llcl_call, ...)								do{llcl_call; } while(0) // Non-propagable error-warning call.
#define llc_hrcall(hr_call)										do{hr_call	; } while(0) // HRESULT call.
#define llc_hrecall(hr_call, ...)								do{hr_call	; } while(0) // HRESULT call.
#endif

#define e_if													error_if
#define w_if													warn_if
#define i_if													info_if

#define re_if													ret_error_if
#define rw_if													ret_warn_if
#define ri_if													ret_info_if

#define be_if													break_error_if
#define bw_if													break_warn_if
#define bi_if													break_info_if

#define ce_if													continue_error_if
#define cw_if													continue_warn_if
#define ci_if													continue_info_if

#define rve_if													retval_error_if
#define rvw_if													retval_warn_if
#define rvi_if													retval_info_if

#define rne_if													retnul_error_if
#define rnw_if													retnul_warn_if 
#define rni_if													retnul_info_if

#define ree_if													reterr_error_if
#define rew_if													reterr_warn_if 
#define rei_if													reterr_info_if 

#define rwe_if													retwarn_error_if
#define rww_if													retwarn_warn_if 
#define rwi_if													retwarn_info_if 

#endif // LLC_LOG_H_8927349654687654365
