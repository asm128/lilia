#include "llc_array.h"
#include "llc_matrix.h"


#ifndef LLC_GEOMETRY_H
#define LLC_GEOMETRY_H

namespace llc 
{
	//template <typename _tUnit>
	//struct SGeometryGrid {
	//				::llc::SCoord3<_tUnit>								Normal										;
	//				::llc::array_pod<::llc::STriangle3D	<_tUnit>>		Positions									;
	//				::llc::array_pod<::llc::STriangle2D	<_tUnit>>		UVs											;
	//};

	//template <typename _tUnit>
	//struct SGeometryBox {
	//				::llc::array_pod<::llc::STriangle3D	<_tUnit>>		Positions									;
	//				::llc::array_pod<::llc::SCoord3		<_tUnit>>		Normals										;
	//				::llc::array_pod<::llc::STriangle2D	<_tUnit>>		UVs											;
	//};

	//template <typename _tUnit>
	//struct SGeometryPolyhedron {
	//				::llc::array_pod<::llc::STriangle3D	<_tUnit>>		Positions									;
	//				::llc::array_pod<::llc::SCoord3		<_tUnit>>		Normals										;
	//				::llc::array_pod<::llc::STriangle2D	<_tUnit>>		UVs											;
	//};

	template <typename _tUnit>
	struct SModelGeometry {
					::llc::array_pod<::llc::STriangle3D	<_tUnit>>		Positions									;
					::llc::array_pod<::llc::SCoord3		<_tUnit>>		Normals										;
					::llc::array_pod<::llc::STriangle2D	<_tUnit>>		UVs											;
	};

	template <typename _tUnit>
	struct SModelPivot {
					::llc::SCoord3		<_tUnit>						Scale										;
					::llc::SQuaternion	<_tUnit>						Orientation									;
					::llc::SCoord3		<_tUnit>						Position									;
	};

	template <typename _tUnit>
	struct SModelTransform {
					::llc::SMatrix4		<_tUnit>						World										;
					::llc::SMatrix4		<_tUnit>						WorldInverse								;
	};

	template <typename _tUnit>
	struct SModelBase {
					::llc::SModelGeometry	<_tUnit>					Geometry									= {};
					::llc::SModelPivot		<_tUnit>					Pivot										= {};
	};

	// --- Geometry generation: Cube.
				::llc::error_t										generateCubePositions					(::llc::array_pod<::llc::STriangle3D<float>>& out_Positions	);
				::llc::error_t										generateCubeNormals						(::llc::array_pod<::llc::SCoord3	<float>>& out_Normals	);
				::llc::error_t										generateCubeUV							(::llc::array_pod<::llc::STriangle2D<float>>& out_UV		);
				::llc::error_t										generateCubeGeometry					(::llc::array_pod<::llc::STriangle3D<float>>& out_Positions	, ::llc::array_pod<::llc::SCoord3<float>>& out_Normals, ::llc::array_pod<::llc::STriangle2D<float>>& out_UV);
}

#endif // LLC_GEOMETRY_H
