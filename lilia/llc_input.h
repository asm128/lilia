#include "llc_typeint.h"
#include "llc_coord.h"

#ifndef LLC_INPUT_H_2890374982734
#define LLC_INPUT_H_2890374982734

namespace llc
{
	struct SInputMouse {
							::llc::SCoord2<int32_t>					Position									= {};
							::llc::SCoord2<int32_t>					Deltas										= {};
							uint8_t									ButtonState			[32]					= {};
							double									ButtonTime			[32]					= {};
							int64_t									ButtonStrength		[32]					= {};
	};

	struct SInputKeyboard {
							uint8_t									KeyState			[256]					= {};
							double									KeyTime				[256]					= {};
							int64_t									KeyStrength			[256]					= {};
	};

	struct SInput {
							SInputKeyboard							KeyboardPrevious							= {};
							SInputKeyboard							KeyboardCurrent								= {};
							SInputMouse								MousePrevious								= {};
							SInputMouse								MouseCurrent								= {};

		inline constexpr	bool									ButtonUp									(uint8_t buttonIndex)		const	noexcept	{ return MouseCurrent.ButtonState[buttonIndex	] == 0 && MousePrevious.ButtonState[buttonIndex	] != 0; }
		inline constexpr	bool									ButtonDown									(uint8_t buttonIndex)		const	noexcept	{ return MouseCurrent.ButtonState[buttonIndex	] != 0 && MousePrevious.ButtonState[buttonIndex	] == 0; }
		inline constexpr	bool									KeyUp										(uint8_t keyIndex	)		const	noexcept	{ return KeyboardCurrent.KeyState[keyIndex		] == 0 && KeyboardPrevious.KeyState[keyIndex	] != 0; }
		inline constexpr	bool									KeyDown										(uint8_t keyIndex	)		const	noexcept	{ return KeyboardCurrent.KeyState[keyIndex		] != 0 && KeyboardPrevious.KeyState[keyIndex	] == 0; }
	};
} // namespace

#endif // LLC_INPUT_H_2890374982734
