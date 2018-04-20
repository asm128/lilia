#include "llc_geometry.h"

// Vertex coordinates for cube faces
static constexpr const ::llc::STriangle3D<float>					geometryCubePositions			[12]						= 
	{ {{1.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f}}	// Right	- first		
	, {{1.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f}, {1.0f, 1.0f, 0.0f}}	// Right	- second	
	, {{0.0f, 0.0f, 1.0f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f, 0.0f}}	// Back		- first		
	, {{0.0f, 0.0f, 1.0f}, {0.0f, 1.0f, 1.0f}, {0.0f, 1.0f, 0.0f}}	// Back		- second	
	, {{1.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f, 0.0f}}	// Bottom	- first		
	, {{1.0f, 0.0f, 0.0f}, {1.0f, 0.0f, 1.0f}, {0.0f, 0.0f, 1.0f}}	// Bottom	- second	
	, {{1.0f, 0.0f, 1.0f}, {0.0f, 1.0f, 1.0f}, {0.0f, 0.0f, 1.0f}}	// Left		- first
	, {{1.0f, 0.0f, 1.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 1.0f, 1.0f}}	// Left		- second
	, {{1.0f, 0.0f, 1.0f}, {1.0f, 1.0f, 0.0f}, {1.0f, 1.0f, 1.0f}}	// Front	- first
	, {{1.0f, 0.0f, 1.0f}, {1.0f, 0.0f, 0.0f}, {1.0f, 1.0f, 0.0f}}	// Front	- second
	, {{1.0f, 1.0f, 0.0f}, {0.0f, 1.0f, 1.0f}, {1.0f, 1.0f, 1.0f}}	// Top		- first
	, {{1.0f, 1.0f, 0.0f}, {0.0f, 1.0f, 0.0f}, {0.0f, 1.0f, 1.0f}}	// Top		- second
	};

static constexpr	const ::llc::SCoord3<float>						geometryCubeNormalsTriangle		[12]						= 
	{ {0.0f, 0.0f, -1.0f}	// Right	- first			
	, {0.0f, 0.0f, -1.0f}	// Right	- second		

	, {-1.0f, 0.0f, 0.0f}	// Back		- first			
	, {-1.0f, 0.0f, 0.0f}	// Back		- second		

	, {0.0f, -1.0f, 0.0f}	// Bottom	- first			
	, {0.0f, -1.0f, 0.0f}	// Bottom	- second		

	, {0.0f, 0.0f, 1.0f}	// Left		- first
	, {0.0f, 0.0f, 1.0f}	// Left		- second

	, {1.0f, 0.0f, 0.0f}	// Front	- first
	, {1.0f, 0.0f, 0.0f}	// Front	- second

	, {0.0f, 1.0f, 0.0f}	// Top		- first
	, {0.0f, 1.0f, 0.0f}	// Top		- second
	};

// -----------------------------------------------------
static constexpr	const ::llc::STriangle3D<float>					geometryCubeNormalsVertex		[12]						= 
	{ { {0.0f, 0.0f, -1.0f}	, {0.0f, 0.0f, -1.0f}	, {0.0f, 0.0f, -1.0f}	} // Right	- first			
	, { {0.0f, 0.0f, -1.0f}	, {0.0f, 0.0f, -1.0f}	, {0.0f, 0.0f, -1.0f}	} // Right	- second		

	, { {-1.0f, 0.0f, 0.0f}	, {-1.0f, 0.0f, 0.0f}	, {-1.0f, 0.0f, 0.0f}	} // Back	- first			
	, { {-1.0f, 0.0f, 0.0f}	, {-1.0f, 0.0f, 0.0f}	, {-1.0f, 0.0f, 0.0f}	} // Back	- second		

	, { {0.0f, -1.0f, 0.0f}	, {0.0f, -1.0f, 0.0f}	, {0.0f, -1.0f, 0.0f}	} // Bottom	- first			
	, { {0.0f, -1.0f, 0.0f}	, {0.0f, -1.0f, 0.0f}	, {0.0f, -1.0f, 0.0f}	} // Bottom	- second		

	, { {0.0f, 0.0f, 1.0f}	, {0.0f, 0.0f, 1.0f}	, {0.0f, 0.0f, 1.0f}	} // Left	- first
	, { {0.0f, 0.0f, 1.0f}	, {0.0f, 0.0f, 1.0f}	, {0.0f, 0.0f, 1.0f}	} // Left	- second

	, { {1.0f, 0.0f, 0.0f}	, {1.0f, 0.0f, 0.0f}	, {1.0f, 0.0f, 0.0f}	} // Front	- first
	, { {1.0f, 0.0f, 0.0f}	, {1.0f, 0.0f, 0.0f}	, {1.0f, 0.0f, 0.0f}	} // Front	- second

	, { {0.0f, 1.0f, 0.0f}	, {0.0f, 1.0f, 0.0f}	, {0.0f, 1.0f, 0.0f}	} // Top	- first
	, { {0.0f, 1.0f, 0.0f}	, {0.0f, 1.0f, 0.0f}	, {0.0f, 1.0f, 0.0f}	} // Top	- second
	};

