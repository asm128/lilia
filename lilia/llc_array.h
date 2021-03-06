#include "llc_array_view.h"
#include "llc_memory.h"
#include <memory.h>

#ifndef LLC_ARRAY_H_29837498237498237429837
#define LLC_ARRAY_H_29837498237498237429837

namespace llc 
{
	template <typename... _Args>	void			clear					(_Args&&... args)						{ const int32_t results[] = {args.clear()..., 0}; }
	template <typename... _Args>	::llc::error_t	resize					(uint32_t newSize, _Args&&... args)		{ 
		const uint32_t										oldSizes	[]			= {args.size	()			..., 0}; 
		const int32_t										results		[]			= {args.resize	(newSize)	..., 0}; 
		for(uint32_t i=0; i < ::llc::size(results); ++i)
			if(errored(results[i])) {
				error_printf("Failed to set size: %i. Out of memory?", (int32_t)newSize);
				const int32_t dummy	[] = {args.resize(oldSizes[0])..., 0}; 
				dummy;
				return -1;
			}
		return 0; 
	}

	// Base for arrays that keeps track of its actual size.
	template<typename _tCell>
	struct array_base : public array_view<_tCell> {
	protected:
		using				array_view<_tCell>::		Count;
							uint32_t					Size										= 0;

		inline constexpr								array_base									()																			noexcept	= default;
		inline constexpr								array_base									(const array_base<_tCell>&	other)											noexcept	= delete;
		inline constexpr								array_base									(const array_base<_tCell>&&	other)											noexcept	= delete;

							array_base<_tCell>&			operator =									(const array_base<_tCell>&	other)														= delete;
							array_base<_tCell>&			operator =									(const array_base<_tCell>&&	other)														= delete;
		// This helper method is used to prevent redundancies. It returns a safe integer of the same or a higher value than the one passed as argument.
		inline constexpr	uint32_t					calc_reserve_size							(const uint32_t newSize)											const	noexcept	{ return ::llc::max(newSize, newSize + ::llc::max(newSize >> 1, 4U));					}
		inline constexpr	uint32_t					calc_malloc_size							(const uint32_t newSize)											const	noexcept	{ return ::llc::max(newSize*(uint32_t)sizeof(_tCell), Count*(uint32_t)sizeof(_tCell));	}
	}; // array_base

	// This class is optimized to contain POD instances and won't work for C++ objects that require calling constructors/destructors.
	template<typename _tPOD>
	struct array_pod : public array_base<_tPOD> {
		typedef				array_base<_tPOD>			TArrayBase									;
		typedef				array_view<_tPOD>			TArrayView									;

		using				TArrayBase::				Count										;
		using				TArrayBase::				Data										;
		using				TArrayBase::				Size										;
		using				TArrayBase::				calc_reserve_size							;
		using				TArrayBase::				calc_malloc_size							;
		using				TArrayBase::				operator[]									;

		inline											~array_pod									()																						{ safe_llc_free(Data);		}
		inline constexpr								array_pod									()																			noexcept	= default;
		inline											array_pod									(uint32_t initialSize)																	{ resize(initialSize);		}
														array_pod									(::std::initializer_list<_tPOD> init)													{ 
			throw_if(errored(resize((uint32_t)init.size())), ::std::exception(), "Failed to resize array! Why?");
			memcpy(Data, init.begin(), Count * sizeof(_tPOD));
		}

