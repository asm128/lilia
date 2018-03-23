#include "llc_grid_view.h"		// for grid_view<>
#include "llc_ascii_target.h"	// for SASCIITarget

#ifndef LLC_ASCII_DISPLAY_H_34234234
#define LLC_ASCII_DISPLAY_H_34234234

namespace llc
{
					::llc::error_t									asciiDisplayCreate					(uint32_t frontBufferWidth, uint32_t frontBufferHeight);
					::llc::error_t									asciiDisplayDestroy					();
					::llc::error_t									asciiDisplayPresent					(const ::llc::view_const_uint8& characters, const ::llc::view_const_uint16& colorRefs);
					::llc::error_t									asciiDisplayClear					(uint8_t character = ' ', uint16_t colorRef = ::llc::ASCII_COLOR_WHITE);
					::llc::error_t									asciiDisplaySize					(::llc::SCoord2<uint32_t>& size);

					::llc::error_t									asciiDisplayResize					(uint32_t frontBufferWidth, uint32_t frontBufferHeight);
					::llc::error_t									asciiDisplayPaletteSet				(const ::llc::array_view<::llc::SColorRGBA>& palette);
					::llc::error_t									asciiDisplayPaletteReset			();
					::llc::error_t									asciiDisplayTitleSet				(const ::llc::view_const_string& title);

	template <typename _tUnit>
	static inline	::llc::error_t									asciiDisplayCreate					(const ::llc::SCoord2<_tUnit>& displaySize)				{ return ::llc::asciiDisplayCreate(displaySize.x, displaySize.y); }
	static inline	::llc::error_t									asciiDisplayPresent					(const ::llc::SASCIITarget& renderTarget)				{
		return ::llc::asciiDisplayPresent
			( ::llc::view_const_uint8 {renderTarget.Characters	.begin(), renderTarget.Characters	.size()}
			, ::llc::view_const_uint16{renderTarget.Colors		.begin(), renderTarget.Colors		.size()}
			);
	}
}

#endif // LLC_ASCII_DISPLAY_H_34234234
