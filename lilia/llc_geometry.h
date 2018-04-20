#include "llc_array.h"
#include "llc_matrix.h"


#ifndef LLC_GEOMETRY_H
#define LLC_GEOMETRY_H

namespace llc 
{
	template <typename _tUnit>
	struct SModelGeometry {
					::llc::array_pod<::llc::STriangle3D	<_tUnit>>		Positions									;
					::llc::array_pod<::llc::SCoord3		<_tUnit>>		NormalsTriangle								;
					::llc::array_pod<::llc::STriangle3D	<_tUnit>>		NormalsVertex								;
					::llc::array_pod<::llc::STriangle2D	<_tUnit>>		UVs											;
	};

	template <typename _tUnit>
	struct SModelGeometryIndexed {
					::llc::array_pod<int32_t>							IndicesPositions							;
					::llc::array_pod<::llc::STriangle3D	<_tUnit>>		Positions									;
					::llc::array_pod<int32_t>							IndicesNormalsTriangle						;
					::llc::array_pod<::llc::SCoord3		<_tUnit>>		NormalsTriangle								;
					::llc::array_pod<int32_t>							IndicesNormalsVertex						;
					::llc::array_pod<::llc::STriangle3D	<_tUnit>>		NormalsVertex								;
					::llc::array_pod<int32_t>							IndicesNormalsUVs							;
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
				::llc::error_t										generateCubeNormalsTriangle				(::llc::array_pod<::llc::SCoord3	<float>>& out_Normals	);
				::llc::error_t										generateCubeNormalsVertex				(::llc::array_pod<::llc::STriangle3D<float>>& out_Normals	);
				::llc::error_t										generateCubeUV							(::llc::array_pod<::llc::STriangle2D<float>>& out_UV		);
				::llc::error_t										generateCubeGeometry					
					( ::llc::array_pod<::llc::STriangle3D	<float>> & out_Positions	
					, ::llc::array_pod<::llc::SCoord3		<float>> & out_Normals
					, ::llc::array_pod<::llc::STriangle3D	<float>> & out_NormalsVertex
					, ::llc::array_pod<::llc::STriangle2D	<float>> & out_UV
					);
				static inline ::llc::error_t						generateCubeGeometry					(::llc::SModelGeometry<float>& out_Geometry)	{
					return ::llc::generateCubeGeometry
						( out_Geometry.Positions
						, out_Geometry.NormalsTriangle
						, out_Geometry.NormalsVertex
						, out_Geometry.UVs
						);
				}

				::llc::error_t										generateGridPositions					(const ::llc::SCoord2<uint32_t> & gridMetrics, ::llc::array_pod<::llc::STriangle3D	<float>>& out_Positions	);
				::llc::error_t										generateGridNormalsTriangle				(const ::llc::SCoord2<uint32_t> & gridMetrics, ::llc::array_pod<::llc::SCoord3		<float>>& out_Normals	);
				::llc::error_t										generateGridNormalsVertex				(const ::llc::SCoord2<uint32_t> & gridMetrics, ::llc::array_pod<::llc::STriangle3D	<float>>& out_Normals	);
				::llc::error_t										generateGridUV							(const ::llc::SCoord2<uint32_t> & gridMetrics, ::llc::array_pod<::llc::STriangle2D	<float>>& out_UV		);
				::llc::error_t										generateGridGeometry					
					( const ::llc::SCoord2<uint32_t>					& gridMetrics
					, ::llc::array_pod<::llc::STriangle3D	<float>>	& out_Positions	
					, ::llc::array_pod<::llc::SCoord3		<float>>	& out_Normals
					, ::llc::array_pod<::llc::STriangle3D	<float>>	& out_NormalsVertex
					, ::llc::array_pod<::llc::STriangle2D	<float>>	& out_UV
					);
				static inline ::llc::error_t						generateGridGeometry					(const ::llc::SCoord2<uint32_t>	& gridMetrics, ::llc::SModelGeometry<float>& out_Geometry)	{
					return ::llc::generateGridGeometry
						( gridMetrics
						, out_Geometry.Positions
						, out_Geometry.NormalsTriangle
						, out_Geometry.NormalsVertex
						, out_Geometry.UVs
						);
				}
} // namespace

#endif // LLC_GEOMETRY_H