		//template<size_t sizeStatic>
		//												array_pod									(const _tPOD (&init)[sizeStatic])														{ 
		//	throw_if(errored(resize(sizeStatic)), ::std::exception(), "Failed to resize array! Why?");
		//	memcpy(Data, init, Count * sizeof(_tPOD));
		//}
														array_pod									(array_pod<_tPOD>&& other)													noexcept	{
			Size											= other.Size									;
			Count											= other.Count									;
			Data											= other.Data									;

			other.Size										= other.Count									= 0;
			other.Data										= 0;
		}
														array_pod									(const array_view<_tPOD>& other)														{
			if(other.Count) {
				const uint32_t										newSize										= other.Count;
				const uint32_t										reserveSize									= calc_reserve_size(newSize);
				uint32_t											mallocSize									= calc_malloc_size(reserveSize);
				throw_if(mallocSize != (reserveSize * (uint32_t)sizeof(_tPOD)), "", "Alloc size overflow. Requested size: %u. malloc size: %u.", reserveSize, mallocSize);
				::llc::auto_llc_free								safeguard;
				Data											= (_tPOD*)(safeguard.Handle = ::llc::llc_malloc(mallocSize));
				throw_if(0 == Data			, "", "Failed to allocate array. Requested size: %u. ", (uint32_t)newSize);
				throw_if(0 == other.Data	, "", "%s", "other.Data is null!");
				memcpy(Data, other.Data, newSize * sizeof(_tPOD));
				//for(uint32_t i = 0, count = newSize; i<count; ++i)
				//	Data[i]											= other[i];
				Size											= (uint32_t)reserveSize;
				Count											= other.Count;
				safeguard.Handle								= 0;
			}
		}
														array_pod									(const array_pod<_tPOD>& other)														{
			if(other.Count) {
				const uint32_t										newSize										= other.Count;
				const uint32_t										reserveSize									= calc_reserve_size(newSize);
				uint32_t											mallocSize									= calc_malloc_size(reserveSize);
				throw_if(mallocSize != (reserveSize * (uint32_t)sizeof(_tPOD)), "", "Alloc size overflow. Requested size: %u. malloc size: %u.", reserveSize, mallocSize);
				::llc::auto_llc_free								safeguard;
				Data											= (_tPOD*)(safeguard.Handle = ::llc::llc_malloc(mallocSize));
				throw_if(0 == Data			, "", "Failed to allocate array. Requested size: %u. ", (uint32_t)newSize);
				throw_if(0 == other.Data	, "", "%s", "other.Data is null!");
				memcpy(Data, other.Data, newSize * sizeof(_tPOD));
				//for(uint32_t i = 0, count = newSize; i<count; ++i)
				//	Data[i]											= other[i];
				Size											= (uint32_t)reserveSize;
				Count											= other.Count;
				safeguard.Handle								= 0;
			}
		}
		template <size_t _otherCount>
														array_pod									(const _tPOD (&other)[_otherCount])														{
			if(other.Count) {
				const uint32_t										newSize										= other.Count;
				const uint32_t										reserveSize									= calc_reserve_size(newSize);
				uint32_t											mallocSize									= calc_malloc_size(reserveSize);
				throw_if(mallocSize != (reserveSize * (uint32_t)sizeof(_tPOD)), "", "Alloc size overflow. Requested size: %u. malloc size: %u.", reserveSize, mallocSize);
				::llc::auto_llc_free								safeguard;
				Data											= (_tPOD*)(safeguard.Handle = ::llc::llc_malloc(mallocSize));
				throw_if(0 == Data			, "", "Failed to allocate array. Requested size: %u. ", (uint32_t)newSize);
				throw_if(0 == other.Data	, "", "%s", "other.Data is null!");
				memcpy(Data, other, newSize * sizeof(_tPOD));
				//for(uint32_t i = 0, count = newSize; i < count; ++i)
				//	Data[i]											= other[i];
				Size											= (uint32_t)reserveSize;
				Count											= other.Count;
				safeguard.Handle								= 0;
			} // 
		}
							array_pod<_tPOD>&			operator =									(const array_view<_tPOD>& other)														{
			throw_if(resize(other.Count) != (int32_t)other.Count, "", "Failed to assign array.");
			memcpy(Data, other, other.Count * sizeof(_tPOD));
			//for(uint32_t iElement = 0; iElement < other.Count; ++iElement)
			//	operator[](iElement)							= other[iElement];
			return *this;
		}
		template<size_t sizeStatic>
							array_pod<_tPOD>&			operator =									(const _tPOD (&init)[sizeStatic])														{ 
			throw_if(errored(resize(sizeStatic)), ::std::exception(), "Failed to resize array! Why?");
			memcpy(Data, init, Count * sizeof(_tPOD));
			return *this;
		}

		// This method doesn't call destructors of the contained PODs.
		inline				::llc::error_t				clear										()																			noexcept	{ return Count = 0; }

