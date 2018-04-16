#include "llc_geometry.h"
#include "llc_color.h"
#include "llc_camera.h"

#ifndef LLC_SCENE_H_203492837
#define LLC_SCENE_H_203492837

namespace llc
{
	struct SColorsMaterial {
							::llc::array_pod<::llc::SColorBGRA>						Ambient										= {};
							::llc::array_pod<::llc::SColorBGRA>						Diffuse										= {};
							::llc::array_pod<::llc::SColorBGRA>						Specular									= {};
	};

	struct SLightsDirectional {
							::llc::array_pod<::llc::SCoord3<float>>					Directions									= {};
							::llc::SColorsMaterial									Colors										= {};
	};

	struct SLightsPoint {
							::llc::array_pod<::llc::SCoord3<float>>					Directions									= {};
							::llc::array_pod<::llc::SCoord3<float>>					Positions									= {};
							::llc::array_pod<::llc::SCoord3<float>>					Attenuation									= {};
							::llc::SColorsMaterial									Colors										= {};
	};

	struct SLightsSpot {
							::llc::array_pod<::llc::SCoord3<float>>					Directions									= {};
							::llc::array_pod<::llc::SCoord3<float>>					Positions									= {};
							::llc::array_pod<::llc::SCoord3<float>>					Attenuation									= {};
							::llc::array_pod<float>									SpotPower									= {};
							::llc::SColorsMaterial									Colors										= {};
	};

	struct SSceneGeometries {
							::llc::array_pod<::llc::SModelGeometry	<float>>		Polyhedron									= {};
							::llc::SModelGeometry					<float>			Box											= {};
							::llc::array_pod<::llc::SModelGeometry	<float>>		Grid										= {};
							::llc::array_pod<::llc::SModelGeometry	<float>>		Models										= {};
	};

	struct SSceneLights	{
							::llc::SLightsDirectional								LightsDirectional							= {};
							::llc::SLightsPoint										LightsPoint									= {};
							::llc::SLightsSpot										LightsSpot									= {};
	};

	struct SSceneCamera {
							::llc::SCameraPoints									Points								;
							::llc::SCameraRange										Range								;
							::llc::SCameraVectors									Vectors								;
	};

	enum SCENE_OBJECT_TYPE 
		{ SCENE_OBJECT_TYPE_BOX
		, SCENE_OBJECT_TYPE_GRID
		, SCENE_OBJECT_TYPE_POLYHEDRON
		, SCENE_OBJECT_TYPE_MODEL
		};

	struct SSceneObject {
							::llc::SCENE_OBJECT_TYPE								Type;
							int32_t													IndexTransform;
							int32_t													IndexGeometry;
	};

	struct SSceneTransforms {
							::llc::SMatrix4<float>									Viewport									= {};
							::llc::SMatrix4<float>									ViewportInverse								= {};
							::llc::SMatrix4<float>									ViewportInverseCentered						= {};
							::llc::SMatrix4<float>									FieldOfView									= {};
							::llc::SMatrix4<float>									FinalProjection								= {};
							::llc::SMatrix4<float>									FinalProjectionInverse						= {};
							::llc::SMatrix4<float>									View										= {};
	};

	struct SScene {
							::llc::array_pod<::llc::SSceneObject>					Objects										= {};
							::llc::SSceneGeometries									Geometries									= {};
							::llc::SSceneLights										Lights										= {};
							::llc::SSceneTransforms									Transforms									= {};

							::llc::array_pod<::llc::SModelPivot	<float>>			ObjectPivots								= {};
							::llc::array_pod<::llc::SMatrix4	<float>>			ObjectTransform								= {};
							::llc::array_pod<::llc::SMatrix4	<float>>			ObjectOrientations							= {};
							::llc::array_pod<::llc::SMatrix4	<float>>			ObjectOrientationsInverse					= {};

							::llc::SSceneCamera										Camera										= 
								{ ::llc::SCameraPoints{{20, 2, 0}, {}}	
								, ::llc::SCameraRange
									{ 0.01
									, 40.0
									, .25
									}
								, ::llc::SCameraVectors	
									{ {1, 0, 0}
									, {0, 1, 0}
									, {0, 0, 1}
									}
								};
	};

	static inline		::llc::error_t											sceneBoxAdd									(SScene& scene)	{
			const uint32_t																newIndex									= scene.ObjectPivots.size();
		if(errored(::llc::resize
			( newIndex + 1
			, scene.ObjectPivots			
			, scene.ObjectTransform		
			, scene.ObjectOrientations		
			, scene.ObjectOrientationsInverse
			)))
		{
			::llc::resize
				( newIndex + 1
				, scene.ObjectPivots			
				, scene.ObjectTransform		
				, scene.ObjectOrientations		
				, scene.ObjectOrientationsInverse		
				);
			return -1;
		}
		return newIndex;
	}

} // namespace

#endif // LLC_SCENE_H_203492837
