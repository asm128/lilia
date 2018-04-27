#include "llc_ro_rsw.h"
#include "llc_coord.h"

#pragma pack(push, 1)
	struct SRSWHeader {
					char													Filecode[4];
					char													VersionMinor; 
					char													VersionMajor; 
	};


	struct SRSWWorldInfo {
					unsigned char											Unknown0	[40];
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
					uint32_t												Top;
					uint32_t												Bottom;
					uint32_t												Left;
					uint32_t												Right;
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
	info_printf("RSW version: %u.%u.", header.VersionMajor, header.VersionMinor);
	//info_printf("RSW version: 0x%x.", header.Version);
	loaded.INIFilename	= (const char*)&input[byteOffset]; byteOffset	+= 40;
	loaded.GNDFilename	= (const char*)&input[byteOffset]; byteOffset	+= 40;
	if(header.VersionMinor >= 2)
		loaded.GATFilename	= (const char*)&input[byteOffset]; byteOffset	+= 40;
	//iniFile = file->readString(40); // ehh...read inifile twice?

	info_printf("RSW INI: %s.", &loaded.INIFilename[0]);
	info_printf("RSW GND: %s.", &loaded.GNDFilename[0]);
	info_printf("RSW GAT: %s.", &loaded.GATFilename[0]);
	SRSWWorldInfo												worldInfo;
	SRSWWorldObject												worldObject;
	::llc::array_view<uint32_t>									u0232														= {(uint32_t*)	worldInfo.Unknown0, 12 / 4};
	::llc::array_view<int32_t>									i0232														= {(int32_t*)	worldInfo.Unknown0, 12 / 4};
	::llc::array_view<uint16_t>									u0216														= {(uint16_t*)	worldInfo.Unknown0, 12 / 2};
	::llc::array_view<int16_t>									i0216														= {(int16_t*)	worldInfo.Unknown0, 12 / 2};
	::llc::array_view<float>									f02															= {(float*)		worldInfo.Unknown0, 12 / 4};
																																										   
	//::llc::array_view<uint32_t>									u0432														= {(uint32_t*)	worldInfo.Unknown2, (176 - 60) / 4};
	//::llc::array_view<int32_t>									i0432														= {(int32_t*)	worldInfo.Unknown2, (176 - 60) / 4};
	//::llc::array_view<uint16_t>									u0416														= {(uint16_t*)	worldInfo.Unknown2, (176 - 60) / 2};
	//::llc::array_view<int16_t>									i0416														= {(int16_t*)	worldInfo.Unknown2, (176 - 60) / 2};
	//::llc::array_view<float>									f04															= {(float*)		worldInfo.Unknown2, (176 - 60) / 4};

	::llc::array_view<uint32_t>									u032														= {(uint32_t*)	worldObject.part0, (30 * 4) / 4};
	::llc::array_view<int32_t>									i032														= {(int32_t*)	worldObject.part0, (30 * 4) / 4};
	::llc::array_view<uint16_t>									u016														= {(uint16_t*)	worldObject.part0, (30 * 4) / 2};
	::llc::array_view<int16_t>									i016														= {(int16_t*)	worldObject.part0, (30 * 4) / 2};
	::llc::array_view<float>									f0															= {(float*)		worldObject.part0, (30 * 4) / 4};
	::llc::array_view<uint32_t>									u132														= {(uint32_t*)	worldObject.somePath, (212 - 169) / 4};
	::llc::array_view<int32_t>									i132														= {(int32_t*)	worldObject.somePath, (212 - 169) / 4};
	::llc::array_view<uint16_t>									u116														= {(uint16_t*)	worldObject.somePath, (212 - 169) / 2};
	::llc::array_view<int16_t>									i116														= {(int16_t*)	worldObject.somePath, (212 - 169) / 2};
	::llc::array_view<float>									f1															= {(float*)		worldObject.somePath, (212 - 169) / 4};

	sizeof(SRSWWorldObject);
	memcpy(&worldInfo, &input[byteOffset], sizeof(SRSWWorldInfo));
	byteOffset													+= sizeof(SRSWWorldInfo);
	while(byteOffset < input.size()) {
		::std::string													modelName											= (const char*)&input[byteOffset]; 
		byteOffset													+= 40;
		if(0 == modelName.size())
			break;
		else {
			memcpy(&worldObject, &input[byteOffset], 212);
			byteOffset													+= 212;
		}
		loaded.RSWFilenames	.push_back(modelName);
		loaded.RSWObjects	.push_back(worldObject);
		info_printf("RSW object found    : %s.", &modelName[0]);
		info_printf("RSW object position : {%f, %f, %f}.", worldObject.Position	.x, worldObject.Position	.y, worldObject.Position	.z);
		info_printf("RSW object rotation : {%f, %f, %f}.", worldObject.Rotation	.x, worldObject.Rotation	.y, worldObject.Rotation	.z);
		info_printf("RSW object scale    : {%f, %f, %f}.", worldObject.Scale	.x, worldObject.Scale		.y, worldObject.Scale		.z);
		info_printf("RSW object unk0     : %i.", worldObject.u0);
		info_printf("RSW object unk1     : %f.", worldObject.uf0);
	}
	info_printf("RSW objects loaded: %u.", loaded.RSWObjects.size());
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
	uint64_t unk = *(uint64_t*)&fileInMemory[fileInMemory.size() - 8];
	info_printf("%u", unk);

	info_printf("Parsing RSW file: %s.", input.begin());
	return rswFileLoad(loaded, {fileInMemory.begin(), fileInMemory.size() - 8});
}