		// Returns the new size of the array
		inline				::llc::error_t				pop_back									(_tPOD* oldValue)																		{ 
			ree_if(0 == Count, "%s", "Cannot pop elements of an empty array."); 
			--Count;
			safe_podcpy(oldValue, &Data[Count]); 
			return Count; 
		}

		// Returns the index of the pushed value or -1 on failure
							::llc::error_t				push_back									(const _tPOD& newValue)																	{ 
			const int32_t										indexExpected								= Count;
			const int32_t										indexNew									= resize(Count+1, newValue)-1; 
			ree_if(indexNew != indexExpected, "Failed to push value! Array size: %i.", indexExpected);
			return indexNew;
		}

		// Returns the index of the pushed value
		template<size_t _Length>
		inline				::llc::error_t				append										(const _tPOD (&newChain)[_Length])														{ return append(newChain, _Length);		}
							::llc::error_t				append										(const _tPOD* chainToAppend, uint32_t chainLength)										{ 
			const uint32_t										startIndex									= Count;
			const uint32_t										requestedSize								= Count + chainLength; 
			ree_if(requestedSize < Count, "Size overflow. Cannot append chain.");
			const int32_t										newSize										= resize(requestedSize); 
			ree_if(newSize != (int32_t)requestedSize, "Failed to resize array for appending.");

			//for(uint32_t i = 0, maxCount = ::llc::min(chainLength, newSize - startIndex); i < maxCount; ++i)
				//Data[startIndex + i]							= chainToAppend[i];
			memcpy(&Data[startIndex], chainToAppend, sizeof(_tPOD) * chainLength);
			return startIndex;
		}

		// Returns the new size of the array.
							::llc::error_t				resize										(uint32_t newSize, const _tPOD& newValue)												{ 
			int32_t												oldCount									= (int32_t)Count;
			int32_t												newCount									= resize(newSize);
			ree_if(newCount != (int32_t)newSize, "Failed to resize array. Requested size: %u. Current size: %u (%u).", newCount, Count, Size);
			for( int32_t i = oldCount; i < newCount; ++i )
				::llc::podcpy(&Data[i], &newValue);
			return newCount;			
		}

		// Returns the new size of the array.
							::llc::error_t				resize										(uint32_t newSize)																		{ 
			const uint32_t										oldCount									= Count;
			if(newSize >= Size) {
				_tPOD												* oldData									= Data;
				uint32_t											reserveSize									= calc_reserve_size(newSize);
				uint32_t											mallocSize									= calc_malloc_size(reserveSize);
				ree_if(mallocSize != (reserveSize * (uint32_t)sizeof(_tPOD)), "Alloc size overflow. Requested size: %u. malloc size: %u.", reserveSize, mallocSize);
				::llc::auto_llc_free								safeguard;
				_tPOD												* newData									= (_tPOD*)(safeguard.Handle = ::llc::llc_malloc(mallocSize));
				ree_if(nullptr == newData, "Failed to resize array. Requested size: %u. Current size: %u.", newSize, (uint32_t)Size);

				//TArrayView											safe_data									= {newData, reserveSize};
				if(oldData) {
					memcpy(newData, Data, ::llc::min(newSize, oldCount) * sizeof(_tPOD));
					//for(uint32_t i = 0, count = ::llc::min(newSize, oldCount); i<count; ++i)
					//	safe_data[i]									= operator[](i);
				}
				Size											= (uint32_t)reserveSize;
				Count											= newSize;
				Data											= newData;
				safeguard.Handle								= 0;
				if(oldData)
					::llc::llc_free(oldData);
			}
			else
				Count											= newSize;

			return (int32_t)Count;
		}

