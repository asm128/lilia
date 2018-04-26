#include "llc_ro_gnd.h"
#include "llc_grid_view.h"

				::llc::error_t								llc::gndFileLoad										(::llc::SGNDFileContents& loaded, const ::llc::array_view<ubyte_t>	& input)							{
	uint32_t														nMagicHeader									= *(uint32_t*)input.begin();
#if defined (LLC_ANDROID) || defined(LLC_LINUX)
	ree_if(nMagicHeader != 0x4E475247UL , "Invalid GND file header.");
#elif defined(LLC_WINDOWS)
	ree_if(nMagicHeader != 'NGRG', "Invalid GND file header.");
#endif
	uint8_t															nVersionMajor									= input[4];
	uint8_t															nVersionMinor									= input[5];
	ree_if((nVersionMajor < 1) || (nVersionMajor == 1 && nVersionMinor < 5), "Invalid GND file version. Major version: %u, Minor version: %u", (int)nVersionMajor, (int)nVersionMinor);
	int32_t															byteOffset										= 6;
	uint32_t														nTextureCount									= 0;
	uint32_t														nTextureStringSize								= 0;
	int32_t 
		byteCount = (int32_t)sizeof(loaded.Metrics	);	memcpy(&loaded.Metrics		, &input[byteOffset], byteCount);	byteOffset += byteCount;
		byteCount = (int32_t)sizeof(uint32_t		);	memcpy(&nTextureCount		, &input[byteOffset], byteCount);	byteOffset += byteCount;
		byteCount = (int32_t)sizeof(uint32_t		);	memcpy(&nTextureStringSize	, &input[byteOffset], byteCount);	byteOffset += byteCount;
	loaded.TextureNames.resize(nTextureCount);
	for(uint32_t iTexture = 0; iTexture < nTextureCount; ++iTexture) 
		loaded.TextureNames[iTexture].resize(nTextureStringSize);

	byteCount													= nTextureStringSize;
	for(uint32_t iTexture = 0; iTexture < nTextureCount; ++iTexture) {
		memcpy(&loaded.TextureNames[iTexture][0], &input[byteOffset], byteCount);
		byteOffset													+= byteCount;
	}
	uint32_t														tileCountBrightness								= 0; 
		byteCount = (int32_t)sizeof(uint32_t);			memcpy(&tileCountBrightness		, &input[byteOffset], byteCount);	byteOffset += byteCount;
		byteCount = (int32_t)sizeof(uint32_t);			memcpy(&loaded.LightmapSize.x	, &input[byteOffset], byteCount);	byteOffset += byteCount;
		byteCount = (int32_t)sizeof(uint32_t);			memcpy(&loaded.LightmapSize.y	, &input[byteOffset], byteCount);	byteOffset += byteCount;
		byteCount = (int32_t)sizeof(uint32_t);			memcpy(&loaded.LightmapTiles	, &input[byteOffset], byteCount);	byteOffset += byteCount;
	loaded.lstTileBrightnessData.resize(tileCountBrightness);	byteCount = (int32_t)(sizeof(STileBrightnessGND) * tileCountBrightness);	memcpy(loaded.lstTileBrightnessData	.begin(), &input[byteOffset], byteCount);	byteOffset += byteCount; 

	uint32_t														tileCountSkin									= *(uint32_t*)&input[byteOffset]; byteOffset += sizeof(uint32_t); 
	loaded.lstTileTextureData	.resize(tileCountSkin);			byteCount = (int32_t)(sizeof(STileSkinGND) * tileCountSkin);				memcpy(loaded.lstTileTextureData	.begin(), &input[byteOffset], byteCount);	byteOffset += byteCount; 

	uint32_t														tileCountGeometry								= loaded.Metrics.Size.x * loaded.Metrics.Size.y;//*(uint32_t*)&input[byteOffset]; byteOffset													+= sizeof(uint32_t); 
	loaded.lstTileGeometryData.resize(tileCountGeometry); 
	if( nVersionMajor > 1 || ( nVersionMajor == 1 && nVersionMinor >= 7 ) ) {
		byteCount													= (int32_t)((sizeof(STileGeometryGND)) * tileCountGeometry);		
		memcpy(loaded.lstTileGeometryData	.begin(), &input[byteOffset], byteCount);	
		byteOffset													+= byteCount; 
	}
	else if( nVersionMajor < 1 || ( nVersionMajor == 1 && nVersionMinor <= 5 ) ) {// it seems old 1.5 format used 16 bit integers
		for(uint32_t iTile = 0; iTile < tileCountGeometry; ++iTile) {
			int16_t															top												= -1;
			int16_t															right											= -1;
			int16_t															front											= -1;
			int16_t															flags											= -1;
			::llc::STileGeometryGND												& tileGeometry									= loaded.lstTileGeometryData[iTile];
			byteCount													= (int32_t)(sizeof(float) * 4);		
			memcpy(tileGeometry.fHeight, &input[byteOffset], byteCount);	
			byteOffset													+= byteCount; 
			byteCount													= 2;
			memcpy( &top	, &input[byteOffset], byteCount);	byteOffset += byteCount; 
			memcpy( &right	, &input[byteOffset], byteCount);	byteOffset += byteCount; 
			memcpy( &front	, &input[byteOffset], byteCount);	byteOffset += byteCount; 
			memcpy( &flags	, &input[byteOffset], byteCount);	byteOffset += byteCount; 
			tileGeometry.SkinMapping.SkinIndexTop						= top	;
			tileGeometry.SkinMapping.SkinIndexRight						= right	;
			tileGeometry.SkinMapping.SkinIndexFront						= front	;
			//tileGeometry.Flags											= flags	;
		}
	}
	return byteOffset;
}

			::llc::error_t								llc::gndFileLoad											(::llc::SGNDFileContents& loaded, FILE								* input)							{ 
	loaded, input;
	return 0; 
}

			::llc::error_t								llc::gndFileLoad											(::llc::SGNDFileContents& loaded, const ::llc::view_const_string	& input)							{ 
	FILE														* fp												= 0;
	ree_if(0 != fopen_s(&fp, input.begin(), "rb"), "Cannot open .gnd file: %s.", input.begin());

	fseek(fp, 0, SEEK_END);
	int32_t														fileSize											= (int32_t)ftell(fp);
	fseek(fp, 0, SEEK_SET);
	::llc::array_pod<ubyte_t>									fileInMemory										= 0;
	llc_necall(fileInMemory.resize(fileSize), "File too large? : %llu.", (uint64_t)fileSize);
	if(fileSize != (int32_t)fread(fileInMemory.begin(), sizeof(ubyte_t), fileSize, fp)) {
		fclose(fp);
		error_printf("fread() failed! file: '%s'.", input.begin());
		return -1;
	}
	fclose(fp);

	return gndFileLoad(loaded, fileInMemory);
}

