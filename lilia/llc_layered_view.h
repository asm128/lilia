#include "llc_grid_view.h"

#ifndef LLC_LAYERED_VIEW_H_0923782
#define LLC_LAYERED_VIEW_H_0923782

namespace llc
{
#pragma pack(push, 1)
	template <typename _tElement>
	class layered_view {
	protected:
		// Properties / Member Variables
							_tElement									* Data						= 0;
							::llc::SCoord3<uint32_t>					Size						= {0, 0, 0};
							uint32_t									LayerSize					= 0;
	public:
		// Constructors
		inline constexpr												layered_view				()																				noexcept	= default;
		inline															layered_view				(_tElement* dataElements, uint32_t layerWidth, uint32_t layerHeight, uint32_t layerCount)	: Data{dataElements}, Size{layerWidth, layerHeight, layerCount}, LinearSize(layerWidth * layerHeight) {
			throw_if(0 == dataElements && 0 != Size.x && 0 != Size.y && 0 != Size.z, ::std::exception(""), "Invalid parameters.");	// Crash if we received invalid parameters in order to prevent further malfunctioning.
		}

		// Operators
							::llc::grid_view<_tElement>					operator[]					(uint32_t layer)																			{ throw_if(0 == Data, ::std::exception(""), "Uninitialized array pointer."); throw_if(layer >= Size.z, ::std::exception(""), "Invalid row."); return ::llc::grid_view<_tElement			>(&Data[layer * LayerSize], Size.x, Size.y); }
							::llc::grid_view<const _tElement>			operator[]					(uint32_t layer)														const				{ throw_if(0 == Data, ::std::exception(""), "Uninitialized array pointer."); throw_if(layer >= Size.z, ::std::exception(""), "Invalid row."); return ::llc::grid_view<const _tElement	>(&Data[layer * LayerSize], Size.x, Size.y); }

		// Methods
		inline constexpr	const _tElement*							begin						()																		const	noexcept	{ return Data;						}
		inline constexpr	const _tElement*							end							()																		const	noexcept	{ return Data + size();				}

		inline constexpr	_tElement*									begin						()																				noexcept	{ return Data;						}
		inline constexpr	_tElement*									end							()																				noexcept	{ return Data + size();				}

		inline constexpr	::llc::SCoord3<uint32_t>					metrics						()																		const	noexcept	{ return {Size.x, Size.y, Size.z};	}
		inline constexpr	uint32_t									size						()																		const	noexcept	{ return Size.x * Size.y * Size.z;	}
		inline constexpr	uint32_t									layer_width					()																		const	noexcept	{ return Size.x;					}
		inline constexpr	uint32_t									layer_height				()																		const	noexcept	{ return Size.y;					}
		inline constexpr	uint32_t									layer_count					()																		const	noexcept	{ return Size.z;					}
	};
#pragma pack(pop)

	// grid_view common typedefs
	typedef				::llc::layered_view<char_t				>	view3d_char				;
	typedef				::llc::layered_view<ubyte_t				>	view3d_ubyte			;
	typedef				::llc::layered_view<byte_t				>	view3d_byte				;
	typedef				::llc::layered_view<float				>	view3d_float32			;
	typedef				::llc::layered_view<double				>	view3d_float64			;
	typedef				::llc::layered_view<uint8_t				>	view3d_uint8			;
	typedef				::llc::layered_view<uint16_t			>	view3d_uint16			;
	typedef				::llc::layered_view<uint32_t			>	view3d_uint32			;
	typedef				::llc::layered_view<uint64_t			>	view3d_uint64			;
	typedef				::llc::layered_view<int8_t				>	view3d_int8				;
	typedef				::llc::layered_view<int16_t				>	view3d_int16			;
	typedef				::llc::layered_view<int32_t				>	view3d_int32			;
	typedef				::llc::layered_view<int64_t				>	view3d_int64			;

	// grid_view<const> common typedefs
	typedef				::llc::layered_view<const char_t		>	view3d_const_char		;
	typedef				::llc::layered_view<const ubyte_t		>	view3d_const_ubyte		;
	typedef				::llc::layered_view<const byte_t		>	view3d_const_byte		;
	typedef				::llc::layered_view<const float			>	view3d_const_float32	;
	typedef				::llc::layered_view<const double		>	view3d_const_float64	;
	typedef				::llc::layered_view<const uint8_t		>	view3d_const_uint8		;
	typedef				::llc::layered_view<const uint16_t		>	view3d_const_uint16		;
	typedef				::llc::layered_view<const uint32_t		>	view3d_const_uint32		;
	typedef				::llc::layered_view<const uint64_t		>	view3d_const_uint64		;
	typedef				::llc::layered_view<const int8_t		>	view3d_const_int8		;
	typedef				::llc::layered_view<const int16_t		>	view3d_const_int16		;
	typedef				::llc::layered_view<const int32_t		>	view3d_const_int32		;
	typedef				::llc::layered_view<const int64_t		>	view3d_const_int64		;

}

#endif // LLC_LAYERED_VIEW_H_0923782
