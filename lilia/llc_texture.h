#include "llc_array.h"
#include "llc_grid_view.h"
#include "llc_bit_array_view.h"

#ifndef LLC_TEXTURE_H_902387498237
#define LLC_TEXTURE_H_902387498237

namespace llc
{
	template<typename _tTexel>
	struct STexture {
		typedef				_tTexel												TTexel;

							::llc::array_pod<_tTexel>							Texels										;
							::llc::grid_view<_tTexel>							View										;

		constexpr																STexture									()													= default;
																				STexture									(const ::llc::grid_view<_tTexel>& other)				{ 
			Texels																	= other;
			View																	= {Texels.begin(), other.metrics()};
			return *this; 
		}

																				STexture									(const ::llc::STexture<_tTexel>& other)				{ 
			Texels																	= other.Texels;
			View																	= {Texels.begin(), other.View.metrics()};
			return *this; 
		}

							::llc::STexture<_tTexel>&							operator=									(const ::llc::grid_view<_tTexel>& other)			{ 
			Texels																	= {other.begin(), other.size()};
			View																	= {Texels.begin(), other.metrics()};
			return *this; 
		}

							::llc::STexture<_tTexel>&							operator=									(const ::llc::STexture<_tTexel>& other)				{ 
			Texels																	= other.Texels;
			View																	= {Texels.begin(), other.View.metrics()};
			return *this; 
		}


		inline				::llc::array_view<_tTexel>							operator[]									(uint32_t index)									{ return View[index]; }
		inline	const		::llc::array_view<_tTexel>							operator[]									(uint32_t index)	const							{ return View[index]; }

							::llc::error_t										resize										(uint32_t newSizeX, uint32_t newSizeY)				{ llc_necall(Texels.resize(newSizeX * newSizeY), "cannot resize?"); View = {Texels.begin(), newSizeX, newSizeY}; return 0; }
		inline				::llc::error_t										resize										(::llc::SCoord2<uint32_t> newSize)					{ return resize(newSize.x, newSize.y); }
	}; // struct

	template<typename _tTexel>
	struct STextureMonochrome {
		typedef				_tTexel												TTexel;

							::llc::array_pod		<_tTexel>					Texels										;
							::llc::bit_array_view	<_tTexel>					View										;
							uint32_t											Pitch										= 0;

		constexpr																STextureMonochrome							()													= default;
																				STextureMonochrome							(const ::llc::bit_array_view<_tTexel>& other)		{ 
			Texels																	= other;
			View																	= {Texels.begin(), Texels.size()};
			Pitch																	= other.Pitch;
			return *this; 
		}

																				STextureMonochrome							(const ::llc::STextureMonochrome<_tTexel>& other)	{ 
			Texels																	= other.Texels;
			View																	= {Texels.begin(), Texels.size()};
			Pitch																	= other.Pitch;
			return *this; 
		}

							::llc::STextureMonochrome<_tTexel>&					operator=									(const ::llc::bit_array_view<_tTexel>& other)		{ 
			Texels																	= other;
			View																	= {Texels.begin(), Texels.size()};
			Pitch																	= other.Pitch;
			return *this; 
		}

							::llc::STextureMonochrome<_tTexel>&					operator=									(const ::llc::STextureMonochrome<_tTexel>& other)	{ 
			Texels																	= other.Texels;
			View																	= {Texels.begin(), Texels.size()};
			Pitch																	= other.Pitch;
			return *this; 
		}

							::llc::error_t										resize										(uint32_t newSizeX, uint32_t newSizeY)				{ llc_necall(Texels.resize(newSizeX * newSizeY), "cannot resize?"); View = {Texels.begin(), newSizeX * newSizeY}; Pitch = newSizeX; return 0; }
		inline				::llc::error_t										resize										(const ::llc::SCoord2<uint32_t> & newSize)			{ return resize(newSize.x, newSize.y); }
	}; // struct



	template<typename _tTexel>
	struct STextureProcessable {
		typedef				_tTexel												TTexel;

							::llc::STexture<_tTexel>							Original;
							::llc::STexture<_tTexel>							Processed;
	}; // struct
} // namespace 

#endif // LLC_TEXTURE_H_902387498237