#pragma warning(disable : 4100)
static		::llc::error_t									gndGenerateFaceGeometryRight							(uint32_t baseX, uint32_t baseZ, float tileScale, const ::llc::STileGeometryGND & geometryTile, const ::llc::STileGeometryGND * geometryTileRight, const ::llc::array_view<::llc::STileSkinGND>& lstTileSkinData, ::llc::SModelNodeGND& generated, ::llc::STileMapping & out_mapping)	{ 
	const uint32_t													baseVertexIndex											= generated.Vertices.size();
	const ::llc::SCoord3<float>										faceVerts	[4]											= 
		{ {baseX + 0.0f, (geometryTile.fHeight[2] / tileScale), baseZ + 1.0f}
		, {baseX + 1.0f, (geometryTile.fHeight[3] / tileScale), baseZ + 1.0f}
		, {baseX + 0.0f, geometryTileRight ? (geometryTileRight->fHeight[0] / tileScale) : 0.0f, baseZ + 1.0f}
		, {baseX + 1.0f, geometryTileRight ? (geometryTileRight->fHeight[1] / tileScale) : 0.0f, baseZ + 1.0f}
		};
	const ::llc::SCoord3<float>										faceNormals	[4]											=
		{ (faceVerts[1] - faceVerts[0]).Cross(faceVerts[2] - faceVerts[0]).Normalize()
		, (faceVerts[3] - faceVerts[1]).Cross(faceVerts[0] - faceVerts[1]).Normalize()
		, (faceVerts[0] - faceVerts[2]).Cross(faceVerts[3] - faceVerts[2]).Normalize()
		, (faceVerts[2] - faceVerts[3]).Cross(faceVerts[1] - faceVerts[3]).Normalize()
		};

	generated.Vertices		.append(faceVerts	);
	generated.Normals		.append(faceNormals	);
	for(uint32_t i = 0; i < 4; ++i)
		out_mapping.VerticesRight[i]								= baseVertexIndex + i;

	{
		const int32_t													faceSkins	[4]										= 
			{ geometryTile.SkinMapping.SkinIndexRight
			, geometryTile.SkinMapping.SkinIndexRight
			, geometryTile.SkinMapping.SkinIndexRight
			, geometryTile.SkinMapping.SkinIndexRight
			};
		//generated.SkinIndices.append(faceSkins);
		const ::llc::STileSkinGND										& skinTile											= lstTileSkinData[faceSkins[0]];
		::llc::SCoord2<float>											faceUVs	[4]											;
		for(uint32_t i = 0; i < 4; ++i)
			faceUVs[i]													= {skinTile.u[i], skinTile.v[i]};
		generated.UVs.append(faceUVs);
	}
	{
		const ::llc::STriangleWeights<uint32_t>							faceIndices[6]										= 
			{	{ baseVertexIndex + 0 // + 0//+ 0 // 0
				, baseVertexIndex + 1 // + 1//+ 2 // 1
				, baseVertexIndex + 2 // + 2//+ 1 // 2
				} //			 //// 	+	 //// 
			,	{ baseVertexIndex + 1 // + 1//+ 1 // 1
				, baseVertexIndex + 3 // + 3//+ 2 // 3
				, baseVertexIndex + 2 // + 2//+ 3 // 2
			}
			};
		generated.VertexIndices	.append(faceIndices);
	}	
	return 0; 
}

