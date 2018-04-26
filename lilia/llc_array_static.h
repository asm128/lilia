#include "llc_array_view.h"

#ifndef LLC_ARRAY_STATIC_H_2983749823749826534465243
#define LLC_ARRAY_STATIC_H_2983749823749826534465243
namespace llc
{
	template<typename _tCell, uint32_t _sizeArray>
	struct array_static	{
							_tCell						Storage	[_sizeArray]						;
	
							operator					array_view<_tCell>							()												{ return {Storage, _sizeArray}; }
							operator					array_view<const _tCell>					()										const	{ return {Storage, _sizeArray}; }

		inline				const _tCell&				operator[]									(uint32_t index)						const											{
			throw_if(index >= _sizeArray, ::std::exception(""), "Invalid index: %i. Size: %i.", (int32_t)index, (int32_t)_sizeArray);
			return Storage[index];
		}
		inline				_tCell&						operator[]									(uint32_t index)																		{
			throw_if(index >= _sizeArray, ::std::exception(""),  "Invalid index: %i. Size: %i.", (int32_t)index, (int32_t)_sizeArray);
			return Storage[index];
		}
		//constexpr										array_static								()																						= default;
		//												array_static								(::std::initializer_list<_tCell> init)													{ 
		//	throw_if(errored(init.size() > _sizeArray), ::std::exception(), "Failed to resize array! Why?");
		//	for(uint32_t i = 0, count = (uint32_t)init.size(); i < count; ++i)
		//		Storage[i]										= *(init.begin() + i);
		//}
	};
} // namespace

#endif // LLC_ARRAY_STATIC_H_2983749823749826534465243