		// returns the new size of the list or -1 on failure.
							::llc::error_t				insert										(uint32_t index, const _tPOD& newValue)													{ 
			ree_if(index >= Count, "Invalid index: %u.", index);
			if(Size < Count + 1) {
				_tPOD												* oldData									= Data;
				uint32_t											reserveSize									= calc_reserve_size(Count + 1);
				uint32_t											mallocSize									= calc_malloc_size(reserveSize);
				ree_if(mallocSize != (reserveSize * (uint32_t)sizeof(_tPOD)), "Alloc size overflow. Requested size: %u. malloc size: %u.", reserveSize, mallocSize);
				::llc::auto_llc_free								safeguard;
				_tPOD												* newData									= (_tPOD*)(safeguard.Handle = ::llc::llc_malloc(mallocSize));
				ree_if(nullptr == newData, "Failed to allocate array for inserting new value.");

				TArrayView											viewSafe									= {newData, Count+1};
				memcpy(viewSafe.begin(), oldData, ::llc::min(index, Count) * sizeof(_tPOD));
				//for(uint32_t i = 0, maxCount = ::llc::min(index, Count); i < maxCount; ++i)
				//	viewSafe[i]										= oldData[i];
				viewSafe[index]									= newValue;
				//if(index < Count)
				//	memcpy(&viewSafe[index + 1], oldData, (Count - index) * sizeof(_tPOD));
				for(uint32_t i = index, maxCount = ::llc::min(index + 1, Count); i < maxCount; ++i) 
					viewSafe[i + 1]									= oldData[i];
				Data											= newData;
				safeguard.Handle								= 0;
			}	
			else {
				for(int32_t i = (int32_t)::llc::min(index, Count - 1), maxCount = (int32_t)index; i >= maxCount; --i) 
					Data[i + 1]										= Data[i];

				Data[index]										= newValue;
			}			
			return ++Count;
		}

		// returns the new size of the list or -1 on failure.
							::llc::error_t				insert										(uint32_t index, const _tPOD* chainToInsert, uint32_t chainLength)						{
			ree_if(index >= Count, "Invalid index: %u.", index);

			if(Size < Count + chainLength) {
				_tPOD												* oldData									= Data;
				uint32_t											newSize										= calc_reserve_size(Count + chainLength);
				uint32_t											mallocSize									= calc_malloc_size(newSize);
				ree_if(mallocSize != (newSize * (uint32_t)sizeof(_tPOD)), "Alloc size overflow. Requested size: %u. malloc size: %u.", newSize, mallocSize);
				::llc::auto_llc_free								safeguard;
				_tPOD												* newData									= (_tPOD*)(safeguard.Handle = ::llc::llc_malloc(mallocSize));
				ree_if(nullptr == newData, "Failed to allocate array for inserting new value.");
				::llc::array_view<_tPOD>							viewSafe									= {newData, newSize};
				for(uint32_t i = 0					, maxCount = ::llc::min(index, Count)				; i <	maxCount; ++i)	viewSafe[i			]	= oldData[i];
				for(uint32_t i = 0					, maxCount = ::llc::min(chainLength, newSize-index)	; i <	maxCount; ++i)	viewSafe[i + index	]	= chainToInsert[i];
				for(uint32_t i = index + chainLength, maxCount = ::llc::min(index + 1, Count)			; i <	maxCount; ++i)	viewSafe[i + 1		]	= oldData[i];
				Data											= newData;
				safeguard.Handle								= 0;
			}	
			else {	// no need to reallocate and copy, just shift rightmost elements and insert in-place
				for(int32_t  i = (int32_t)::llc::min(index, Count - 1), maxCount = (int32_t)index	; i >=	maxCount; --i)	Data[i + chainLength]	= Data[i];
				for(uint32_t i = 0, maxCount = ::llc::min(chainLength, Count - index)				; i <	maxCount; ++i)	Data[i + index		]	= chainToInsert[i];
			}
			return Count += chainLength;
		}

		template<size_t _chainLength>
		inline				::llc::error_t				insert										(uint32_t index, const _tPOD* (&chainToInsert)[_chainLength])							{ return insert(index, chainToInsert, _chainLength); }

		// Returns the new size of the list or -1 if the array pointer is not initialized.
							::llc::error_t				remove_unordered							(uint32_t index)																		{ 
			ree_if(0 == Data, "%s", "Uninitialized array pointer!");
			ree_if(index >= Count, "Invalid index: %u.", index);
			Data[index]										= Data[--Count];
			return Count;
		}

