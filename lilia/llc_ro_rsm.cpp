#include "llc_ro_rsm.h"
#include "llc_matrix.h"

#include <string>

#pragma pack(push, 1)
struct SRSMHeader {	// RSM Header
	char										filecode[4];	
	uint8_t										versionMajor;
	uint8_t										versionMinor;
	uint32_t									AnimLength;
	uint32_t									ShadeType;
	uint8_t										Alpha;
	char										todo[16];
};
#pragma pack(pop)

			
			::llc::error_t								analyzeArray												(const ::llc::array_view<ubyte_t>& input) {
	info_printf("---- Analyzing bytes     :"); for(uint32_t iChar = 0; iChar < input.size() / 1; ++iChar) info_printf(  "%.4u : %.4i : 0x%x", (uint32_t)input[iChar], (int32_t)input[iChar], input[iChar]);
										  
	info_printf("---- Analyzing shorts    :");			for(uint32_t iChar = 0; iChar < input.size() / 2; ++iChar) info_printf(  "%.6u : %.6i : 0x%.4x" , ((uint16_t	*)input.begin())[iChar], (int32_t)((int16_t	*)input.begin())[iChar], ((uint16_t	*)input.begin())[iChar]);
	info_printf("---- Analyzing ints      :");			for(uint32_t iChar = 0; iChar < input.size() / 4; ++iChar) info_printf("%.12u : %.12i : 0x%.8x" , ((uint32_t	*)input.begin())[iChar], ((int32_t	*)input.begin())[iChar], ((uint32_t	*)input.begin())[iChar]);
	//info_printf("---- Analyzing long longs:");			for(uint32_t iChar = 0; iChar < input.size() / 8; ++iChar) info_printf("%.30u : %.30i : 0x%.16x", ((uint64_t	*)input.begin())[iChar], ((int64_t	*)input.begin())[iChar], ((uint64_t	*)input.begin())[iChar]);
	info_printf("---- Analyzing floats    :");			for(uint32_t iChar = 0; iChar < input.size() / 4; ++iChar) info_printf("%.12f : %.12f", ((float		*)input.begin())[iChar], ((float	*)input.begin())[iChar]);
	info_printf("---- Analyzing doubles   :");			for(uint32_t iChar = 0; iChar < input.size() / 8; ++iChar) info_printf("%.30f : %.30f", ((double	*)input.begin())[iChar], ((double	*)input.begin())[iChar]);

	info_printf("---- Analyzing shifted shorts    :");	for(uint32_t iChar = 0; iChar < (input.size() - 1) / 2; ++iChar) info_printf(  "%.6u : %.6i : 0x%.4x" , ((uint16_t	*)&input[1])[iChar], (int32_t)((int16_t	*)&input[1])[iChar], ((uint16_t	*)&input[1])[iChar]);
	info_printf("---- Analyzing shifted ints      :");	for(uint32_t iChar = 0; iChar < (input.size() - 1) / 4; ++iChar) info_printf("%.12u : %.12i : 0x%.8x" , ((uint32_t	*)&input[1])[iChar], ((int32_t	*)&input[1])[iChar], ((uint32_t	*)&input[1])[iChar]);
	//info_printf("---- Analyzing shifted long longs:");	for(uint32_t iChar = 0; iChar < (input.size() - 1) / 8; ++iChar) info_printf("%.30u : %.30i : 0x%.16x", ((uint64_t	*)&input[1])[iChar], ((int64_t	*)&input[1])[iChar], ((uint64_t	*)&input[1])[iChar]);
	info_printf("---- Analyzing shifted floats    :");	for(uint32_t iChar = 0; iChar < (input.size() - 1) / 4; ++iChar) info_printf("%.12f : %.12f", ((float		*)&input[1])[iChar], ((float	*)&input[1])[iChar]);
	//info_printf("---- Analyzing shifted doubles   :");	for(uint32_t iChar = 0; iChar < (input.size() - 1) / 8; ++iChar) info_printf("%.30f : %.30f", ((double		*)&input[1])[iChar], ((double	*)&input[1])[iChar]);
	return 0;
}



			::llc::error_t								llc::rsmFileLoad											(::llc::SRSMFileContents& loaded, const ::llc::array_view<ubyte_t>	& input)							{
	uint32_t													byteOffset													= 0;
	SRSMHeader													header														= *(SRSMHeader*)input.begin();
	byteOffset												+= sizeof(SRSMHeader);
	uint32_t													textureCount												= *(uint32_t*)&input[byteOffset];			// Get the number of textures
	byteOffset												+= sizeof(uint32_t);
	info_printf("RSM magic number   : %s."		, header.filecode);
	info_printf("RSM version        : %u.%u."	, header.versionMajor, header.versionMinor);
	info_printf("RSM animation time : %u."		, (uint32_t)header.AnimLength);
	info_printf("RSM shade type     : %u."		, (uint32_t)header.ShadeType);
	info_printf("RSM alpha          : %u."		, (uint32_t)header.Alpha);
	for(uint32_t iUnk = 0; iUnk < 4; ++iUnk) 
		info_printf("RSM unk %u: {%u, %u, %u, %u}.", iUnk
			, (uint32_t)header.todo[iUnk * 4 + 0]
			, (uint32_t)header.todo[iUnk * 4 + 1]
			, (uint32_t)header.todo[iUnk * 4 + 2]
			, (uint32_t)header.todo[iUnk * 4 + 3]
			);

	loaded.TextureNames.resize(textureCount);
	for(uint32_t iTex = 0; iTex < textureCount; ++iTex) {
		const char													* texName													= (const char*)&input[byteOffset];
		loaded.TextureNames[iTex]								= texName;			// Get the number of textures
		info_printf("Texture %i name: %s.", (int32_t)iTex, texName);
		byteOffset												+= 40;
	}
	int32_t														iMesh														= 0;

	uint32_t													totalVertices												= 0;
	uint32_t													totalUVs													= 0;
	uint32_t													totalFaces													= 0;
	uint32_t													byteOffsetStartModel										= byteOffset;
	uint32_t													meshCountIBelieve											= 0;

	while(byteOffset < (input.size() - 40)) {
		::llc::SRSMNode												newNode														= {};
		const char													* modelName													= (const char*)&input[byteOffset];
		byteOffset												+= 40;
		newNode.Name											= modelName;
		info_printf("---------------------------------------------- Reading mesh node: %u ----------------------------------------------", (uint32_t)iMesh);
		info_printf("Mesh node name: %s.", modelName);
		if(0 == iMesh) {
			meshCountIBelieve										= *(uint32_t*)&input[byteOffset];			// Get the number of textures
			byteOffset												+= sizeof(uint32_t);
			info_printf("Mesh count maybe?: %u.", meshCountIBelieve);
		}
		const char													* parentName												= (const char*)&input[byteOffset];
		byteOffset												+= 40;
		newNode.ParentName										= parentName;
		info_printf("Parent node name: %s.", parentName);
		if(0 == iMesh) {
			{
				ubyte_t														uUnknown[3 * 4];
				memcpy(uUnknown, &input[byteOffset], ::llc::size(uUnknown));
				byteOffset												+= ::llc::size(uUnknown);
				analyzeArray(uUnknown);
				//for(uint32_t i=0; i < ::llc::size(uUnknown); ++i)
				//	info_printf("Unknown: %u.", uUnknown[i]);
			}
			{
				ubyte_t														iUnknown[7 * 4];
				memcpy(iUnknown, &input[byteOffset], ::llc::size(iUnknown));
				byteOffset												+= ::llc::size(iUnknown);
				//analyzeArray(iUnknown);
				//for(uint32_t i=0; i < ::llc::size(iUnknown); ++i)
				//	info_printf("Unknown: %i.", (int32_t)iUnknown[i]);
			}
		}
		{
			uint32_t													texMappingCount												= *(uint32_t*)&input[byteOffset];			// Get the number of texture indices for this model
			info_printf("Texture index count: %u.", texMappingCount);
			byteOffset												+= sizeof(uint32_t);
			if(texMappingCount) {
				::llc::array_pod<int32_t>									& modelTextures												= newNode.TextureIndices;	
				modelTextures.resize(texMappingCount);
				memcpy(modelTextures.begin(), &input[byteOffset], sizeof(uint32_t) * texMappingCount);
				byteOffset												+= sizeof(uint32_t) * texMappingCount;
			}
		}

		newNode.Transform										= *(SRSMNodeTransform*)&input[byteOffset];			// Get model transform
		byteOffset												+= sizeof(SRSMNodeTransform);
		info_printf("Node transform (Row0	): {%f, %f, %f}."		, newNode.Transform.Row0	.x, newNode.Transform.Row0		.y, newNode.Transform.Row0		.z);
		info_printf("Node transform (Row2	): {%f, %f, %f}."		, newNode.Transform.Row2	.x, newNode.Transform.Row2		.y, newNode.Transform.Row2		.z);
		info_printf("Node transform (Row1	): {%f, %f, %f}."		, newNode.Transform.Row1	.x, newNode.Transform.Row1		.y, newNode.Transform.Row1		.z);
		info_printf("Node transform (Row3	): {%f, %f, %f}."		, newNode.Transform.Row3	.x, newNode.Transform.Row3		.y, newNode.Transform.Row3		.z);
		info_printf("Node transform (Unk	): {%f, %f, %f, %f}."	, newNode.Transform.Unk		.x, newNode.Transform.Unk		.y, newNode.Transform.Unk		.z, newNode.Transform.Unk		.w);
		info_printf("Node transform (Offset	): {%f, %f, %f}."		, newNode.Transform.Offset	.x, newNode.Transform.Offset	.y, newNode.Transform.Offset	.z);
		info_printf("Node transform (Scale	): {%f, %f, %f}."		, newNode.Transform.Scale	.x, newNode.Transform.Scale		.y, newNode.Transform.Scale		.z);

		{
			uint32_t													vertexCount													= *(uint32_t*)&input[byteOffset];			// Get the number of vertex
			byteOffset												+= sizeof(uint32_t);
			info_printf("Vertex count: %u.", vertexCount);
			if(vertexCount) {
				::llc::array_pod<::llc::SCoord3<float>>						& modelVertices												= newNode.Vertices;	
				modelVertices.resize(vertexCount);
				memcpy(modelVertices.begin(), &input[byteOffset], sizeof(::llc::SCoord3<float>) * vertexCount);
				byteOffset												+= sizeof(::llc::SCoord3<float>) * vertexCount;
			}
		}
		{
			uint32_t													texVtxCount													= *(uint32_t*)&input[byteOffset];			// Get the number of unk
			byteOffset												+= sizeof(uint32_t);
			info_printf("UV coord count: %u.", texVtxCount);
			if(texVtxCount) {
				::llc::array_pod<::llc::SCoord3<float>>						& modelUNKs													= newNode.UVs;	
				modelUNKs.resize(texVtxCount);
				memcpy(modelUNKs.begin(), &input[byteOffset], sizeof(::llc::SCoord3<float>) * texVtxCount);
				byteOffset												+= sizeof(::llc::SCoord3<float>) * texVtxCount;
				//for(uint32_t i=0; i < modelUNKs.size(); ++i)
				//	info_printf("UV: {%f, %f, %f}.", modelUNKs[i].x, modelUNKs[i].y, modelUNKs[i].z);

			}
		}
		{
			uint32_t													faceCount													= *(uint32_t*)&input[byteOffset];			// Get the number of face
			byteOffset												+= sizeof(uint32_t);
			info_printf("Face count: %u.", faceCount);
			if(faceCount) {
				::llc::array_pod<SRSMFace>									& modelFaces												= newNode.Faces;		
				modelFaces.resize(faceCount);
				memcpy(modelFaces.begin(), &input[byteOffset], sizeof(SRSMFace) * faceCount);
				byteOffset												+= sizeof(SRSMFace) * faceCount;
			}
		}
		{
			uint32_t													keyframeCount												= *(uint32_t*)&input[byteOffset];			// Get the number of keyframe
			byteOffset												+= sizeof(uint32_t);
			info_printf("Rotation keyframe count: %u.", keyframeCount);
			if(keyframeCount) {
				::llc::array_pod<SRSMFrameRotation>							modelKeyframes;	
				modelKeyframes.resize(keyframeCount);
				memcpy(modelKeyframes.begin(), &input[byteOffset], sizeof(SRSMFrameRotation) * keyframeCount);
				byteOffset												+= sizeof(SRSMFrameRotation) * keyframeCount;
			}
		}
		//	fread (&ntextures, sizeof(int), 1, fp);
			//	if (main)
//		fread (ftodo, sizeof(float), 10, fp);
		loaded.Nodes.push_back(newNode);
		totalVertices	+= newNode.Vertices	.size();
		totalUVs		+= newNode.UVs		.size();
		totalFaces		+= newNode.Faces	.size();

		++iMesh;
	}
	info_printf("Total Vertices        : %u.", totalVertices	);
	info_printf("Total UVs             : %u.", totalUVs			);
	info_printf("Total Faces           : %u.", totalFaces		);
	info_printf("Total Node bytes read : %u.", (byteOffset - byteOffsetStartModel));
	return byteOffset;
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
	return rsmFileLoad(loaded, {fileInMemory.begin(), fileInMemory.size() - 8});
}