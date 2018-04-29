#include "llc_array_view.h"

#ifndef LLC_STREAM_VIEW_H_992834928749283
#define LLC_STREAM_VIEW_H_992834928749283

namespace llc
{
	template<typename _tElement>
	struct stream_view : public array_view<_tElement> {
		uint32_t										CursorPosition					= 0;
		
		using											array_view<_tElement>::			array_view;
		using											array_view<_tElement>::			Data;

									inline				::llc::error_t					rewd								(uint32_t positions)			{ CursorPosition += positions; return CursorPosition; }
									inline				::llc::error_t					ffwd								(uint32_t positions)			{ CursorPosition += positions; return CursorPosition; }

		template<typename _tPOD>	inline				::llc::error_t					read_pod							(_tPOD& read)					{ memcpy(&read, &Data[CursorPosition], sizeof(_tPOD));			CursorPosition += sizeof(_tPOD); return sizeof(_tPOD); }
		template<typename _tOBJ>	inline				::llc::error_t					read_obj							(_tOBJ& read)					{ read = *(_tOBJ*)&Data[CursorPosition];						CursorPosition += sizeof(_tOBJ); return sizeof(_tOBJ); }
		template<typename _tPOD>	inline				::llc::error_t					read_pods							(_tPOD* read, uint32_t nCount)	{ memcpy(read, &Data[CursorPosition], sizeof(_tPOD) * nCount);	CursorPosition += sizeof(_tPOD) * nCount; return sizeof(_tPOD) * nCount; }
		template<typename _tOBJ>						::llc::error_t					read_objs							(_tOBJ* read, uint32_t nCount)	{ 
			_tOBJ																				* start								= (_tOBJ*)&Data[CursorPosition];
			for(uint32_t i = 0; i < nCount; ++i)
				read[i]																			= start[i]; 
			return sizeof(_tOBJ) * nCount; 
		}
	};
} // namespace

#endif // LLC_STREAM_VIEW_H_992834928749283
