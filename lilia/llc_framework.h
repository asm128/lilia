#include "llc_runtime.h"
#include "llc_display.h"
#include "llc_input.h"
#include "llc_timer.h"
#include "llc_frameinfo.h"
#include "llc_texture.h"

#ifndef LLC_FRAMEWORK_H_20987347928
#define LLC_FRAMEWORK_H_20987347928

namespace llc
{
	struct SFramework {
		typedef				::llc::STexture<::llc::SColorBGRA>					TOffscreen;

							::llc::SRuntimeValues								RuntimeValues								= {};
							::llc::SDisplay										MainDisplay									= {};
							::llc::SInput										Input										= {};
							::llc::STimer										Timer										= {};
							::llc::SFrameInfo									FrameInfo									= {};
							TOffscreen											Offscreen									= {};

																				SFramework									(::llc::SRuntimeValues& runtimeValues)			noexcept	: RuntimeValues(runtimeValues) {}
	}; // struct

						::llc::error_t										updateFramework								(::llc::SFramework& framework);
} // namespace

#endif // LLC_FRAMEWORK_H_20987347928