		// returns the new array size or -1 if failed.
		template<typename _tObj>
							::llc::error_t				erase										(const _tObj* address)																	{ 
			ree_if(0 == Data, "Uninitialized array pointer!");
			const ptrdiff_t										ptrDiff										= ptrdiff_t(address) - (ptrdiff_t)Data;
			const uint32_t										index										= (uint32_t)(ptrDiff / (ptrdiff_t)sizeof(_tObj));
			ree_if(index >= Count, "Invalid index: %u.", index);
			return remove(index); 
		}

		// returns the new array size or -1 if failed.
							::llc::error_t				remove										(uint32_t index)																		{ 
			ree_if(0 == Data, "Uninitialized array pointer!");
			ree_if(index >= Count, "Invalid index: %u.", index);
			--Count;
			while(index < Count) {
				memcpy(&Data[index], &Data[index + 1], sizeof(_tPOD));		// Placement new
				++index;
			}
			return Count;
		}

		// Returns the index of the argument if found or -1 if not.
		inline				::llc::error_t				find										(const _tPOD& valueToLookFor)										const				{
			for(uint32_t i = 0; i < Count; ++i) 
				if(0 == ::llc::podcmp(&Data[i], &valueToLookFor))
					return i;
			return -1;
		}

							::llc::error_t				read										(const byte_t* input, uint32_t* inout_bytesRead)										{
			uint32_t											elementsToRead								= 0;
			if(input) {
				uint32_t											elementsToRead								= *(uint32_t*)input;
				input											+= sizeof(uint32_t);
			}

			*inout_bytesRead								+= sizeof(uint32_t);
			if(0 == elementsToRead) {
				Count											= 0;
				return 0;
			}

			::llc::array_view<_tPOD>							newStorage									= {(_tPOD*)::llc::llc_malloc(sizeof(_tPOD) * elementsToRead), elementsToRead};
			ree_if(0 == newStorage.begin(), "Failed to allocate array for storing read elements.");
			if(0 == input) {
				for(uint32_t i = 0; i < Count; ++i) 
					*inout_bytesRead								+= sizeof(_tPOD);
			}
			else {
				for(uint32_t i = 0; i < Count; ++i) {
					::llc::podcpy(&newStorage[i], input);
					input											+= sizeof(_tPOD);
					*inout_bytesRead								+= sizeof(_tPOD);
				}
			}
			Count											= elementsToRead;
			Size											= elementsToRead;
			_tPOD											* old											= Data;
			Data											= newStorage.begin();
			::llc::safe_llc_free(old);
			return 0;
		}

							::llc::error_t				write										(byte_t* input, uint32_t* inout_bytesWritten)						const				{
			if(0 == input) {
				inout_bytesWritten							+= sizeof(uint32_t) + sizeof(_tPOD) * Count;	// Just return the size required to store this.
				return 0;
			}

			*(uint32_t*)input								= Count;
			input											+= sizeof(uint32_t);
			*inout_bytesWritten								+= sizeof(uint32_t);

			::llc::array_view<_tPOD>							newStorage									= {input, Count};
			for(uint32_t i = 0; i < Count; ++i) {
				::llc::podcpy(&newStorage[i], &Data[i]);
				input											+= sizeof(_tPOD);
				*inout_bytesWritten								+= sizeof(_tPOD);
			}
			return 0;
		}
	}; // array_pod

	//template<typename _tPOD>
	//struct string : public array_pod<_tPOD> {};

	//------------------------------------------------------------------------------------------------------------
	template<typename _tObj>
	struct array_obj : public array_base<_tObj> {
		typedef				array_base<_tObj>			_TVectorBase;

		using											_TVectorBase::Count;
		using											_TVectorBase::Data;
		using											_TVectorBase::Size;
		using											_TVectorBase::calc_reserve_size;
		using											_TVectorBase::calc_malloc_size;
		using											_TVectorBase::operator[];

