#include "llc_array.h"
#include "llc_coord.h"
#include "llc_array_static.h"

#include <string>

#ifndef LLC_RSM_H_02938402983
#define LLC_RSM_H_02938402983

namespace llc
{
#pragma pack(push, 1)
	struct SRSMFace {
					int16_t													Vertices[3]			;	// Vertex indices?
					int16_t													UVs		[3]			;	// UVs indices? 
					int16_t													IndexTextureIndex	;	// texture index index
					int16_t													todo1				;	//
					int32_t													todo2				;	//
					int32_t													SmoothGroup			;	//
	};


	struct SRSMNodeTransform {
					::llc::SCoord3<float>									Row0;
					::llc::SCoord3<float>									Row1;
					::llc::SCoord3<float>									Row2;
					::llc::SCoord3<float>									Offset;
					::llc::SCoord3<float>									Translation;
					float													Rotation;
					::llc::SCoord3<float>									RotAxis;
					::llc::SCoord3<float>									Scale;
	}; //

	struct SRSMFrameRotation {
					int														time;
					::llc::SQuaternion<float>								orientation;
	};

	struct SRSMFramePosition {
					int														time;
					::llc::SCoord2<float>									Position;
	};

	struct SRSMTexCoord {
					uint32_t												Unknown;
					::llc::SCoord2<float>									UV;
	};
#pragma pack(pop)
	struct SRSMNode {
					::llc::array_pod<int32_t>								TextureIndices;
					::llc::array_pod<::llc::SCoord3<float>>					Vertices;
					::llc::array_pod<::llc::SRSMTexCoord>					UVs;
					::llc::array_pod<::llc::SRSMFace>						Faces;
					char_t													Name		[40];
					char_t													ParentName	[40];
					::llc::SRSMNodeTransform								Transform;
					::llc::array_pod<SRSMFrameRotation>						RotationKeyframes;
					::llc::array_pod<SRSMFramePosition>						PositionKeyframes;
	};

	struct SRSMFileContents {
					uint32_t												AnimLength;
					uint32_t												ShadeType;
					uint8_t													Alpha;
					char_t													Unknown[16];

					::llc::array_obj<::llc::array_static<char_t, 40>>		TextureNames;

					char_t													RootNodeName[40];
					::llc::array_obj<::llc::SRSMNode>						Nodes;
	};
				::llc::error_t											rsmFileLoad								(::llc::SRSMFileContents& loaded, const ::llc::array_view<ubyte_t>	& input);
				::llc::error_t											rsmFileLoad								(::llc::SRSMFileContents& loaded, FILE								* input);
				::llc::error_t											rsmFileLoad								(::llc::SRSMFileContents& loaded, const ::llc::view_const_string	& input);

	struct SModelNodeRSM {
					::llc::array_pod<::llc::SCoord3<float>>					Normals			;
					::llc::array_pod<::llc::SCoord3<float>>					Vertices		;
					::llc::array_pod<::llc::SCoord2<float>>					UVs				;
					::llc::array_pod<::llc::STriangleWeights<uint32_t>>		VertexIndices	; 
					int32_t													TextureIndex	;
					int32_t													RSMNodeIndex	;
					//::llc::array_pod<int32_t>								SkinIndices		; // one per triangle (VertexIndices.size() / 3)
	};
} // namespace

#endif // LLC_RSM_H_02938402983
