#include "llc_array.h"
#include "llc_coord.h"

#ifndef LLC_GEOMETRY_H
#define LLC_GEOMETRY_H

namespace llc 
{
	struct SModelGeometry {
					::llc::array_pod<::llc::STriangle3D	<float>>		Positions									;
					::llc::array_pod<::llc::SCoord3		<float>>		Normals										;
					::llc::array_pod<::llc::STriangle2D	<float>>		UVs											;
	};

	struct SModelPivot {
					::llc::SCoord3		<float>							Scale										;
					::llc::SQuaternion	<float>							Orientation									;
					::llc::SCoord3		<float>							Position									;
	};

	// --- Geometry generation: Cube.
				::llc::error_t										generateCubePositions					(::llc::array_pod<::llc::STriangle3D<float>>& out_Positions	);
				::llc::error_t										generateCubeNormals						(::llc::array_pod<::llc::SCoord3	<float>>& out_Normals	);
				::llc::error_t										generateCubeUV							(::llc::array_pod<::llc::STriangle2D<float>>& out_UV		);
				::llc::error_t										generateCubeGeometry					(::llc::array_pod<::llc::STriangle3D<float>>& out_Positions	, ::llc::array_pod<::llc::SCoord3<float>>& out_Normals, ::llc::array_pod<::llc::STriangle2D<float>>& out_UV);
}

#endif // LLC_GEOMETRY_H
