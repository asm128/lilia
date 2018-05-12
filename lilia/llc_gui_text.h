#include "llc_array.h"
#include "llc_coord.h"
#include "llc_grid_view.h"
#include "llc_bit_view.h"
#include "llc_grid_copy.h"

#ifndef LLC_GUI_TEXT_H_928734982734
#define LLC_GUI_TEXT_H_928734982734

namespace llc 
{
	struct SGUIText {
									::llc::view_const_string							Text								= {};
									::llc::SCoord2<uint32_t>							Metrics								= {};
									::llc::ALIGN										Align								= {};
	};


	static inline constexpr		::llc::error_t										textLineCalcSize							(const ::llc::SCoord2<int32_t>& sizeCharCell, const ::llc::view_const_string& text0)	{ return (::llc::error_t)(sizeCharCell.x * (text0.size() - 1)); }

	template<typename _tColor>
	static						::llc::error_t										textLineDrawFixedSize							(::llc::grid_view<_tColor>& bmpTarget, const ::llc::grid_view<_tColor>& viewTextureFont, uint32_t characterCellsX, int32_t dstOffsetY, const ::llc::SCoord2<int32_t>& sizeCharCell, const ::llc::view_const_string& text0, const ::llc::SCoord2<int32_t> dstTextOffset)	{	// --- This function will draw some coloured symbols in each cell of the ASCII screen.
		for(int32_t iChar = 0, charCount = (int32_t)text0.size(); iChar < charCount; ++iChar) {
			int32_t																				coordTableX										= text0[iChar] % characterCellsX;
			int32_t																				coordTableY										= text0[iChar] / characterCellsX;
			const ::llc::SCoord2<int32_t>														coordCharTable									= {coordTableX * sizeCharCell.x, coordTableY * sizeCharCell.y};
			const ::llc::SCoord2<int32_t>														dstOffset1										= {sizeCharCell.x * iChar, dstOffsetY};
			const ::llc::SRectangle2D<int32_t>													srcRect0										= ::llc::SRectangle2D<int32_t>{{coordCharTable.x, (int32_t)viewTextureFont.height() - sizeCharCell.y - coordCharTable.y}, sizeCharCell};
			error_if(errored(::llc::grid_copy_alpha(bmpTarget, viewTextureFont, dstTextOffset + dstOffset1, srcRect0, {0xFF, 0x00, 0xFF, 0xFF})), "I believe this never fails.");
			//error_if(errored(::llc::grid_copy(bmpTarget, viewTextureFont, dstTextOffset + dstOffset1, srcRect0)), "I believe this never fails.");
		}
		return 0;
	}

	//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	template<typename _tColor>
	static				::llc::error_t												textLineDrawFixedSizeLit						(::llc::grid_view<_tColor>& bmpTarget, const ::llc::bit_view<uint32_t>& viewTextureFont, const ::llc::SCoord2<uint32_t> & viewMetrics, uint32_t characterCellsX, int32_t dstOffsetY, const ::llc::SCoord2<int32_t>& sizeCharCell, const ::llc::view_const_string& text0, const ::llc::SCoord2<int32_t> dstTextOffset, const _tColor& color)	{	// --- This function will draw some coloured symbols in each cell of the ASCII screen.
		::llc::array_pod<::llc::SCoord2<uint32_t>>										dstCoords;
		for(int32_t iChar = 0, charCount = (int32_t)text0.size(); iChar < charCount; ++iChar) {
			const int32_t																	coordTableX										= text0[iChar] % characterCellsX;
			const int32_t																	coordTableY										= text0[iChar] / characterCellsX;
			const ::llc::SCoord2<int32_t>													coordCharTable									= {coordTableX * sizeCharCell.x, coordTableY * sizeCharCell.y};
			const ::llc::SCoord2<int32_t>													dstOffset1										= {sizeCharCell.x * iChar, dstOffsetY};
			const ::llc::SRectangle2D<int32_t>												srcRect0										= ::llc::SRectangle2D<int32_t>{{coordCharTable.x, (int32_t)viewMetrics.y - sizeCharCell.y - coordCharTable.y}, sizeCharCell};
			//error_if(errored(::llc::grid_copy_alpha_bit(bmpTarget, viewTextureFont, dstTextOffset + dstOffset1, viewMetrics, color, srcRect0)), "I believe this never fails.");
			dstCoords.clear();
			error_if(errored(::llc::grid_raster_alpha_bit(bmpTarget, viewTextureFont, dstTextOffset + dstOffset1, viewMetrics, srcRect0, dstCoords)), "I believe this never fails.");
			for(uint32_t iCoord = 0; iCoord < dstCoords.size(); ++iCoord)
				::llc::drawPixelLight(bmpTarget, dstCoords[iCoord], color, 0.05f, 0.75);
		}
		return 0;
	}

	template<typename _tColor>
	static				::llc::error_t												textLineDrawAlignedFixedSizeRGBA			(::llc::grid_view<_tColor>& targetView, const ::llc::grid_view<_tColor>& fontAtlas, uint32_t lineOffset, const ::llc::SCoord2<uint32_t>& targetSize, const ::llc::SCoord2<int32_t>& sizeCharCell, const ::llc::view_const_string& text0 )	{	// --- This function will draw some coloured symbols in each cell of the ASCII screen.
		const ::llc::SCoord2<int32_t>														dstTextOffset								= {(int32_t)targetSize.x / 2 - (int32_t)::llc::textLineCalcSize(sizeCharCell, text0) / 2, };
		uint32_t																			dstOffsetY									= (int32_t)(targetSize.y - lineOffset * sizeCharCell.y - sizeCharCell.y);
		return ::llc::textLineDrawFixedSize(targetView, fontAtlas, 32, dstOffsetY, sizeCharCell, text0, dstTextOffset);
	}

	template<typename _tColor>
	static				::llc::error_t												textLineDrawAlignedFixedSizeLit					(::llc::grid_view<_tColor>& targetView, const ::llc::bit_view<uint32_t>& fontAtlas, const ::llc::SCoord2<uint32_t> & viewMetrics, uint32_t lineOffset, const ::llc::SCoord2<uint32_t>& targetSize, const ::llc::SCoord2<int32_t>& sizeCharCell, const ::llc::view_const_string& text0, const _tColor& color)	{	// --- This function will draw some coloured symbols in each cell of the ASCII screen.
		const ::llc::SCoord2<int32_t>														dstTextOffset								= {(int32_t)targetSize.x / 2 - (int32_t)::llc::textLineCalcSize(sizeCharCell, text0) / 2, };
		uint32_t																			dstOffsetY									= (int32_t)(targetSize.y - lineOffset * sizeCharCell.y - sizeCharCell.y);
		return ::llc::textLineDrawFixedSizeLit(targetView, fontAtlas, viewMetrics, 32, dstOffsetY, sizeCharCell, text0, dstTextOffset, color);
	}
} // namespace

#endif // LLC_GUI_TEXT_H_928734982734
