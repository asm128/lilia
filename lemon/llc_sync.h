#include "llc_typeint.h"

#ifndef LLC_SYNC_H_023749082374
#define LLC_SYNC_H_023749082374

namespace llc
{
#if defined(LLC_ANDROID) || defined(LLC_LINUX)
/// <Atomic Builtins> http://gcc.gnu.org/onlinedocs/gcc-4.4.3/gcc/Atomic-Builtins.html#Atomic-Builtins
#	include <mutex>
#	define sync_increment(nCount)							(__sync_add_and_fetch(&nCount, 1))
#	define sync_decrement(nCount)							(__sync_sub_and_fetch(&nCount, 1))
#	define sync_exchange(target, value)						(__sync_lock_test_and_set(&target, value))
#	define sync_compare_exchange(nCount, value, comparand)	(__sync_val_compare_and_swap(&nCount, comparand, value))
#	define DECLARE_SHARED_SECTION(Name)						::std::mutex Name //(true)
#	define INIT_SHARED_SECTION(Name)						(1)
#	define ENTER_SHARED_SECTION(Name)						(Name).lock()
#	define LEAVE_SHARED_SECTION(Name)						(Name).unlock()
#	define DELETE_SHARED_SECTION(Name)						(0)
#elif defined(LLC_WINDOWS)  
#	define WIN32_LEAN_AND_MEAN
#	include <Windows.h>
#	if (defined( _WIN64 ) || defined( WIN64 )) 
#		define sync_increment(nCount)							( InterlockedIncrement64		( &nCount ) )
#		define sync_decrement(nCount)							( InterlockedDecrement64		( &nCount ) )
#		define sync_exchange(nCount)							( InterlockedExchange64			( &nCount ) )
#		define sync_compare_exchange(nCount, value, comparand)	( InterlockedCompareExchange64	( &nCount, value, comparand ) )
#	elif (defined( _WIN32 ) || defined( WIN32 )) 
#		define sync_increment(nCount)							( InterlockedIncrement			( &nCount ) )
#		define sync_decrement(nCount)							( InterlockedDecrement			( &nCount ) )
#		define sync_exchange(nCount)							( InterlockedExchange			( &nCount ) )
#		define sync_compare_exchange(nCount, value, comparand)	( InterlockedCompareExchange	( &nCount, value, comparand ) )
#	endif
#	define DECLARE_SHARED_SECTION(Name)						CRITICAL_SECTION Name
#	define INIT_SHARED_SECTION(Name)						((int32_t)InitializeCriticalSectionAndSpinCount(&Name, 400))
#	define ENTER_SHARED_SECTION(Name)						EnterCriticalSection (&Name)
#	define LEAVE_SHARED_SECTION(Name)						LeaveCriticalSection (&Name)
#	define DELETE_SHARED_SECTION(Name)						DeleteCriticalSection(&Name)
#else
#	include <mutex>
#	define sync_increment(nCount)							(++(nCount))
#	define sync_decrement(nCount)							(--(nCount))
#	define sync_exchange(Target, Value)						(Target = Value)
#	define DECLARE_SHARED_SECTION(Name)						::std::mutex Name //(true)
#	define INIT_SHARED_SECTION(Name)						(1)
#	define ENTER_SHARED_SECTION(Name)						(Name).lock()
#	define LEAVE_SHARED_SECTION(Name)						(Name).unlock()
#	define DELETE_SHARED_SECTION(Name)						(0)			
#endif

#if defined(LLC_MTSUPPORT)
#	define DECLARE_CRITICAL_SECTION							DECLARE_SHARED_SECTION
#	define INIT_CRITICAL_SECTION							INIT_SHARED_SECTION
#	define ENTER_CRITICAL_SECTION							ENTER_SHARED_SECTION
#	define LEAVE_CRITICAL_SECTION							LEAVE_SHARED_SECTION
#	define DELETE_CRITICAL_SECTION							DELETE_SHARED_SECTION
#	if defined(LLC_ANDROID) || defined(LLC_LINUX)
#		define llc_sync_increment								sync_increment
#		define llc_sync_decrement								sync_decrement
#		define llc_sync_exchange								sync_exchange 
#	elif defined(LLC_WINDOWS)  
#		if (defined( _WIN64 ) || defined( WIN64 )) 
#			define llc_sync_increment(nCount)						( InterlockedIncrement64( &nCount ) )
#			define llc_sync_decrement(nCount)						( InterlockedDecrement64( &nCount ) )
#			define llc_sync_exchange(nCount)						( InterlockedExchange64 ( &nCount ) )
#		elif (defined( _WIN32 ) || defined( WIN32 ))				 
#			define llc_sync_increment								sync_increment
#			define llc_sync_decrement								sync_decrement
#			define llc_sync_exchange								sync_exchange 
#		endif
#	else
#		define llc_sync_increment								sync_increment
#		define llc_sync_decrement								sync_decrement
#		define llc_sync_exchange								sync_exchange 
#	endif
#else
#	define llc_sync_increment(nCount)						(++(nCount))
#	define llc_sync_decrement(nCount)						(--(nCount))
#	define llc_sync_exchange(Target, Value)					(Target = Value)

#	define DECLARE_CRITICAL_SECTION(...)	
#	define INIT_CRITICAL_SECTION(...)						(1)
#	define ENTER_CRITICAL_SECTION(...)						(0)
#	define LEAVE_CRITICAL_SECTION(...)						(0)
#	define DELETE_CRITICAL_SECTION(...)						do {} while(false)
#endif
}

#endif // LLC_SYNC_H_023749082374
