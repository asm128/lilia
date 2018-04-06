#include "llc_coord.h"

#ifndef LLC_CAMERA_H_20934702938492
#define LLC_CAMERA_H_20934702938492

namespace llc
{
	struct SCameraPoints {
							::llc::SCoord3<float>							Position	;
							::llc::SCoord3<float>							Target		;
	};

	struct SCameraVectors {
							::llc::SCoord3<float>							Front		;
							::llc::SCoord3<float>							Up			;
							::llc::SCoord3<float>							Right		;
	};

	struct SCameraRange {
							double											Near		;
							double											Far			;
							double											Angle		;
	};
} // namespace

#endif // LLC_CAMERA_H_20934702938492
