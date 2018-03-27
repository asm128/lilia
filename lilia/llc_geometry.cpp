#include "llc_geometry.h"

// Vertex coordinates for cube faces
static constexpr const ::llc::STriangle3D<float>					geometryCubePositions	[12]						= 
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

static constexpr	const ::llc::SCoord3<float>						geometryCubeNormals		[12]						= 
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

// Vertex coordinates for cube faces
static constexpr	const ::llc::STriangle2D<float>					geometryCubeUV			[12]						= 
	{ {{1.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 1.0f}}	// Right	- first			?? I have no idea if this is correct lol
	, {{1.0f, 0.0f}, {0.0f, 1.0f}, {1.0f, 1.0f}}	// Right	- second	

	, {{1.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 1.0f}}	// Back		- first		
	, {{1.0f, 0.0f}, {0.0f, 1.0f}, {1.0f, 1.0f}}	// Back		- second	

	, {{1.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 1.0f}}	// Bottom	- first		
	, {{1.0f, 0.0f}, {0.0f, 1.0f}, {1.0f, 1.0f}}	// Bottom	- second	

	, {{1.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 1.0f}}	// Left		- first
	, {{1.0f, 0.0f}, {0.0f, 1.0f}, {1.0f, 1.0f}}	// Left		- second

	, {{1.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 1.0f}}	// Front	- first
	, {{1.0f, 0.0f}, {0.0f, 1.0f}, {1.0f, 1.0f}}	// Front	- second

	, {{1.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 1.0f}}	// Top		- first
	, {{1.0f, 0.0f}, {0.0f, 1.0f}, {1.0f, 1.0f}}	// Top		- second
	};

					::llc::error_t					llc::generateCubePositions					(::llc::array_pod<::llc::STriangle3D<float>>& out_Positions	) { out_Positions	= geometryCubePositions	; return 0; }
					::llc::error_t					llc::generateCubeNormals					(::llc::array_pod<::llc::SCoord3	<float>>& out_Normals	) { out_Normals		= geometryCubeNormals	; return 0; }
					::llc::error_t					llc::generateCubeUV							(::llc::array_pod<::llc::STriangle2D<float>>& out_UV		) { out_UV			= geometryCubeUV		; return 0; }
					::llc::error_t					llc::generateCubeGeometry					
	( ::llc::array_pod<::llc::STriangle3D	<float>> & out_Positions	
	, ::llc::array_pod<::llc::SCoord3		<float>> & out_Normals
	, ::llc::array_pod<::llc::STriangle2D	<float>> & out_UV
	)
{ 
	llc_necall(generateCubePositions	(out_Positions	), "Some nasty problem caused this to fail.");
	llc_necall(generateCubeNormals		(out_Normals	), "Some nasty problem caused this to fail.");
	llc_necall(generateCubeUV			(out_UV			), "Some nasty problem caused this to fail.");
	return 0; 
}
