#include "llc_ro_rsw.h"
#include "llc_coord.h"

#pragma pack(push, 1)
	struct SRSWHeader {
					char													Filecode[4];
					uint8_t													VersionMajor; 
					uint8_t													VersionMinor; 
	};

	//struct SRSWWorldInfoFilenames {
	//				unsigned char											INIFilename	[40];
	//				unsigned char											GNDFilename	[40];
	//				unsigned char											GATFilename	[40];
	//				unsigned char											SOMFilename	[40];
	//};

	struct SRSWWorldInfo {
					float													WaterHeight		;
					uint32_t												WaterType		;
					float													WaterAmplitude	;
					float													WaterSpeed		;
					float													WaterPitch		;
					uint32_t												WaterTexCycling	;
					int32_t													Longitude		;
					int32_t													Latitude		;
					float													Diffuse[3];
					float													Ambient[3];
					float													Intensity;
					int32_t													Top;
					int32_t													Bottom;
					int32_t													Left;
					int32_t													Right;
					uint32_t												ObjectCount;
					//unsigned char											Unknown2	[8];
					//unsigned char											UnknownStr	[40];
					//uint32_t												UnknownInts	[2];
	};
#pragma pack(pop)

			::llc::error_t								llc::rswFileLoad											(::llc::SRSWFileContents& loaded, const ::llc::array_view<ubyte_t>	& input)							{
	uint32_t													byteOffset													= 0;
	SRSWHeader													header														= *(SRSWHeader*)input.begin();
	byteOffset												+= sizeof(SRSWHeader);
	info_printf("RSW magic number: %s.", header.Filecode);
	info_printf("RSW version major: 0x%x.", (uint32_t)header.VersionMajor);
	info_printf("RSW version minor: 0x%x.", (uint32_t)header.VersionMinor);
	info_printf("RSW version number: 0x%x.", (uint32_t)*(uint16_t*)&input[4]);
	//info_printf("RSW version: 0x%x.", header.Version);
	loaded.INIFilename										= (const char*)&input[byteOffset]; byteOffset	+= 40;
	loaded.GNDFilename										= (const char*)&input[byteOffset]; byteOffset	+= 40;
	if(header.VersionMajor > 1 || (header.VersionMajor == 1 && header.VersionMinor >= 4))
		loaded.GATFilename										= (const char*)&input[byteOffset]; byteOffset	+= 40;
	loaded.SOMFilename										= (const char*)&input[byteOffset]; byteOffset	+= 40;

	info_printf("RSW INI: %s.", &loaded.INIFilename[0]);
	info_printf("RSW GND: %s.", &loaded.GNDFilename[0]);
	info_printf("RSW GAT: %s.", &loaded.GATFilename[0]);
	info_printf("RSW SOM: %s.", &loaded.SOMFilename[0]);
	SRSWWorldInfo												worldInfo;
	if(header.VersionMajor > 1 || (header.VersionMajor == 1 && header.VersionMinor >= 3)) {
		worldInfo.WaterHeight		= *(float*)&input[byteOffset]; byteOffset += sizeof(float);
	}
	if(header.VersionMajor > 1 || (header.VersionMajor == 1 && header.VersionMinor >= 8)) {
		worldInfo.WaterType			= *(int32_t	*)&input[byteOffset]; byteOffset += sizeof(int32_t);
		worldInfo.WaterAmplitude	= *(float	*)&input[byteOffset]; byteOffset += sizeof(float);
		worldInfo.WaterSpeed		= *(float	*)&input[byteOffset]; byteOffset += sizeof(float);
		worldInfo.WaterPitch		= *(float	*)&input[byteOffset]; byteOffset += sizeof(float);
	}
	if(header.VersionMajor > 1 || (header.VersionMajor == 1 && header.VersionMinor >= 9))  {
		worldInfo.WaterTexCycling	= *(int32_t	*)&input[byteOffset]; byteOffset += sizeof(int32_t);
	}
	//else {
	//	// throw "todo";
	//}
	//
	if(header.VersionMajor > 1 || (header.VersionMajor == 1 && header.VersionMinor >= 5))  {
		worldInfo.Latitude			= *(int32_t	*)&input[byteOffset]; byteOffset += sizeof(int32_t);
		worldInfo.Longitude			= *(int32_t	*)&input[byteOffset]; byteOffset += sizeof(int32_t);
		memcpy(worldInfo.Diffuse, &input[byteOffset], sizeof(float) * 3); byteOffset += sizeof(float) * 3;
		memcpy(worldInfo.Ambient, &input[byteOffset], sizeof(float) * 3); byteOffset += sizeof(float) * 3;
	}
	if(header.VersionMajor > 1 || (header.VersionMajor == 1 && header.VersionMinor >= 7)) {
		worldInfo.Intensity			= *(float	*)&input[byteOffset]; byteOffset += sizeof(float);
	}

	if(header.VersionMajor > 1 || (header.VersionMajor == 1 && header.VersionMinor >= 6)) {
		worldInfo.Top				= *(int32_t	*)&input[byteOffset]; byteOffset += sizeof(int32_t);
		worldInfo.Bottom			= *(int32_t	*)&input[byteOffset]; byteOffset += sizeof(int32_t);
		worldInfo.Left				= *(int32_t	*)&input[byteOffset]; byteOffset += sizeof(int32_t);
		worldInfo.Right				= *(int32_t	*)&input[byteOffset]; byteOffset += sizeof(int32_t);
	}
	else {
		worldInfo.Top				= -500;
		worldInfo.Bottom			= 500;
		worldInfo.Left				= -500;
		worldInfo.Right				= 500;
	}

	worldInfo.ObjectCount										= *(int32_t	*)&input[byteOffset]; 
	byteOffset													+= sizeof(int32_t);
	//memcpy(&worldInfo, &input[byteOffset], sizeof(SRSWWorldInfo));
	//byteOffset													+= sizeof(SRSWWorldInfo);
	SModelInfoRSW												modelInfo;

	while(byteOffset < input.size()) {
		int32_t														objectType											= *(int32_t	*)&input[byteOffset]; 
		byteOffset												+= sizeof(int32_t);
		switch(objectType) {
		default		: break;
		case	1	: // RSM Model
			if(header.VersionMajor > 1 || (header.VersionMajor == 1 && header.VersionMinor >= 3)) {
				modelInfo.Name											= (const char*)&input[byteOffset]; byteOffset	+= 40;
				modelInfo.AnimType										= *(int32_t	*)&input[byteOffset]; byteOffset += sizeof(int32_t);
				modelInfo.AnimSpeed										= *(float	*)&input[byteOffset]; byteOffset += sizeof(float);
				modelInfo.BlockType										= *(int32_t	*)&input[byteOffset]; byteOffset += sizeof(int32_t);
			}
			modelInfo.Filename										= (const char*)&input[byteOffset]; byteOffset	+= 40;
			modelInfo.Str2											= (const char*)&input[byteOffset]; byteOffset	+= 40;
			modelInfo.RootRSMNode									= (const char*)&input[byteOffset]; byteOffset	+= 40;
			modelInfo.Str4											= (const char*)&input[byteOffset]; byteOffset	+= 40;
			memcpy(&modelInfo.Position	, &input[byteOffset], sizeof(float) * 3); byteOffset += sizeof(float) * 3;
			memcpy(&modelInfo.Rotation	, &input[byteOffset], sizeof(float) * 3); byteOffset += sizeof(float) * 3;
			memcpy(&modelInfo.Scale		, &input[byteOffset], sizeof(float) * 3); byteOffset += sizeof(float) * 3;
			info_printf(" ---------------------------------------------------------------------------------- RSW Model: %u ---------------------------------------------------------------------------------- ", loaded.RSWModels.size());
			info_printf("RSW object found     : %s.", &modelInfo.Name[0]);
			info_printf("RSW object filename  : %s.", &modelInfo.Filename[0]);
			info_printf("RSW object str2      : %s.", &modelInfo.Str2[0]);
			info_printf("RSW object str3      : %s.", &modelInfo.RootRSMNode[0]);
			info_printf("RSW object str4      : %s.", &modelInfo.Str4[0]);
			info_printf("RSW object position  : {%f, %f, %f}.", modelInfo.Position	.x, modelInfo.Position	.y, modelInfo.Position	.z);
			info_printf("RSW object rotation  : {%f, %f, %f}.", modelInfo.Rotation	.x, modelInfo.Rotation	.y, modelInfo.Rotation	.z);
			info_printf("RSW object scale     : {%f, %f, %f}.", modelInfo.Scale		.x, modelInfo.Scale		.y, modelInfo.Scale		.z);
			info_printf("RSW object animType  : %i.", modelInfo.AnimType );
			info_printf("RSW object animSpeed : %f.", modelInfo.AnimSpeed);
			info_printf("RSW object blockType : %i.", modelInfo.BlockType);
			if(0 == modelInfo.Filename[0])
				return 0;
			loaded.RSWModels.push_back(modelInfo);
			break;
		case	2	: // Light
			break;
		case	3	: // Sound
			break;
		case	4	: // Effect
			break;
		}
	}
	info_printf("RSW objects loaded: %u.", loaded.RSWModels.size());
	return 0;
}

			::llc::error_t								llc::rswFileLoad											(::llc::SRSWFileContents& loaded, FILE								* input)							{ 
	loaded, input;
	return 0; 
}

			::llc::error_t								llc::rswFileLoad											(::llc::SRSWFileContents& loaded, const ::llc::view_const_string	& input)							{ 
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
	uint64_t													unk															= *(uint64_t*)&fileInMemory[fileInMemory.size() - 8];
	info_printf("Unk64: %u", unk);

	info_printf("Parsing RSW file: %s.", input.begin());
	return rswFileLoad(loaded, {fileInMemory.begin(), fileInMemory.size() - 8});
}
