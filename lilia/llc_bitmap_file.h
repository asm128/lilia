#include "llc_texture.h"
#include "llc_color.h"

#ifndef LLC_BITMAP_FILE_H_236544263544352344
#define LLC_BITMAP_FILE_H_236544263544352344

namespace llc
{
						::llc::error_t							bmpFileLoad							(const byte_t					* source		, ::llc::array_pod	<::llc::SColorBGRA>& out_Colors, ::llc::grid_view<::llc::SColorBGRA>& out_ImageView);
						::llc::error_t							bmpFileLoad							(FILE							* source		, ::llc::array_pod	<::llc::SColorBGRA>& out_Colors, ::llc::grid_view<::llc::SColorBGRA>& out_ImageView);
						::llc::error_t							bmpFileLoad							(const ::llc::view_const_string	& filename		, ::llc::array_pod	<::llc::SColorBGRA>& out_Colors, ::llc::grid_view<::llc::SColorBGRA>& out_ImageView);

						::llc::error_t							bmgFileLoad							(const byte_t					* source		, ::llc::array_pod	<::llc::SColorBGRA>& out_Colors, ::llc::grid_view<::llc::SColorBGRA>& out_ImageView);
						::llc::error_t							bmgFileLoad							(FILE							* source		, ::llc::array_pod	<::llc::SColorBGRA>& out_Colors, ::llc::grid_view<::llc::SColorBGRA>& out_ImageView);
						::llc::error_t							bmgFileLoad							(const ::llc::view_const_string	& filename		, ::llc::array_pod	<::llc::SColorBGRA>& out_Colors, ::llc::grid_view<::llc::SColorBGRA>& out_ImageView);

						::llc::error_t							bmgFileWrite						(byte_t							* destination	, const ::llc::grid_view<::llc::SColorBGRA>& in_ImageView);
						::llc::error_t							bmgFileWrite						(FILE							* destination	, const ::llc::grid_view<::llc::SColorBGRA>& in_ImageView);
						::llc::error_t							bmgFileWrite						(const ::llc::view_const_string	& filename		, const ::llc::grid_view<::llc::SColorBGRA>& in_ImageView);

	static inline		::llc::error_t							bmpFileLoad							(const byte_t					* source		, ::llc::STexture	<::llc::SColorBGRA>& out_Texture)	{ return bmpFileLoad(source		, out_Texture.Texels, out_Texture.View); }
	static inline		::llc::error_t							bmpFileLoad							(FILE							* source		, ::llc::STexture	<::llc::SColorBGRA>& out_Texture)	{ return bmpFileLoad(source		, out_Texture.Texels, out_Texture.View); }
	static inline		::llc::error_t							bmpFileLoad							(const ::llc::view_const_string	& filename		, ::llc::STexture	<::llc::SColorBGRA>& out_Texture)	{ return bmpFileLoad(filename	, out_Texture.Texels, out_Texture.View); }

	static inline		::llc::error_t							bmgFileLoad							(const byte_t					* source		, ::llc::STexture	<::llc::SColorBGRA>& out_Texture)	{ return bmgFileLoad(source		, out_Texture.Texels, out_Texture.View); }
	static inline		::llc::error_t							bmgFileLoad							(FILE							* source		, ::llc::STexture	<::llc::SColorBGRA>& out_Texture)	{ return bmgFileLoad(source		, out_Texture.Texels, out_Texture.View); }
	static inline		::llc::error_t							bmgFileLoad							(const ::llc::view_const_string	& filename		, ::llc::STexture	<::llc::SColorBGRA>& out_Texture)	{ return bmgFileLoad(filename	, out_Texture.Texels, out_Texture.View); }

}

#endif // LLC_BITMAP_FILE_H_236544263544352344
