#include "llc_array.h"
#include "llc_color.h"
#include "llc_input.h"
#include <string>

#ifndef LLC_GUI_H_2903874982374
#define LLC_GUI_H_2903874982374

namespace llc
{
#pragma pack(push, 1)
	enum CONTROL_STATE	: int8_t
		{ CONTROL_STATE_Idle
		, CONTROL_STATE_Hover		
		, CONTROL_STATE_Pressed		
		, CONTROL_STATE_Selected	
		, CONTROL_STATE_Execute
		, CONTROL_STATE_Disabled	
		, CONTROL_STATE_Unused		
		, CONTROL_STATE_COUNT		
		, CONTROL_STATE_INVALID			= (int8_t)-1
		};

	struct SControlState {
							bool												Hover									: 1;
							bool												Pressed									: 1;
							bool												Selected								: 1;
							bool												Execute									: 1;
							bool												Disabled								: 1;
							bool												Unused									: 1;
	};

	struct SControlPropertyIndices {
							int16_t												Text									= -1;
							int16_t												Area									= -1;
							int16_t												AreaRealigned							= -1;
							int16_t												Center									= -1;
							int16_t												ColorFill								= -1;
							int16_t												ColorBorder								= -1;
							int16_t												SizeBorder								= -1;
							int16_t												SizeMargin								= -1;
							int16_t												SizePadding								= -1;
	};

	struct SGUIColorSurface {
							::llc::SColorBGRA									Fill									= {};
							::llc::SColorBGRA									Border									= {};
	};

	struct SGUIColorText {
							::llc::SColorBGRA									Fill									= {};
							::llc::SColorBGRA									Border									= {};
							::llc::SColorBGRA									Background								= {};
	};
#pragma pack(pop)

	struct SControlProperties {
							::std::string										Text									= {};
							::std::string										Tooltip									= {};
							::llc::SRectangle2D<int32_t>						Area									= {};
							::llc::SRectangle2D<int32_t>						AreaControl								= {};
							::llc::SRectangle2D<int32_t>						AreaText								= {};
							::llc::SRectangle2D<int32_t>						AreaRealigned							= {};
							::llc::SCoord2<int32_t>								Center									= {};
							::llc::SGUIColorSurface								ColorSurface							= {};
							::llc::SGUIColorText								ColorText								= {};
							::llc::SCoord2<uint32_t>							SizeBorder								= {};
							::llc::SCoord2<uint32_t>							SizeMargin								= {};
							::llc::ALIGN										AlignArea								= {};
							::llc::ALIGN										AlignText								= {};
	};

	struct SControlPropertyTable {
							::llc::array_pod<::std::string					>	Text									= {};
							::llc::array_pod<::std::string					>	Tooltip									= {};
							::llc::array_pod<::llc::SRectangle2D<int32_t>	>	Area									= {};
							::llc::array_pod<::llc::SRectangle2D<int32_t>	>	AreaRealigned							= {};
							::llc::array_pod<::llc::SCoord2<int32_t>		>	Center									= {};
							::llc::array_pod<::llc::SGUIColorSurface		>	ColorSurface							= {};
							::llc::array_pod<::llc::SGUIColorText			>	ColorText								= {};
							::llc::array_pod<::llc::SCoord2<uint32_t>		>	SizeBorder								= {};
							::llc::array_pod<::llc::SCoord2<uint32_t>		>	SizeMargin								= {};
							::llc::array_pod<::llc::ALIGN					>	AlignArea								= {};
							::llc::array_pod<::llc::ALIGN					>	AlignText								= {};

							// -------------
							void												Clear									()						{
								::llc::clear
									(	Text				
									,	Area				
									,	AreaRealigned		
									,	ColorSurface		
									,	ColorText			
									,	SizeBorder			
									,	SizeMargin			
									,	AlignArea			
									,	AlignText			
									);
							}

							void												Resize									(uint32_t newSize)		{
								::llc::resize
									(	newSize
									,	Text				
									,	Area				
									,	AreaRealigned		
									,	ColorSurface		
									,	ColorText			
									,	SizeBorder			
									,	SizeMargin			
									,	AlignArea			
									,	AlignText			
									);
							}

	};

	struct SGUI {	// 
							::llc::SControlPropertyTable						Properties		[CONTROL_STATE_COUNT]	= {};
							::llc::array_pod<::llc::SControlState>				Controls								= {};
							::llc::SCoord2<uint32_t>							TargetSize								= {};
							::llc::SCoord2<int32_t>								MousePosition							= {};
	};

						::llc::error_t										guiControlCreate						(::llc::SGUI& guiSystem, const ::llc::SControlProperties& controlProperties);
						::llc::error_t										guiUpdate								(::llc::SGUI& GUISystem, const ::llc::SInput& inputSystem);
} // namespace

#endif // LLC_GUI_H_2903874982374