		inline											~array_obj									()																						{ for(uint32_t i = 0; i < Count; ++i) Data[i].~_tObj(); safe_llc_free(Data); }	// dtor
		inline constexpr								array_obj									()																						= default;
		inline											array_obj									(const uint32_t newSize)																{ throw_if(((int32_t)newSize) != resize(newSize), "", "%s", "Failed to resize array."); }
		inline											array_obj									(array_obj<_tObj>&& other)													noexcept	{
			Size											= other.Size									;
			Count											= other.Count									;
			Data											= other.Data									;

			other.Size										= other.Count									= 0;
			other.Data										= 0;
		}	// move ctor
														array_obj									(const array_obj<_tObj>& other)															{
			if(other.Count) {
				uint32_t											newSize										= other.Count;
				uint32_t											reserveSize									= calc_reserve_size(newSize);
				uint32_t											mallocSize									= calc_malloc_size(reserveSize);
				throw_if(mallocSize != (reserveSize*(uint32_t)sizeof(_tObj)), "", "Alloc size overflow. Requested size: %u. malloc size: %u.", reserveSize, mallocSize);
				::llc::auto_llc_free								safeguard;
				_tObj												* newData									= (_tObj*)(safeguard.Handle = ::llc::llc_malloc(mallocSize));
				throw_if(0 == newData		, "", "Failed to resize array. Requested size: %u. Current size: %u.", (uint32_t)newSize, (uint32_t)Size);
				throw_if(0 == other.Data	, "", "%s", "other.Data is null!");
				for(uint32_t i = 0; i < newSize; ++i) 
					new (&newData[i]) _tObj(other.Data[i]);
				Data											= newData;
				Size											= reserveSize;
				Count											= other.Count;
				safeguard.Handle								= 0;
			}
		}
		inline				array_obj<_tObj>&			operator =									(const array_obj<_tObj>& other)															{
			throw_if(resize(other.Count) != (int32_t)other.Count, "", "Failed to resize array!");
			for(uint32_t iElement = 0; iElement < other.Count; ++iElement)
				this->operator[](iElement)							= other[iElement];
			return *this;
		}
		inline				int32_t						clear										()																						{ 
			for(uint32_t i = 0; i < Count; ++i)
				Data[i].~_tObj();
			return Count									= 0; 
		}
		// Returns the new size of the array
		inline				int32_t						pop_back									(_tObj* oldValue)																		{ 
			ree_if(0 == Count, "%s", "Cannot pop elements from an empty array."); 
			--Count;
			safe_assign(oldValue, Data[Count]); 
			Data[Count].~_tObj();
			return Count; 
		}
		// Returns the index of the pushed value or -1 on failure
		inline				int32_t						push_back									(const _tObj& newValue)																	{ 
			int32_t												indexExpected								= Count;
			int32_t												indexNew									= resize(Count+1, newValue)-1; 
			ree_if(indexNew != indexExpected, "Failed to push value! Array size: %i.", indexExpected);
			return indexNew;
		}

		// returns the new array size or -1 if failed.
		template<typename _tObj>
		inline				int32_t						erase										(const _tObj* address)																	{ 
			ree_if(0 == Data, "Uninitialized array pointer!");
			const ptrdiff_t										ptrDiff										= ptrdiff_t(address) - (ptrdiff_t)Data;
			const uint32_t										index										= (uint32_t)(ptrDiff / (ptrdiff_t)sizeof(_tObj));
			ree_if(index >= Count, "Invalid index: %u.", index);
			return remove(index); 
		}

							int32_t						remove										(uint32_t index)																		{ 
			ree_if(0 == Data, "Uninitialized array pointer!");
			ree_if(index >= Count, "Invalid index: %u.", index);
			--Count;
			while(index < Count) {
				Data[index].~_tObj();							// Destroy old
				new (&Data[index]) _tObj(Data[index + 1]);		// Placement new
				++index;
			}
			Data[index].~_tObj();							// Destroy last
			return Count;
		}

