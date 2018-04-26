#include "llc_array.h"
#include "llc_coord.h"

#include <string>

#ifndef LLC_RO_RSW_H_9283742893742893
#define LLC_RO_RSW_H_9283742893742893

namespace llc
{
#pragma pack(push, 1)
	struct SRSWWorldObject {
					char													part0											[30 * 4		];
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
#pragma pack(pop)
	//struct SRSWWorldObject {
	//				SRSWWorldObjectInfo										Info;
	//				::std::string											Name;
	//};

	struct SRSWFileContents {
					::std::string											INIFilename;
					::std::string											GNDFilename;
					::std::string											GATFilename;
					::llc::array_obj<::std::string>							RSWFilenames;
					::llc::array_pod<SRSWWorldObject>						RSWObjects;
	};

				::llc::error_t											rswFileLoad								(::llc::SRSWFileContents& loaded, const ::llc::array_view<ubyte_t>	& input);
				::llc::error_t											rswFileLoad								(::llc::SRSWFileContents& loaded, FILE								* input);
				::llc::error_t											rswFileLoad								(::llc::SRSWFileContents& loaded, const ::llc::view_const_string	& input);
}

#endif // LLC_RO_RSW_H_9283742893742893