static		::llc::error_t									gndGenerateFaceGeometryFront							(uint32_t baseX, uint32_t baseZ, float tileScale, const ::llc::STileGeometryGND & geometryTile, const ::llc::STileGeometryGND * geometryTileFront, const ::llc::array_view<::llc::STileSkinGND>& lstTileSkinData, ::llc::SModelNodeGND& generated, ::llc::STileMapping & out_mapping)	{ 
	const uint32_t													baseVertexIndex											= generated.Vertices.size();
	const ::llc::SCoord3<float>										faceVerts	[4]											= 
		{ {baseX + 1.0f, (geometryTile.fHeight[3] / tileScale), baseZ + 1.0f}
		, {baseX + 1.0f, (geometryTile.fHeight[1] / tileScale), baseZ + 0.0f}
		, {baseX + 1.0f, geometryTileFront ? (geometryTileFront->fHeight[2] / tileScale) : 0.0f, baseZ + 1.0f}
		, {baseX + 1.0f, geometryTileFront ? (geometryTileFront->fHeight[0] / tileScale) : 0.0f, baseZ + 0.0f}
		};
	const ::llc::SCoord3<float>										faceNormals	[4]											=
		{ (faceVerts[1] - faceVerts[0]).Cross(faceVerts[2] - faceVerts[0]).Normalize()
		, (faceVerts[3] - faceVerts[1]).Cross(faceVerts[0] - faceVerts[1]).Normalize()
		, (faceVerts[0] - faceVerts[2]).Cross(faceVerts[3] - faceVerts[2]).Normalize()
		, (faceVerts[2] - faceVerts[3]).Cross(faceVerts[1] - faceVerts[3]).Normalize()
		};
	generated.Vertices		.append(faceVerts	);
	generated.Normals		.append(faceNormals	);
	for(uint32_t i = 0; i < 4; ++i)
		out_mapping.VerticesFront[i]								= baseVertexIndex + i;
	{
		const int32_t													faceSkins	[4]										= 
			{ geometryTile.SkinMapping.SkinIndexFront
			, geometryTile.SkinMapping.SkinIndexFront
			, geometryTile.SkinMapping.SkinIndexFront
			, geometryTile.SkinMapping.SkinIndexFront
			};
		//generated.SkinIndices.append(faceSkins);
		const ::llc::STileSkinGND										& skinTile											= lstTileSkinData[faceSkins[0]];
		::llc::SCoord2<float>											faceUVs	[4]											;
		for(uint32_t i = 0; i < 4; ++i)
			faceUVs[i]													= {skinTile.u[i], skinTile.v[i]};
		generated.UVs.append(faceUVs);
		const ::llc::STriangleWeights<uint32_t>							faceIndices[6]										= 
			{	{ baseVertexIndex + 0 // + 0//+ 0 // 0
				, baseVertexIndex + 1 // + 1//+ 2 // 1
				, baseVertexIndex + 2 // + 2//+ 1 // 2
				} //			 //// 	+	 //// 
			,	{ baseVertexIndex + 1 // + 1//+ 1 // 1
				, baseVertexIndex + 3 // + 3//+ 2 // 3
				, baseVertexIndex + 2 // + 2//+ 3 // 2
			}
			};
		generated.VertexIndices	.append(faceIndices);
	}	
	return 0; 
}
#pragma warning(default : 4100)

