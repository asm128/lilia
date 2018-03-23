// Tip: Best viewed with zoom level at 81%.
// Tip: Hold Left ALT + SHIFT while tapping or holding the arrow keys in order to select multiple columns and write on them at once. 
//		Also useful for copy & paste operations in which you need to copy a bunch of variable or function names and you can't afford the time of copying them one by one.
#include "llc_framework.h"

#if defined(LLC_WINDOWS)
#	include <Windows.h>
#endif

					::llc::error_t										llc::updateFramework						(::llc::SFramework& framework)													{
	framework.Input.KeyboardPrevious											= framework.Input.KeyboardCurrent;
	framework.Input.MousePrevious												= framework.Input.MouseCurrent;
	framework.Input.MouseCurrent.Deltas											= {};
	::llc::SFrameInfo															& frameInfo									= framework.FrameInfo;
	::llc::STimer																& timer										= framework.Timer;
	timer		.Frame();
	frameInfo	.Frame(::llc::min(timer.LastTimeMicroseconds, 200000ULL));
	::llc::SDisplay																& mainWindow								= framework.MainDisplay;
	ree_if(errored(::llc::displayUpdate(mainWindow)), "Not sure why this would fail.");
	rvi_if(1, 0 == mainWindow.PlatformDetail.WindowHandle, "Application exiting because the main window was closed.");
	::llc::SFramework::TOffscreen												& offscreen									= framework.Offscreen;
	error_if(errored(::llc::displayPresentTarget(mainWindow, offscreen.View)), "Unknown error.");
#if defined(LLC_WINDOWS)
	Sleep(1);
#endif
	return 0;
}

