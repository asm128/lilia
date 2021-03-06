#include "llc_array.h"
#include "llc_coord.h"

#ifndef LLC_RO_RSW_H_9283742893742893
#define LLC_RO_RSW_H_9283742893742893

namespace llc
{
#pragma pack(push, 1)
	struct SRSWWorldObject {
					char													part0			[40];
					char													part1			[40];
					char													part2			[40];
					::llc::SCoord3<float>									Position		;
					::llc::SCoord3<float>									Rotation		;
					::llc::SCoord3<float>									Scale			;
					char													u0				;
					char													uf0				[4];
					char													somePath		[16];
					char													unkstr			[12];
					::llc::SCoord3<int8_t>									unkchars		;
					uint32_t												minusone		;
					char													somevals		[8];
					float													part3			;
					uint8_t													part4			[4];
	};

	struct SLightInfoRSW {
					char													Name			[40];
					char													ToDo			[40];
					::llc::SCoord3<float>									Position 		;	
					::llc::SCoord3<float>									Color			;	
					float													ToDo2			;	
	};

	struct SEffectInfoRSW {
					char													name			[40];
					char													nameUnk			[40];
					::llc::SCoord3<float>									position		;
					int														id				;
					float													loop			;
					float													param1			;
					float													param2			;
					float													param3			;
					float													param4			;
	};

	struct SSoundInfoRSW {
					char													name			[40];	
					char													strUnk0			[40];	
					char													fileName		[40];	
					char													strUnk1			[40];	
					::llc::SCoord3<float>									position		;		
					float													vol				;		
					int32_t													width			;		
					int32_t													height			;		
					float													range			;		
					float													cycle			;			// v >= 2.0
	};
	
	struct SModelInfoRSW {
					char_t													Name			[40];
					char_t													Filename		[40];
					char_t													Str2			[40];
					char_t													RootRSMNode		[40];
					char_t													Str4			[40];
					int32_t													AnimType		;	
					float													AnimSpeed		;
					int32_t													BlockType		;
					::llc::SCoord3<float>									Position		;
					::llc::SCoord3<float>									Rotation		;
					::llc::SCoord3<float>									Scale			;
	};

	struct SRSWWorldWater {
					float													Height			;
					uint32_t												Type			;
					float													Amplitude		;
					float													Speed			;
					float													Pitch			;
					uint32_t												TexCycling		;
	};

	struct SRSWWorldLight {
					int32_t													Longitude		;
					int32_t													Latitude		;
					::llc::SCoord3<float>									Diffuse			;
					::llc::SCoord3<float>									Ambient			;
					float													Intensity		;
	};

	struct SRSWFileContents {
					::llc::array_pod<SModelInfoRSW	>						RSWModels		;
					::llc::array_pod<SLightInfoRSW	>						RSWLights		;
					::llc::array_pod<SEffectInfoRSW	>						RSWEffects		;
					::llc::array_pod<SSoundInfoRSW	>						RSWSounds		;

					char_t													INIFilename		[40];
					char_t													GNDFilename		[40];
					char_t													GATFilename		[40];
					char_t													SOMFilename		[40];

					::llc::SRSWWorldWater									Water			;
					::llc::SRSWWorldLight									Light			;

	};
#pragma pack(pop)

				::llc::error_t											rswFileLoad								(::llc::SRSWFileContents& loaded, const ::llc::array_view<ubyte_t>	& input);
				::llc::error_t											rswFileLoad								(::llc::SRSWFileContents& loaded, FILE								* input);
				::llc::error_t											rswFileLoad								(::llc::SRSWFileContents& loaded, const ::llc::view_const_string	& input);
}

#endif // LLC_RO_RSW_H_9283742893742893
