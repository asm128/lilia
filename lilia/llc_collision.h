// Tip: Best viewed with zoom level at 81%.
// Tip: Hold Left ALT + SHIFT while tapping or holding the arrow keys in order to select multiple columns and write on them at once. 
//		Also useful for copy & paste operations in which you need to copy a bunch of variable or function names and you can't afford the time of copying them one by one.
#include "llc_coord.h"
#include "llc_log.h"

#ifndef LLC_COLLISION_H_209384
#define LLC_COLLISION_H_209384

namespace llc
{
// Calculate intersection of two lines. return 1 if found, 0 if not found or -1 on error
	template<typename _tCoord>
					::llc::error_t											line_line_intersect			
		( const ::llc::SLine2D<_tCoord>	& line1
		, const ::llc::SLine2D<_tCoord>	& line2
		, ::llc::SCoord2<_tCoord>		& out_intersect_point
		)
	{
		double																		detL1						= ::llc::determinant(line1);
		double																		detL2						= ::llc::determinant(line2);
	
		double																		x1mx2						= line1.A.x - line1.B.x;
		double																		y1my2						= line1.A.y - line1.B.y;
	
		double																		x3mx4						= line2.A.x - line2.B.x;
		double																		y3my4						= line2.A.y - line2.B.y;
	
		double																		xnom						= ::llc::determinant(detL1, x1mx2, detL2, x3mx4);
		double																		ynom						= ::llc::determinant(detL1, y1my2, detL2, y3my4);
		double																		denom						= ::llc::determinant(x1mx2, y1my2, x3mx4, y3my4);
		if(denom == 0.0) { // Lines don't seem to cross
			out_intersect_point.x																= NAN;
			out_intersect_point.y																= NAN;
			return 0;
		}
		out_intersect_point.x																= (_tCoord)(xnom / denom);	
		out_intersect_point.y																= (_tCoord)(ynom / denom);
		ree_if(!isfinite(out_intersect_point.x) 
			|| !isfinite(out_intersect_point.y)
			, "Probably a numerical issue.");
		return 1; //All OK
	}

	template<typename _tCoord>
					::llc::error_t											point_in_segment
		( const ::llc::SLine2D<_tCoord>	& segment
		, const ::llc::SCoord2<_tCoord>	& point
		)
	{
		return (	point.x >=	::llc::min(segment.A.x, segment.B.x)
				&&	point.y >=	::llc::min(segment.A.y, segment.B.y)
				&&	point.x <=	::llc::max(segment.A.x, segment.B.x)
				&&	point.y <=	::llc::max(segment.A.y, segment.B.y)
				) ? 1 :  0;
	}
	// Calculate intersection of two lines. return 1 if found, 0 if not found or -1 on error
	template<typename _tCoord>
					::llc::error_t											line_segment_intersect			
		( const ::llc::SLine2D<_tCoord>	& line
		, const ::llc::SLine2D<_tCoord>	& segment
		, ::llc::SCoord2<_tCoord>		& out_intersect_point
		)
	{
		::llc::error_t																collision					= line_line_intersect(line, segment, out_intersect_point);
		return (1 == collision) ? point_in_segment(segment, out_intersect_point) : collision;
	}

	// Calculate intersection of two lines. return 1 if found, 0 if not found or -1 on error
	template<typename _tCoord>
					::llc::error_t											segment_segment_intersect			
		( const ::llc::SLine2D<_tCoord>	& segment1
		, const ::llc::SLine2D<_tCoord>	& segment2
		, ::llc::SCoord2<_tCoord>		& out_intersect_point
		)
	{
		::llc::error_t																collision					= line_segment_intersect(segment1, segment2, out_intersect_point);
		return (1 == collision) ? point_in_segment(segment1, out_intersect_point) : collision;
	}


	template<typename _tCoord>
						::llc::error_t										buildAABBSegments		
		( const ::llc::SCoord2<_tCoord> & center
		, const _tCoord					halfSizeBox
		,		::llc::SLine2D<_tCoord>	& left
		,		::llc::SLine2D<_tCoord>	& top
		,		::llc::SLine2D<_tCoord>	& right
		,		::llc::SLine2D<_tCoord>	& bottom
		) 
	{
		typedef	::llc::SCoord2<_tCoord>												TCoord2;
		left																	= {center + TCoord2{-halfSizeBox, halfSizeBox}, center + TCoord2{-halfSizeBox,-halfSizeBox}};
		top																		= {center + TCoord2{ halfSizeBox, halfSizeBox}, center + TCoord2{-halfSizeBox, halfSizeBox}};
		right																	= {center + TCoord2{ halfSizeBox, halfSizeBox}, center + TCoord2{ halfSizeBox,-halfSizeBox}};
		bottom																	= {center + TCoord2{ halfSizeBox,-halfSizeBox}, center + TCoord2{-halfSizeBox,-halfSizeBox}};
		return 0;
	}

	template<typename _tCoord>
						::llc::error_t										buildAABBSegments		
		( const ::llc::SRectangle2D<_tCoord>	& rect
		,		::llc::SLine2D<_tCoord>			& left
		,		::llc::SLine2D<_tCoord>			& top
		,		::llc::SLine2D<_tCoord>			& right
		,		::llc::SLine2D<_tCoord>			& bottom
		) 
	{
		typedef	::llc::SCoord2<_tCoord>												TCoord2;
		left																	= {rect.Offset										, rect.Offset + TCoord2{0			, rect.Size.y}};
		top																		= {rect.Offset + TCoord2{0, rect.Size.y}			, rect.Offset + TCoord2{rect.Size.x	, rect.Size.y}};
		right																	= {rect.Offset + TCoord2{rect.Size.x, rect.Size.y}	, rect.Offset + TCoord2{rect.Size.x, 0}};
		bottom																	= {rect.Offset										, rect.Offset + TCoord2{rect.Size.x, 0}};
		return 0;
	}

}


#endif // LLC_COLLISION_H_209384
