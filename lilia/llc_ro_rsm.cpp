#include "llc_ro_rsm.h"
#include "llc_matrix.h"
#include "llc_stream_view.h"

#include <string>

#pragma pack(push, 1)
struct SRSMHeader {	// RSM Header
	char										filecode[4];	
	uint8_t										versionMajor;
	uint8_t										versionMinor;
};
#pragma pack(pop)

			
			::llc::error_t								analyzeArray												(const ::llc::array_view<ubyte_t>& input) {
	info_printf("---- Analyzing bytes     :");			for(uint32_t iChar = 0; iChar < input.size() / 1; ++iChar)			info_printf("'%c' : %.4u : %.4i : 0x%x"	, input[iChar] ? input[iChar] : ' ', (uint32_t)input[iChar], (int32_t)((int8_t*)input.begin())[iChar], input[iChar]);
										  
	info_printf("---- Analyzing shorts    :");			for(uint32_t iChar = 0; iChar < input.size() / 2; ++iChar)			info_printf(  "%.6u : %.6i : 0x%.4x"	, ((uint16_t	*)&input[0])[iChar], (int32_t)((int16_t	*)input.begin())[iChar], ((uint16_t	*)input.begin())[iChar]);
	info_printf("---- Analyzing ints      :");			for(uint32_t iChar = 0; iChar < input.size() / 4; ++iChar)			info_printf("%.12u : %.12i : 0x%.8x"	, ((uint32_t	*)&input[0])[iChar], ((int32_t			*)input.begin())[iChar], ((uint32_t	*)input.begin())[iChar]);
	info_printf("---- Analyzing floats    :");			for(uint32_t iChar = 0; iChar < input.size() / 4; ++iChar)			info_printf("%.12f"						, ((float		*)&input[0])[iChar]);

	info_printf("---- Analyzing shifted shorts (1):");	for(uint32_t iChar = 0; iChar < (input.size() - 1) / 2; ++iChar)	info_printf(  "%.6u : %.6i : 0x%.4x"	, ((uint16_t	*)&input[1])[iChar], (int32_t)((int16_t	*)&input[1])[iChar], ((uint16_t	*)&input[1])[iChar]);
	info_printf("---- Analyzing shifted ints   (1):");	for(uint32_t iChar = 0; iChar < (input.size() - 1) / 4; ++iChar)	info_printf("%.12u : %.12i : 0x%.8x"	, ((uint32_t	*)&input[1])[iChar], ((int32_t			*)&input[1])[iChar], ((uint32_t	*)&input[1])[iChar]);
	info_printf("---- Analyzing shifted floats (1):");	for(uint32_t iChar = 0; iChar < (input.size() - 1) / 4; ++iChar)	info_printf("%.12f : %.12f"				, ((float		*)&input[1])[iChar], ((float			*)&input[1])[iChar]);

	info_printf("---- Analyzing shifted ints   (2):");	for(uint32_t iChar = 0; iChar < (input.size() - 2) / 4; ++iChar)	info_printf("%.12u : %.12i : 0x%.8x"	, ((uint32_t	*)&input[2])[iChar], ((int32_t			*)&input[2])[iChar], ((uint32_t	*)&input[2])[iChar]);
	info_printf("---- Analyzing shifted floats (2):");	for(uint32_t iChar = 0; iChar < (input.size() - 2) / 4; ++iChar)	info_printf("%.12f : %.12f"				, ((float		*)&input[2])[iChar], ((float			*)&input[2])[iChar]);

	info_printf("---- Analyzing shifted ints   (3):");	for(uint32_t iChar = 0; iChar < (input.size() - 3) / 4; ++iChar)	info_printf("%.12u : %.12i : 0x%.8x"	, ((uint32_t	*)&input[3])[iChar], ((int32_t			*)&input[3])[iChar], ((uint32_t	*)&input[3])[iChar]);
	info_printf("---- Analyzing shifted floats (3):");	for(uint32_t iChar = 0; iChar < (input.size() - 3) / 4; ++iChar)	info_printf("%.12f : %.12f"				, ((float		*)&input[3])[iChar], ((float			*)&input[3])[iChar]);
	return 0;
}

			::llc::error_t								llc::rsmFileLoad											(::llc::SRSMFileContents& loaded, const ::llc::array_view<ubyte_t>	& input)							{
	::llc::stream_view<const ubyte_t>							gnd_stream													= {input.begin(), input.size()};
	SRSMHeader													header														= {};
	gnd_stream.read_pod	(header);
	gnd_stream.read_pod	(loaded.AnimLength);
	gnd_stream.read_pod	(loaded.ShadeType);
	if(header.versionMajor > 1 || (header.versionMajor == 1 && header.versionMinor >= 4))
		gnd_stream.read_pod	(loaded.Alpha);
	
	gnd_stream.read_pod(loaded.Unknown);

	uint32_t													textureCount												= 0;			// Get the number of textures
	gnd_stream.read_pod(textureCount);

	info_printf("RSM magic number   : %.4s."	, header.filecode);
	info_printf("RSM version        : %u.%u."	, header.versionMajor, header.versionMinor);
	info_printf("RSM animation time : %u."		, (uint32_t)loaded.AnimLength);
	info_printf("RSM shade type     : %u."		, (uint32_t)loaded.ShadeType);
	info_printf("RSM alpha          : %u."		, (uint32_t)loaded.Alpha);
\
	loaded.TextureNames.resize(textureCount);
	for(uint32_t iTex = 0; iTex < textureCount; ++iTex) {
		gnd_stream.read_pod(loaded.TextureNames[iTex].Storage);
		info_printf("Texture %i name: %s.", (int32_t)iTex, loaded.TextureNames[iTex].Storage);
	}

	gnd_stream.read_pod(loaded.RootNodeName); 

	int32_t														meshCountIBelieve											= 1;
	gnd_stream.read_pod(meshCountIBelieve);

	uint32_t													totalVertices												= 0;
	uint32_t													totalUVs													= 0;
	uint32_t													totalFaces													= 0;
	uint32_t													byteOffsetStartModel										= gnd_stream.CursorPosition;

	loaded.Nodes.resize(meshCountIBelieve);
	for(int32_t iMesh = 0; iMesh < meshCountIBelieve; ++iMesh) {
		::llc::SRSMNode												& newNode													= loaded.Nodes[iMesh];
		gnd_stream.read_pod(newNode.Name		);
		gnd_stream.read_pod(newNode.ParentName	);
		info_printf("---------------------------------------------- Reading mesh node: %u ----------------------------------------------", (uint32_t)iMesh);
		info_printf("Mesh node name: %s."	, newNode.Name			);
		info_printf("Parent node name: %s."	, newNode.ParentName	);
		{
			uint32_t													texMappingCount												= 0;			// Get the number of texture indices for this model
			gnd_stream.read_pod(texMappingCount);
			info_printf("Texture index count: %u.", texMappingCount);
			if(texMappingCount) {
				::llc::array_pod<int32_t>									& modelTextures												= newNode.TextureIndices;	
				modelTextures.resize(texMappingCount);
				gnd_stream.read_pod(modelTextures.begin(), texMappingCount);
			}
		}

		gnd_stream.read_pod(newNode.Transform);
		info_printf("Node transform (Row0	): {%f, %f, %f}."		, newNode.Transform.Row0		.x, newNode.Transform.Row0			.y, newNode.Transform.Row0			.z);
		info_printf("Node transform (Row1	): {%f, %f, %f}."		, newNode.Transform.Row1		.x, newNode.Transform.Row1			.y, newNode.Transform.Row1			.z);
		info_printf("Node transform (Row2	): {%f, %f, %f}."		, newNode.Transform.Row2		.x, newNode.Transform.Row2			.y, newNode.Transform.Row2			.z);
		info_printf("Node offset             : {%f, %f, %f}."		, newNode.Transform.Offset		.x, newNode.Transform.Offset		.y, newNode.Transform.Offset		.z);
		info_printf("Node translation        : {%f, %f, %f}."		, newNode.Transform.Translation	.x, newNode.Transform.Translation	.y, newNode.Transform.Translation	.z);
		info_printf("Node rotation value     : %f."					, newNode.Transform.Rotation);
		info_printf("Node rotation axis      : {%f, %f, %f}."		, newNode.Transform.RotAxis		.x, newNode.Transform.RotAxis		.y, newNode.Transform.RotAxis		.z);
		info_printf("Node scale              : {%f, %f, %f}."		, newNode.Transform.Scale		.x, newNode.Transform.Scale			.y, newNode.Transform.Scale			.z);
		{
			uint32_t													vertexCount													= 0;			// Get the number of vertex
			gnd_stream.read_pod(vertexCount);
			info_printf("Vertex count: %u.", vertexCount);
			if(vertexCount) {
				::llc::array_pod<::llc::SCoord3<float>>						& modelVertices												= newNode.Vertices;	
				modelVertices.resize(vertexCount);
				gnd_stream.read_pod(modelVertices.begin(), vertexCount);
			}
		}
		{
			uint32_t													texVtxCount													= 0;			// Get the number of unk
			gnd_stream.read_pod(texVtxCount);
			info_printf("UV coord count: %u.", texVtxCount);
			if(texVtxCount) {
				::llc::array_pod<::llc::SRSMTexCoord>						& modelUNKs													= newNode.UVs;	
				modelUNKs.resize(texVtxCount);
				if((header.versionMajor == 1 && header.versionMinor >= 2) || header.versionMajor > 1) //{ >= v1.2
					gnd_stream.read_pod(modelUNKs.begin(), texVtxCount);
				else {
					for( uint32_t iVertex = 0; iVertex < texVtxCount; ++iVertex ) {
						::llc::SRSMTexCoord											& curTexCoord												= modelUNKs[iVertex]; 
						curTexCoord.Unknown										= (uint32_t)-1;
						gnd_stream.read_pod(curTexCoord.UV);
					}
				}
			}
		}
		{
			uint32_t													faceCount													= 0;			// Get the number of face
			gnd_stream.read_pod(faceCount);
			info_printf("Face count: %u.", faceCount);
			if(faceCount) {
				::llc::array_pod<SRSMFace>									& modelFaces												= newNode.Faces;		
				modelFaces.resize(faceCount);
				gnd_stream.read_pod(modelFaces.begin(), faceCount);
			}
		}
		if((header.versionMajor == 1 && header.versionMinor >= 5) || (header.versionMajor > 1)) { // >= v1.5 
			uint32_t													positionFrameCount											= 0;
			gnd_stream.read_pod(positionFrameCount);
			_CrtDbgBreak();
			if(positionFrameCount) {
				::llc::array_pod<SRSMFramePosition>							& modelKeyframes											= newNode.PositionKeyframes;	
				gnd_stream.read_pod(modelKeyframes.begin(), positionFrameCount);
			}
		}

		{
			uint32_t													keyframeCount												= 0;			// Get the number of keyframe
			gnd_stream.read_pod(keyframeCount);
			info_printf("Rotation keyframe count: %u.", keyframeCount);
			if(keyframeCount) {
				::llc::array_pod<SRSMFrameRotation>							& modelKeyframes											= newNode.RotationKeyframes;	
				modelKeyframes.resize(keyframeCount);
				gnd_stream.read_pod(modelKeyframes.begin(), keyframeCount);
			}
		}
		totalVertices											+= newNode.Vertices	.size();
		totalUVs												+= newNode.UVs		.size();
		totalFaces												+= newNode.Faces	.size();
	}
	info_printf("Total Vertices        : %u.", totalVertices	);
	info_printf("Total UVs             : %u.", totalUVs			);
	info_printf("Total Faces           : %u.", totalFaces		);
	info_printf("Total Node bytes read : %u.", (gnd_stream.CursorPosition - byteOffsetStartModel));
	return gnd_stream.CursorPosition;
}


			::llc::error_t								llc::rsmFileLoad											(::llc::SRSMFileContents& loaded, FILE								* input)							{ 
	loaded, input;
	return 0; 
}

			::llc::error_t								llc::rsmFileLoad											(::llc::SRSMFileContents& loaded, const ::llc::view_const_string	& input)							{ 
	FILE														* fp														= 0;
	ree_if(0 != fopen_s(&fp, input.begin(), "rb"), "Cannot open .rsm file: %s.", input.begin());
	fseek(fp, 0, SEEK_END);
	int32_t														fileSize													= (int32_t)ftell(fp);
	fseek(fp, 0, SEEK_SET);
	::llc::array_pod<ubyte_t>									fileInMemory												= 0;
	llc_necall(fileInMemory.resize(fileSize), "File too large? : %llu.", (uint64_t)fileSize);
	if(fileSize != (int32_t)fread(fileInMemory.begin(), sizeof(ubyte_t), fileSize, fp)) {
		fclose(fp);
		error_printf("fread() failed! file: '%s'.", input.begin());
		return -1;
	}
	fclose(fp);
	uint64_t unk = *(uint64_t*)&fileInMemory[fileInMemory.size() - 8];
	info_printf("%u", unk);

	info_printf("Parsing RSM file: %s.", input.begin());
	return rsmFileLoad(loaded, {fileInMemory.begin(), fileInMemory.size()});
}