		// Returns the new size of the list or -1 if the array pointer is not initialized.
							int32_t						remove_unordered							(uint32_t index)																		{ 
			ree_if(0 == Data, "Uninitialized array pointer!");
			ree_if(index >= Count, "Invalid index: %u.", index);
			Data[index].~_tObj();							// Destroy old
			if(1 == Count) 
				--Count;
			else {
				new (&Data[index]) _tObj(Data[--Count]);	// Placement new
				Data[Count].~_tObj();						// Destroy reordered
			}

			return Count;
		}
		// returns the new size of the list or -1 on failure.
							int32_t						insert										(uint32_t index, const _tObj& newValue)													{ 
			ree_if(index >= Count, "Invalid index: %u.", index);

			if(Size < Count + 1) {
				_tObj												* oldData								= Data;
				uint32_t											reserveSize								= calc_reserve_size(Count+1);
				uint32_t											mallocSize								= calc_malloc_size(reserveSize);
				ree_if(mallocSize != (reserveSize*(uint32_t)sizeof(_tObj)), "Alloc size overflow. Requested size: %u. malloc size: %u.", reserveSize, mallocSize);
				::llc::auto_llc_free								safeguard;
				_tObj												* newData								= (_tObj*)(safeguard.Handle = ::llc::llc_malloc(mallocSize));
				ree_if(0 == newData, "Failed to allocate for inserting new element into array! current size: %u. new size: %u.", Size, mallocSize);
				::llc::array_view<_tObj>							viewSafe								= {newData, Count+1};
				for(uint32_t i = 0, maxCount = ::llc::min(index + 1, Count); i < maxCount; ++i) {
					new (&viewSafe[i]) _tObj(oldData[i]);
					oldData[i].~_tObj();
				}
				new (&viewSafe[index]) _tObj(newValue);
				for(uint32_t i = index, maxCount = ::llc::min(index + 1, Count); i < maxCount; ++i) {
					new (&viewSafe[i + 1]) _tObj(oldData[i]);
					oldData[i].~_tObj();
				}
				Data											= newData;
				Size											= reserveSize;
				safeguard.Handle								= 0;
			}	
			else {
				for(int32_t i = (int32_t)::llc::min(index, Count - 1), maxCount = (int32_t)index; i >= maxCount; --i) {
					new (&Data[i + 1]) _tObj(Data[i]);
					Data[i].~_tObj();
				}
				new (&Data[index]) _tObj(newValue);
			}			
			return ++Count;
		}
		// Returns the new size of the array or -1 if failed.
		template <typename... _tArgs>
							int32_t						resize										(uint32_t newSize, _tArgs&&... constructorArgs)											{
			uint32_t											oldCount									= Count;
			if(newSize > Size) {
				_tObj												* oldData									= Data;
				uint32_t											reserveSize									= calc_reserve_size(newSize);
				uint32_t											mallocSize									= calc_malloc_size(reserveSize);
				ree_if(mallocSize != (reserveSize*(uint32_t)sizeof(_tObj)), "Alloc size overflow. Requested size: %u. malloc size: %u.", reserveSize, mallocSize);
				::llc::auto_llc_free								safeguard;
				_tObj												* newData									= (_tObj*)(safeguard.Handle = ::llc::llc_malloc(mallocSize));
				ree_if(nullptr == newData, "Failed to resize array. Requested size: %u. Current size: %u.", newSize, Size);
				if(oldData) {
					for(uint32_t i = 0, copyCount = ::llc::min(oldCount, newSize); i < copyCount; ++i)
						new (&newData[i])_tObj{oldData[i]};
					for(uint32_t i = 0; i < oldCount; ++i)
						oldData[i].~_tObj();
				}
				Data											= newData;
				Size											= reserveSize;
				Count											= (uint32_t)newSize;
				safeguard.Handle								= 0;
				if(oldData) 
					::llc::llc_free(oldData);
			}
			else {
				for(int32_t i = ((int32_t)oldCount) - 1, newCount = (int32_t)newSize; i >= newCount; --i)	// we need to cast to int32_t because otherwise --i	will underflow to UINT_MAX and loop forever.
					Data[i].~_tObj();
				Count											= (uint32_t)newSize;
			}
			for(uint32_t i = oldCount; i < Count; ++i)
				new (&Data[i])_tObj{constructorArgs...};

			return (int32_t)Count;
		}	
		//// returns the index or -1 if not found.
		//inline				int32_t						find										(const _tObj& valueToLookFor)										const				{
		//	for(uint32_t i=0; i<Count; ++i)
		//		if(Data[i] == valueToLookFor)
		//			return i;
		//	return -1;
		//}
	}; // array_obj
}

#endif // LLC_ARRAY_H_29837498237498237429837
