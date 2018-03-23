#include "llc_log.h"
#include "llc_eval.h"

#include <exception>	// for ::std::exception

#ifndef LLC_ARRAY_VIEW_BIT_H_9276349872384
#define LLC_ARRAY_VIEW_BIT_H_9276349872384

namespace llc
{
	template <typename _tElement>
	struct bit_array_view_proxy {
							_tElement									& Element;
							uint8_t										Offset;

							operator									bool						()																	const				{ return Element & (1ULL << Offset); }
							bit_array_view_proxy&						operator=					(bool value)																			{ value ? Element |= (1ULL << Offset) : Element &= ~(1ULL << Offset); return *this; }
	};

	template <typename _tElement>
	struct bit_array_view_iterator {
		static constexpr	const uint32_t								ELEMENT_BITS				= sizeof(_tElement) * 8;

							const _tElement								* Begin;
							const _tElement								* End;
							_tElement									* Element;
							int8_t										Offset;

		inline constexpr	operator									bool						()																	const				{ throw_if(Element >= End, ::std::exception(""), "Out of range."); return (*Element) & (1 << Offset); }
		inline constexpr	bool										operator==					(const bit_array_view_iterator& other)								const	noexcept	{ return Element == other.Element; }
		inline constexpr	bool										operator!=					(const bit_array_view_iterator& other)								const	noexcept	{ return Element != other.Element; }

		inline				bit_array_view_iterator&					operator=					(bool value)																			{ value ? *Element |= (1 << Offset) : *Element &= ~(1 << Offset); return *this; }
							bit_array_view_iterator&					operator++					()																						{ ++Offset; if(Offset >= ELEMENT_BITS)	{ ++Element; Offset = 0;				throw_if(Element >= (End	+ 1), ::std::exception(""), "Out of range"); } return *this; }
							bit_array_view_iterator&					operator--					()																						{ --Offset; if(Offset < 0)				{ --Element; Offset = ELEMENT_BITS - 1; throw_if(Element <  (Begin	- 1), ::std::exception(""), "Out of range"); } return *this; }
							bit_array_view_iterator						operator++					(int)																					{ 
			bit_array_view_iterator												result						(*this);	// Make a copy.
			++(*this);																									// Use the prefix version to do the work.
			return result;																								// Return the old value.
		}
							bit_array_view_iterator						operator--					(int)																					{ 
			bit_array_view_iterator												result						(*this);	// Make a copy.
			--(*this);																									// Use the prefix version to do the work.
			return result;																								// Return the old value.
		}
	};

	template <typename _tElement>
	class bit_array_view {
	protected:
		// Properties / Member Variables
		static constexpr	const uint32_t								ELEMENT_BITS				= sizeof(_tElement) * 8;

							_tElement									* Data						= 0;
							uint32_t									Count						= 0;
	public:
		typedef				_tElement									TElement;
		typedef				bit_array_view_iterator<_tElement>			iterator;

		// Constructors
		inline constexpr												bit_array_view				()																			noexcept	= default;
		inline															bit_array_view				(_tElement* dataElements, uint32_t elementCount)										: Data(dataElements), Count(elementCount)										{
			throw_if(0 == dataElements && 0 != elementCount, ::std::exception(""), "Invalid parameters.");	// Crash if we received invalid parameters in order to prevent further malfunctioning.
		}

		template <size_t _elementCount>
		inline constexpr												bit_array_view				(_tElement (&_dataElements)[_elementCount])									noexcept	: Data(_dataElements), Count(_elementCount * ELEMENT_BITS)											{}

		template <size_t _elementCount>
		inline constexpr												bit_array_view				(_tElement (&_dataElements)[_elementCount], uint32_t elementCount)						: Data(_dataElements), Count(::llc::min(_elementCount * ELEMENT_BITS, elementCount))	{
			throw_if(elementCount > (_elementCount * ELEMENT_BITS), ::std::exception(""), "Out of range count.");
		}

		// Operators
							bit_array_view_proxy<_tElement>				operator[]					(uint32_t index)																		{ 
			throw_if(0 == Data, ::std::exception(""), "Uninitialized array pointer."); 
			throw_if(index >= Count, ::std::exception(""), "Invalid index: %u.", index); 
			const uint32_t														offsetRow					= index / ELEMENT_BITS;
			const uint32_t														offsetBit					= index % ELEMENT_BITS;
			_tElement															& selectedElement			= Data[offsetRow];
			return {selectedElement, (uint8_t)offsetBit};
		}

							bool										operator[]					(uint32_t index)													const				{ 
			throw_if(0 == Data, ::std::exception(""), "Uninitialized array pointer."); 
			throw_if(index >= Count, ::std::exception(""), "Invalid index: %u.", index); 
			const uint32_t														offsetElement				= index / ELEMENT_BITS;
			const uint32_t														offsetLocal					= index % ELEMENT_BITS;
			const _tElement														& selectedElement			= Data[offsetElement];
			return (selectedElement & (1ULL << offsetLocal)); 
		}

		// Methods
		inline				bit_array_view_iterator<_tElement>			begin						()																			noexcept	{ return {Data, Data + Count / ELEMENT_BITS, Data, 0};							}
		inline				bit_array_view_iterator<_tElement>			end							()																			noexcept	{ return {Data, Data + Count / ELEMENT_BITS, Data + Count / ELEMENT_BITS, 0};	}

		inline constexpr	const bit_array_view_iterator<_tElement>	begin						()																	const	noexcept	{ return {Data, Data + Count / ELEMENT_BITS, Data, 0};							}
		inline constexpr	const bit_array_view_iterator<_tElement>	end							()																	const	noexcept	{ return {Data, Data + Count / ELEMENT_BITS, Data + Count / ELEMENT_BITS, 0};	}

		inline constexpr	uint32_t									size						()																	const	noexcept	{ return Count;																	}
	};
} // namespace

#endif // LLC_ARRAY_VIEW_BIT_H_9276349872384
