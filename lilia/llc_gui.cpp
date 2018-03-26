#include "llc_gui.h"

::llc::error_t									llc::guiControlCreate			(::llc::SGUI& guiSystem, const ::llc::SControlProperties& definition)						{
	::llc::SRectangle2D<int32_t>						areaRealigned				= {};
	::llc::realignRectangle(guiSystem.TargetSize, definition.Area, areaRealigned, definition.AlignArea);

	::llc::SControlPropertyTable						& controlTable				= guiSystem.Properties[::llc::CONTROL_STATE_Idle];
	int32_t												oldSize						= controlTable.AreaRealigned.size();
	int32_t												resultCheck	[]				= 
		//{	controlTable.AreaRealigned			.push_back(areaRealigned			)
		{	controlTable.Text					.push_back(definition.Text			)
		,	controlTable.Area					.push_back(definition.Area			)
		,	controlTable.AreaRealigned			.push_back(areaRealigned			) //definition.AreaRealigned	)
		,	controlTable.ColorSurface			.push_back(definition.ColorSurface	)
		,	controlTable.ColorText				.push_back(definition.ColorText		)
		,	controlTable.SizeBorder				.push_back(definition.SizeBorder	)
		,	controlTable.SizeMargin				.push_back(definition.SizeMargin	)
		,	controlTable.AlignArea				.push_back(definition.AlignArea		)
		,	controlTable.AlignText				.push_back(definition.AlignText		)
		};

	for(uint32_t iState = 1; iState < CONTROL_STATE_COUNT; ++iState) {
		//guiSystem.Properties[iState].AreaRealigned	.push_back(definition.AreaRealigned	);
		guiSystem.Properties[iState].Text			.push_back(definition.Text			);
		guiSystem.Properties[iState].Area			.push_back(definition.Area			);
		guiSystem.Properties[iState].AreaRealigned	.push_back(areaRealigned			); //definition.AreaRealigned	);
		guiSystem.Properties[iState].ColorSurface	.push_back(definition.ColorSurface	);
		guiSystem.Properties[iState].ColorText		.push_back(definition.ColorText		);
		guiSystem.Properties[iState].SizeBorder		.push_back(definition.SizeBorder	);
		guiSystem.Properties[iState].SizeMargin		.push_back(definition.SizeMargin	);
		guiSystem.Properties[iState].AlignArea		.push_back(definition.AlignArea		);
		guiSystem.Properties[iState].AlignText		.push_back(definition.AlignText		);
	}

	for(uint32_t iCheck = 0, checkCount = ::llc::size(resultCheck) - 1; iCheck < checkCount; ++iCheck )
		if(resultCheck[iCheck] != resultCheck[iCheck + 1]) {
			error_printf("Control property indices don't match. The reasons for this to happen are running out of memory or mismanagement of the control table.");
			controlTable.Resize(oldSize);
			return -1;
		}
	return 0;
}


static						::llc::error_t		updateGUIControlHovered			(::llc::SControlState& controlFlags, const ::llc::SInput& inputSystem)	noexcept{ 
	if(controlFlags.Hover) {
		if(inputSystem.ButtonDown(0) && false == controlFlags.Pressed) 
			controlFlags.Pressed							= true;
		else if(inputSystem.ButtonUp(0) && controlFlags.Pressed) {
			controlFlags.Execute							= true;
			controlFlags.Pressed							= false;
		}
	}
	else 
		controlFlags.Hover								= true;
	return 0;
}

::llc::error_t									llc::guiUpdate					(::llc::SGUI& GUISystem, const ::llc::SInput& inputSystem)										{ 
	const ::llc::SCoord2<int32_t>						& mousePos						= GUISystem.MousePosition = inputSystem.MouseCurrent.Position;
	::llc::SControlPropertyTable						& controlsIdle					= GUISystem.Properties[::llc::CONTROL_STATE_Idle];
	::llc::array_pod<::llc::SControlState>				& controlsState					= GUISystem.Controls;
	for(uint32_t iControl=0; iControl < controlsState.size(); ++iControl) {
		::llc::SRectangle2D<int32_t>						& controlArea					= controlsIdle.AreaRealigned	[iControl];
		::llc::SControlState								& controlFlags					= controlsState[iControl];
		
		// EXECUTE only lasts one tick.
		if (controlFlags.Execute)
			controlFlags.Execute							= false;

		//--------------------
		if(::llc::in_range(mousePos, controlArea))
			updateGUIControlHovered(controlFlags, inputSystem);
		else {
			if (controlFlags.Hover)
				controlFlags.Hover									= false;

			if(inputSystem.ButtonUp(0) && controlFlags.Pressed)
				controlFlags.Pressed								= false;
		}
	}
	return 0;
}
