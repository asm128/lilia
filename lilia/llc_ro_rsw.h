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
#pragma pack(pop)
	//struct SRSWWorldObject {
	//				SRSWWorldObjectInfo										Info;
	//				::std::string											Name;
	//};

	struct SModelInfoRSW {
					::std::string											Name		;
					int32_t													AnimType	;	
					float													AnimSpeed	;
					int32_t													BlockType	;
					::std::string											Filename;
					::std::string											Str2;
					::std::string											RootRSMNode;
					::std::string											Str4;
					::llc::SCoord3<float>									Position;
					::llc::SCoord3<float>									Rotation;
					::llc::SCoord3<float>									Scale;
	};

	struct SRSWFileContents {
					::std::string											INIFilename;
					::std::string											GNDFilename;
					::std::string											GATFilename;
					::std::string											SOMFilename;
					::llc::array_obj<SModelInfoRSW>							RSWModels;
	};

				::llc::error_t											rswFileLoad								(::llc::SRSWFileContents& loaded, const ::llc::array_view<ubyte_t>	& input);
				::llc::error_t											rswFileLoad								(::llc::SRSWFileContents& loaded, FILE								* input);
				::llc::error_t											rswFileLoad								(::llc::SRSWFileContents& loaded, const ::llc::view_const_string	& input);
}

#endif // LLC_RO_RSW_H_9283742893742893
