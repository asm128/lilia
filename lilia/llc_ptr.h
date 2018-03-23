#include "llc_sync.h"
#include "llc_log.h"

#ifndef LLC_PTR_H_0297349827348923
#define LLC_PTR_H_0297349827348923

namespace llc 
{
	template<typename _tInstance>
	struct llc_ref {
		typedef		_tInstance									TInstance;

					_tInstance									* Instance;
					refcount_t									References;
	};
	
	template<typename _tNCO>
						::llc::llc_ref<_tNCO> *				ref_acquire							(::llc::llc_ref<_tNCO>* llc_reference)									{
		if(llc_reference)
			llc_sync_increment(llc_reference->References);
		return llc_reference;
	};

	template<typename _tNCO>
						::llc::error_t						ref_release							(::llc::llc_ref<_tNCO>** llc_reference)									{
		typedef	::llc::llc_ref<_tNCO>								TRef;
		TRef														* oldRef							= *llc_reference;
		*llc_reference											= 0;
		if(oldRef)
			switch(llc_sync_decrement(oldRef->References)) {
			case -1: error_printf("Reference count error!"); return -1;
			case  0:
				if(oldRef->Instance)
					oldRef->Instance->~_tNCO();
				else
					error_printf("Instance is NULL! At the point of writing this code it wouldn't make any sense.");
				::llc::llc_free(oldRef->Instance);
				::llc::llc_free(oldRef);
				break;
			}
		return 0;
	};

	template<typename _tOBJ>
						_tOBJ *								ref_allocate						(::llc::llc_ref<_tOBJ>** llc_reference)									{
		typedef	::llc::llc_ref<_tOBJ>								TRef;
		TRef														* newRef							= (TRef*)::llc::llc_malloc(sizeof(TRef));
		retnul_error_if(0 == newRef, "Failed to allocate reference! Out of memory?");
		if(0 == (newRef->Instance = (_tOBJ*)::llc::llc_malloc(sizeof(_tOBJ)))) {
			::llc::llc_free(newRef);
			error_printf("Failed to allocate instance! Out of memory?");
			return 0;
		}
		newRef->References										= 1;
		TRef														* oldRef							= *llc_reference;
		*llc_reference											= newRef;
		::llc::ref_release(&oldRef);
		return (*llc_reference)->Instance;
	};

	template<typename _tOBJ, typename... _tArgs>
						_tOBJ *								ref_create							(::llc::llc_ref<_tOBJ>** llc_reference, _tArgs&&... argsConstructor)	{
		typedef	::llc::llc_ref<_tOBJ>								TRef;
		TRef														* newRef							= 0;
		retnul_error_if(0 == ::llc::ref_allocate(&newRef), "Failed to allocate reference");
		new (newRef->Instance) _tOBJ{argsConstructor...};

		TRef														* oldRef							= *llc_reference;
		*llc_reference											= newRef;
		::llc::ref_release(&oldRef);
		return (*llc_reference)->Instance;
	};

	template<typename _tNCO>
	class ptr_nco {
	protected:
							::llc::llc_ref<_tNCO>				* Reference							= 0;
	public:
		typedef				::llc::ptr_nco<_tNCO>				TNCOPtr;
		typedef				::llc::llc_ref<_tNCO>				TRef;

		inline													~ptr_nco							()															noexcept	{ ::llc::ref_release(&Reference);													}
		inline constexpr										ptr_nco								()															noexcept	= default;
		inline													ptr_nco								(const TNCOPtr& other)										noexcept	{ Reference = ::llc::ref_acquire(other.Reference);									}
		inline constexpr										ptr_nco								(TNCOPtr&& other)											noexcept	{ Reference = other.Reference; other.Reference = 0;									}
		inline													ptr_nco								(TRef* other)												noexcept	{ Reference = other;																}

		inline constexpr	operator							_tNCO	*							()															noexcept	{ return Reference ? Reference->Instance : 0;										}

		inline constexpr	bool								operator==							(const TNCOPtr& other)								const	noexcept	{ return Reference == other.Reference;												}
		inline constexpr	bool								operator!=							(const TNCOPtr& other)								const	noexcept	{ return !operator==(other);														}

		inline constexpr	TNCOPtr								operator =							(const TNCOPtr& other)										noexcept	{ TRef* oldInstance = Reference; Reference = ::llc::ref_acquire(other.Reference);	::llc::ref_release(&oldInstance); return *this; }
		inline constexpr	TNCOPtr								operator =							(TNCOPtr&& other)											noexcept	{ TRef* oldInstance = Reference; Reference = other.Reference; other.Reference = 0;	::llc::ref_release(&oldInstance); return *this; }
		inline constexpr	TNCOPtr								operator =							(TRef* other)												noexcept	{ TRef* oldInstance = Reference; Reference = other;									::llc::ref_release(&oldInstance); return *this; }

		inline				_tNCO*								operator->							()															noexcept	{ return Reference->Instance; }
		inline				const _tNCO*						operator->							()													const	noexcept	{ return Reference->Instance; }

		inline				TRef**								operator &							()															noexcept	{ return &Reference;	}

		inline constexpr	const TRef*							get_ref								()													const	noexcept	{ return Reference;	}

		template<typename _tNCOOther>
		inline				_tNCO*								as									(_tNCOOther** other)										noexcept	{ return *other = (Reference ? (_tNCOOther*)Reference->Instance : 0);				}

		template<typename _tNCOOther>
		inline				_tNCO*								as									(TNCOPtr& other)											noexcept	{ other = ::llc::ref_acquire(Reference); return 0;									}
	};
	
	template<typename _tOBJ>
	class ptr_obj : public ::llc::ptr_nco<_tOBJ> {
	public:
		typedef				::llc::ptr_nco<_tOBJ>				TNCOPtr;
		typedef				::llc::ptr_obj<_tOBJ>				TOBJPtr;
		typedef				::llc::llc_ref<_tOBJ>				TRef;
		using				TNCOPtr								::Reference;

		inline				const _tOBJ*						operator->							()													const	noexcept	{ return Reference;																	}
		inline				_tOBJ*								operator->							()															noexcept	{ return (0 == Reference) ? ::llc::ref_create(&Reference) : Reference->Instance;	}

		template<typename... _tArgsConstructor>
		inline				_tOBJ*								create								(_tArgsConstructor&&... argsConstructor)					noexcept	{ return ::llc::ref_create(&Reference, argsConstructor...);							}
		inline				_tOBJ*								allocate							()															noexcept	{ return ::llc::ref_allocate(&Reference);											}
	};

	template<typename _tPOD>
	class ptr_pod : public ::llc::ptr_nco<_tPOD> {
	public:
		typedef				::llc::ptr_nco<_tPOD>				TNCOPtr;
		typedef				::llc::ptr_pod<_tPOD>				TPODPtr;
		typedef				::llc::llc_ref<_tPOD>				TRef;
		using				TNCOPtr								::Reference;

		inline				const _tPOD*						operator->							()													const	noexcept	{ return Reference->Instance;														}
		inline				_tPOD*								operator->							()															noexcept	{ return (0 == Reference) ? ::llc::ref_allocate(&Reference) : Reference->Instance;	}

		template<typename... _tArgsConstructor>
		inline				_tPOD*								create								(_tArgsConstructor&&... argsConstructor)					noexcept	{ return ::llc::ref_create(&Reference, argsConstructor...);							}
		inline				_tPOD*								allocate							()															noexcept	{ return ::llc::ref_allocate(&Reference);											}
	};
} // namespace

#endif // LLC_PTR_H_0297349827348923