// Vertex coordinates for cube faces
static constexpr	const ::llc::STriangle2D<float>					geometryCubeUV					[12]						= 
	{ {{1.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 1.0f}}	// Right	- first			?? I have no idea if this is correct lol
	, {{1.0f, 0.0f}, {0.0f, 1.0f}, {1.0f, 1.0f}}	// Right	- second	

	, {{0.0f, 0.0f}, {1.0f, 1.0f}, {1.0f, 0.0f}}	// Back		- first		
	, {{0.0f, 0.0f}, {0.0f, 1.0f}, {1.0f, 1.0f}}	// Back		- second	

	, {{0.0f, 0.0f}, {1.0f, 1.0f}, {1.0f, 0.0f}}	// Bottom	- first		
	, {{0.0f, 0.0f}, {0.0f, 1.0f}, {1.0f, 1.0f}}	// Bottom	- second	

	, {{0.0f, 0.0f}, {1.0f, 1.0f}, {1.0f, 0.0f}}	// Left		- first
	, {{0.0f, 0.0f}, {0.0f, 1.0f}, {1.0f, 1.0f}}	// Left		- second

	, {{1.0f, 0.0f}, {0.0f, 1.0f}, {1.0f, 1.0f}}	// Front	- first
	, {{1.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 1.0f}}	// Front	- second

	, {{0.0f, 0.0f}, {1.0f, 1.0f}, {1.0f, 0.0f}}	// Top		- first
	, {{0.0f, 0.0f}, {0.0f, 1.0f}, {1.0f, 1.0f}}	// Top		- second
	};

					::llc::error_t									llc::generateCubePositions							(::llc::array_pod<::llc::STriangle3D<float>>& out_Positions	) { out_Positions	= geometryCubePositions			; return 0; }
					::llc::error_t									llc::generateCubeNormalsTriangle					(::llc::array_pod<::llc::SCoord3	<float>>& out_Normals	) { out_Normals		= geometryCubeNormalsTriangle	; return 0; }
					::llc::error_t									llc::generateCubeNormalsVertex						(::llc::array_pod<::llc::STriangle3D<float>>& out_Normals	) { out_Normals		= geometryCubeNormalsVertex		; return 0; }
					::llc::error_t									llc::generateCubeUV									(::llc::array_pod<::llc::STriangle2D<float>>& out_UV		) { out_UV			= geometryCubeUV				; return 0; }
					::llc::error_t									llc::generateCubeGeometry					
	( ::llc::array_pod<::llc::STriangle3D	<float>> & out_Positions	
	, ::llc::array_pod<::llc::SCoord3		<float>> & out_Normals
	, ::llc::array_pod<::llc::STriangle3D	<float>> & out_NormalsVertex
	, ::llc::array_pod<::llc::STriangle2D	<float>> & out_UV
	)
{ 
	llc_necall(::llc::generateCubePositions			(out_Positions		), "Some nasty problem caused this to fail.");
	llc_necall(::llc::generateCubeNormalsTriangle	(out_Normals		), "Some nasty problem caused this to fail.");
	llc_necall(::llc::generateCubeNormalsVertex		(out_NormalsVertex	), "Some nasty problem caused this to fail.");
	llc_necall(::llc::generateCubeUV				(out_UV				), "Some nasty problem caused this to fail.");
	return 0; 
}

					
					::llc::error_t									llc::generateGridPositions						(const ::llc::SCoord2<uint32_t> & gridMetrics, ::llc::array_pod<::llc::STriangle3D	<float>>& out_Positions	) { 
	for(uint32_t z = 0; z < gridMetrics.y; ++z) 
		for(uint32_t x = 0; x < gridMetrics.x; ++x) { 
			out_Positions.push_back({{(float)x, 0, (float)z}, {(float)x		, 0, (float)z + 1}, {(float)x + 1, 0, (float)z + 1}}); 
			out_Positions.push_back({{(float)x, 0, (float)z}, {(float)x + 1	, 0, (float)z + 1}, {(float)x + 1, 0, (float)z}}); 
		}
	return 0; 
}
					::llc::error_t									llc::generateGridNormalsTriangle				(const ::llc::SCoord2<uint32_t> & gridMetrics, ::llc::array_pod<::llc::SCoord3		<float>>& out_Normals	) { 
	for(uint32_t z = 0; z < gridMetrics.y; ++z) 
		for(uint32_t x = 0; x < gridMetrics.x; ++x) { 
			out_Normals.push_back({0, 1, 0}); 
			out_Normals.push_back({0, 1, 0}); 
		} 
	return 0; 
}
					::llc::error_t									llc::generateGridNormalsVertex					(const ::llc::SCoord2<uint32_t> & gridMetrics, ::llc::array_pod<::llc::STriangle3D	<float>>& out_Normals	) { 
	for(uint32_t z = 0; z < gridMetrics.y; ++z) 
		for(uint32_t x = 0; x < gridMetrics.x; ++x) { 
			out_Normals.push_back({{0, 1, 0}, {0, 1, 0}, {0, 1, 0}}); 
			out_Normals.push_back({{0, 1, 0}, {0, 1, 0}, {0, 1, 0}}); 
		} 
	return 0; 
}
					::llc::error_t									llc::generateGridUV								(const ::llc::SCoord2<uint32_t> & gridMetrics, ::llc::array_pod<::llc::STriangle2D	<float>>& out_UV		) { 
	const ::llc::SCoord2<double>											gridUnit										= {1.0 / gridMetrics.x, 1.0 / gridMetrics.y};
	const ::llc::SCoord2<double>											gridMetricsF									= gridMetrics.Cast<double>();
	for(uint32_t z = 0; z < gridMetrics.y; ++z) 
		for(uint32_t x = 0; x < gridMetrics.x; ++x) { 
			const ::llc::SCoord2<double>											gridCell										= {x / gridMetricsF.x, z / gridMetricsF.y};
			const ::llc::SCoord2<double>											gridCellFar										= gridCell + gridUnit;
			out_UV.push_back({{(float)gridCell.x, (float)gridCell.y}, {(float)gridCell.x		, (float)gridCellFar.y}	, gridCellFar.Cast<float>()					}); 
			out_UV.push_back({{(float)gridCell.x, (float)gridCell.y}, gridCellFar.Cast<float>()							, {(float)gridCellFar.x, (float)gridCell.y}	}); 
		}
	return 0; 
}
					::llc::error_t									llc::generateGridGeometry					
	( const ::llc::SCoord2<uint32_t>					& gridMetrics
	, ::llc::array_pod<::llc::STriangle3D	<float>>	& out_Positions	
	, ::llc::array_pod<::llc::SCoord3		<float>>	& out_Normals
	, ::llc::array_pod<::llc::STriangle3D	<float>>	& out_NormalsVertex
	, ::llc::array_pod<::llc::STriangle2D	<float>>	& out_UV
	)
{ 
	llc_necall(::llc::generateGridPositions			(gridMetrics, out_Positions		), "Some nasty problem caused this to fail.");
	llc_necall(::llc::generateGridNormalsTriangle	(gridMetrics, out_Normals		), "Some nasty problem caused this to fail.");
	llc_necall(::llc::generateGridNormalsVertex		(gridMetrics, out_NormalsVertex	), "Some nasty problem caused this to fail.");
	llc_necall(::llc::generateGridUV				(gridMetrics, out_UV			), "Some nasty problem caused this to fail.");
	return 0; 
}