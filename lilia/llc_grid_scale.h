#include "llc_grid_view.h"

#ifndef LLC_GRID_SCALE_20654834
#define LLC_GRID_SCALE_20654834

namespace llc
{
	template<typename _tCell>
						::llc::error_t							grid_scale_fast					(::llc::grid_view<_tCell>& dst, const ::llc::grid_view<_tCell>& src, const float xFactor, const float yFactor)		{
		for(uint32_t y = 0; y < dst.height(); ++y) 
			for(uint32_t x = 0; x < dst.width(); ++x) 
				dst[y][x]													= src[(uint32_t)(y * yFactor)][(uint32_t)(x * xFactor)];
		return 0;
	}

	template<typename _tCell>
						::llc::error_t							grid_scale					(::llc::grid_view<_tCell>& dst, const ::llc::grid_view<_tCell>& src)													{
		const float														xFactor						= src.width () / (float)dst.width ();
		const float														yFactor						= src.height() / (float)dst.height();
		return grid_scale_fast(dst, src, xFactor, yFactor);
	}

	template<typename _tCell>
						::llc::error_t							grid_scale					(::llc::grid_view<_tCell>& dst, const ::llc::grid_view<_tCell>& src, const float xFactor, const float yFactor)			{
		for(uint32_t y = 0; y < dst.height(); ++y) 
			for(uint32_t x = 0; x < dst.width(); ++x) { 
				const uint32_t													xSrc						= (uint32_t)(x * yFactor);
				const uint32_t													ySrc						= (uint32_t)(y * xFactor);
				if( ::llc::in_range(ySrc, 0, src.height())
				 && ::llc::in_range(xSrc, 0, src.width ())
				 )
					dst[y][x]													= src[ySrc][xSrc];
			}
		return 0;
	}
}

#endif // LLC_GRID_SCALE_20983429834
