#include "llc_coord.h"
#include "llc_color.h"
#include "llc_grid_view.h"

#if defined(LLC_WINDOWS)
#	include <windows.h>
#endif 

#ifndef LLC_DISPLAY_H_293874928374
#define LLC_DISPLAY_H_293874928374

namespace llc 
{
	struct SDisplayPlatformDetail {
#if defined(LLC_WINDOWS)
							::HWND													WindowHandle								= {};
							::WNDCLASSEX											WindowClass									= {};

		static constexpr	const char												WindowClassName	[256]						= "SUGAR_WINDOW";
#endif
	};

#if defined(LLC_WINDOWS)
	static inline		void													initWndClass								(::HINSTANCE hInstance, const TCHAR* className, ::WNDCLASSEX& wndClassToInit, WNDPROC wndProc)	{
		wndClassToInit																= {sizeof(::WNDCLASSEX),};
		wndClassToInit.lpfnWndProc													= wndProc;
		wndClassToInit.hInstance													= hInstance;
		wndClassToInit.hCursor														= LoadCursor(NULL, IDC_ARROW);
		wndClassToInit.hbrBackground												= (::HBRUSH)(COLOR_3DFACE + 1);
		wndClassToInit.lpszClassName												= className;
	}
#endif

	struct SDisplay {
							::llc::SDisplayPlatformDetail							PlatformDetail								= {};
							::llc::SCoord2<uint32_t>								PreviousSize								= {};
							::llc::SCoord2<uint32_t>								Size										= {};
							bool													Resized										: 1;
							bool													Repaint										: 1;
							bool													NoDraw										: 1;
							bool													MinOrMaxed									: 1;
							bool													Closed										: 1;

																					SDisplay									()
			: Resized		(false)
			, Repaint		(false)
			, NoDraw		(false)
			, MinOrMaxed	(false)
			, Closed		(false)
		{}
	};

		::llc::error_t																displayPresentTarget						(::llc::SDisplay& displayInstance, const ::llc::grid_view<::llc::SColorBGRA>& targetToPresent);
		::llc::error_t																displayUpdate								(::llc::SDisplay& applicationInstance);
} // namespace

#endif // LLC_DISPLAY_H_293874928374