static		::llc::error_t									gndGenerateFaceGeometryTop								(uint32_t baseX, uint32_t baseZ, float tileScale, const ::llc::STileGeometryGND & geometryTile, const ::llc::array_view<::llc::STileSkinGND>& lstTileSkinData, ::llc::SModelNodeGND& generated, ::llc::STileMapping & out_mapping)	{
	const uint32_t													baseVertexIndex											= generated.Vertices.size();
	const ::llc::SCoord3<float>										faceVerts	[4]											= 
		{ {baseX + 0.0f, (geometryTile.fHeight[0] / tileScale), baseZ + 0.0f}
		, {baseX + 1.0f, (geometryTile.fHeight[1] / tileScale), baseZ + 0.0f}
		, {baseX + 0.0f, (geometryTile.fHeight[2] / tileScale), baseZ + 1.0f}
		, {baseX + 1.0f, (geometryTile.fHeight[3] / tileScale), baseZ + 1.0f}
		};
	const ::llc::SCoord3<float>										faceNormals	[4]											=
		{ (faceVerts[1] - faceVerts[0]).Cross(faceVerts[2] - faceVerts[0]).Normalize()
		, (faceVerts[3] - faceVerts[1]).Cross(faceVerts[0] - faceVerts[1]).Normalize()
		, (faceVerts[0] - faceVerts[2]).Cross(faceVerts[3] - faceVerts[2]).Normalize()
		, (faceVerts[2] - faceVerts[3]).Cross(faceVerts[1] - faceVerts[3]).Normalize()
		};
	//::llc::SCoord3<float>											normal												
	//	= faceNormals	[0]
	//	+ faceNormals	[1]
	//	+ faceNormals	[2]
	//	+ faceNormals	[3]
	//	;
	//(normal /=4).Normalize();
	//faceNormals	[0] = normal;
	//faceNormals	[1] = normal;
	//faceNormals	[2] = normal;
	//faceNormals	[3] = normal;
	generated.Vertices		.append(faceVerts	);
	generated.Normals		.append(faceNormals	);
	for(uint32_t i = 0; i < 4; ++i)
		out_mapping.VerticesTop[i]								= baseVertexIndex + i;

	{
		const int32_t													faceSkins	[4]										= 
			{ geometryTile.SkinMapping.SkinIndexTop
			, geometryTile.SkinMapping.SkinIndexTop
			, geometryTile.SkinMapping.SkinIndexTop
			, geometryTile.SkinMapping.SkinIndexTop
			};
		//generated.SkinIndices.append(faceSkins);
		const ::llc::STileSkinGND										& skinTile											= lstTileSkinData[faceSkins[0]];
		::llc::SCoord2<float>											faceUVs	[4]											;
		for(uint32_t i = 0; i < 4; ++i)
			faceUVs[i]													= {skinTile.u[i], skinTile.v[i]};
		generated.UVs.append(faceUVs);
	}
	{
		const ::llc::STriangleWeights<uint32_t>							faceIndices[6]										= 
			{	{ baseVertexIndex + 0 // + 0//+ 0 // 0
				, baseVertexIndex + 1 // + 1//+ 2 // 1
				, baseVertexIndex + 2 // + 2//+ 1 // 2
				} //			 //// 	+	 //// 
			,	{ baseVertexIndex + 1 // + 1//+ 1 // 1
				, baseVertexIndex + 3 // + 3//+ 2 // 3
				, baseVertexIndex + 2 // + 2//+ 3 // 2
			}
			};
		generated.VertexIndices	.append(faceIndices);
	}
	return 0;
}
			::llc::error_t										llc::gndGenerateFaceGeometry						(const ::llc::SGNDFileContents& loaded, TILE_FACE_FACING facing_direction, int32_t textureIndex, SModelNodeGND& generated, ::llc::grid_view<::llc::STileMapping> & out_mapping)	{
	::llc::grid_view<const ::llc::STileGeometryGND>						geometryView										= {loaded.lstTileGeometryData.begin(), loaded.Metrics.Size};
	for(uint32_t y = 0; y < geometryView.metrics().y; ++y)
	for(uint32_t x = 0; x < geometryView.metrics().x; ++x) {
		const ::llc::STileGeometryGND										& geometryTile										= geometryView[y][x];
		const ::llc::STileGeometryGND										* geometryTileFront									= ((x + 1) < geometryView.metrics().x) ? &geometryView	[y][x + 1]	: 0;
		const ::llc::STileGeometryGND										* geometryTileRight									= ((y + 1) < geometryView.metrics().y) ? &geometryView	[y + 1][x]	: 0;
		//const ::llc::STileGeometryGND										* geometryTileFrontRight							
		//	= ((x + 1) < geometryView.metrics().x 
		//	&& (y + 1) < geometryView.metrics().y) 
		//	? &geometryView	[y + 1][x + 1] : 0;
		::llc::TILE_FACE_FACING												facingDirection										= facing_direction;
		if(facingDirection == ::llc::TILE_FACE_FACING_FRONT && geometryTileFront && (geometryTileFront->fHeight[0] > geometryTile.fHeight[1] || geometryTileFront->fHeight[2] > geometryTile.fHeight[3])) facingDirection = ::llc::TILE_FACE_FACING_BACK;
		if(facingDirection == ::llc::TILE_FACE_FACING_BACK	&& geometryTileFront && (geometryTileFront->fHeight[0] < geometryTile.fHeight[1] || geometryTileFront->fHeight[2] < geometryTile.fHeight[3])) facingDirection = ::llc::TILE_FACE_FACING_FRONT;
		if(facingDirection == ::llc::TILE_FACE_FACING_RIGHT && geometryTileRight && (geometryTileRight->fHeight[0] > geometryTile.fHeight[2] || geometryTileRight->fHeight[1] > geometryTile.fHeight[3])) facingDirection = ::llc::TILE_FACE_FACING_LEFT;
		if(facingDirection == ::llc::TILE_FACE_FACING_LEFT	&& geometryTileRight && (geometryTileRight->fHeight[0] < geometryTile.fHeight[2] || geometryTileRight->fHeight[1] < geometryTile.fHeight[3])) facingDirection = ::llc::TILE_FACE_FACING_RIGHT;
		if(facingDirection == ::llc::TILE_FACE_FACING_FRONT && 0 == geometryTileFront && (0 > geometryTile.fHeight[1] || 0 > geometryTile.fHeight[3])) facingDirection = ::llc::TILE_FACE_FACING_BACK;
		if(facingDirection == ::llc::TILE_FACE_FACING_BACK	&& 0 == geometryTileFront && (0 < geometryTile.fHeight[1] || 0 < geometryTile.fHeight[3])) facingDirection = ::llc::TILE_FACE_FACING_FRONT;
		if(facingDirection == ::llc::TILE_FACE_FACING_RIGHT && 0 == geometryTileRight && (0 > geometryTile.fHeight[2] || 0 > geometryTile.fHeight[3])) facingDirection = ::llc::TILE_FACE_FACING_LEFT;
		if(facingDirection == ::llc::TILE_FACE_FACING_LEFT	&& 0 == geometryTileRight && (0 < geometryTile.fHeight[2] || 0 < geometryTile.fHeight[3])) facingDirection = ::llc::TILE_FACE_FACING_RIGHT;
		if(facingDirection != facing_direction)
			continue;
		switch(facing_direction) {
		default:
			break;
		//case TILE_FACE_FACING_BOTTOM: 
		case TILE_FACE_FACING_TOP	: 
			if(-1 == geometryTile.SkinMapping.SkinIndexTop)
				continue;
			if(textureIndex != -1 && textureIndex != loaded.lstTileTextureData[geometryTile.SkinMapping.SkinIndexTop].TextureIndex)
				continue;
			::gndGenerateFaceGeometryTop(x, y, loaded.Metrics.TileScale, geometryTile, loaded.lstTileTextureData, generated, out_mapping[y][x]);
			break;
		case TILE_FACE_FACING_BACK	: 
		case TILE_FACE_FACING_FRONT	: 
			if(-1 == geometryTile.SkinMapping.SkinIndexFront)
				continue;
			if(textureIndex != -1 && textureIndex != loaded.lstTileTextureData[geometryTile.SkinMapping.SkinIndexFront].TextureIndex)
				continue;
			::gndGenerateFaceGeometryFront(x, y, loaded.Metrics.TileScale, geometryTile, geometryTileFront, loaded.lstTileTextureData, generated, out_mapping[y][x]);
			break;
		case TILE_FACE_FACING_LEFT	: 
		case TILE_FACE_FACING_RIGHT	: 
			if(-1 == geometryTile.SkinMapping.SkinIndexRight)
				continue;
			if(textureIndex != -1 && textureIndex != loaded.lstTileTextureData[geometryTile.SkinMapping.SkinIndexRight].TextureIndex)
				continue;
			::gndGenerateFaceGeometryRight(x, y, loaded.Metrics.TileScale, geometryTile, geometryTileRight, loaded.lstTileTextureData, generated, out_mapping[y][x]);
			break;
		}
	}
	return 0;
}
