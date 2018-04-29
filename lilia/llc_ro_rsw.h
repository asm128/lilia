#include "llc_array.h"
#include "llc_coord.h"

#include <string>

#ifndef LLC_RO_RSW_H_9283742893742893
#define LLC_RO_RSW_H_9283742893742893

namespace llc
{
#pragma pack(push, 1)
	struct SRSWWorldObject {
					char													part0											[40];
					char													part1											[40];
					char													part2											[40];
					::llc::SCoord3<float>									Position										;
					::llc::SCoord3<float>									Rotation										;
					::llc::SCoord3<float>									Scale											;
					char													u0;
					char													uf0												[4];
					char													somePath										[16];
					char													unkstr											[12];
					::llc::SCoord3<int8_t>									unkchars										;
					uint32_t												minusone										;
					char													somevals										[8];
					float													part3											;
					uint8_t													part4											[4];
	};

	struct SLightInfoRSW {
					char													Name		[40];
					char													ToDo		[40];
					::llc::SCoord3<float>									Position 	;	
					::llc::SCoord3<float>									Color		;	
					float													ToDo2		;	
	};

	struct SEffectInfoRSW {
					char													name		[40];
					char													nameUnk		[40];
					::llc::SCoord3<float>									position	;
					int														id			;
					float													loop		;
					float													param1		;
					float													param2		;
					float													param3		;
					float													param4		;
	};

	struct SSoundInfoRSW {
					char													name		[40];	
					char													strUnk0		[40];	
					char													fileName	[40];	
					char													strUnk1		[40];	
					::llc::SCoord3<float>									position	;		
					float													vol			;		
					int32_t													width		;		
					int32_t													height		;		
					float													range		;		
					float													cycle		;			// v >= 2.0
	};
	
	struct SModelInfoRSW {
					::std::string											Name		;
					::std::string											Filename;
					::std::string											Str2;
					::std::string											RootRSMNode;
					::std::string											Str4;
					int32_t													AnimType	;	
					float													AnimSpeed	;
					int32_t													BlockType	;
					::llc::SCoord3<float>									Position;
					::llc::SCoord3<float>									Rotation;
					::llc::SCoord3<float>									Scale;
	};

	struct SRSWWorldWater {
					float													Height		;
					uint32_t												Type		;
					float													Amplitude	;
					float													Speed		;
					float													Pitch		;
					uint32_t												TexCycling	;
	};

	struct SRSWWorldLight {
					int32_t													Longitude		;
					int32_t													Latitude		;
					::llc::SCoord3<float>									Diffuse			;
					::llc::SCoord3<float>									Ambient			;
					float													Intensity		;
	};
#pragma pack(pop)

	struct SRSWFileContents {
					::std::string											INIFilename	;
					::std::string											GNDFilename	;
					::std::string											GATFilename	;
					::std::string											SOMFilename	;

					::llc::SRSWWorldWater									Water			;
					::llc::SRSWWorldLight									Light			;

					::llc::array_obj<SModelInfoRSW	>						RSWModels	;
					::llc::array_obj<SLightInfoRSW	>						RSWLights	;
					::llc::array_obj<SEffectInfoRSW	>						RSWEffects	;
					::llc::array_obj<SSoundInfoRSW	>						RSWSounds	;
	};

				::llc::error_t											rswFileLoad								(::llc::SRSWFileContents& loaded, const ::llc::array_view<ubyte_t>	& input);
				::llc::error_t											rswFileLoad								(::llc::SRSWFileContents& loaded, FILE								* input);
				::llc::error_t											rswFileLoad								(::llc::SRSWFileContents& loaded, const ::llc::view_const_string	& input);
}

#endif // LLC_RO_RSW_H_9283742893742893
