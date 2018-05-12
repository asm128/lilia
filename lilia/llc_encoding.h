#include "llc_array.h"
#include <string>

#ifndef LLC_ENCODING_H_209873982374
#define LLC_ENCODING_H_209873982374

namespace llc
{
	::llc::error_t									base64Encode												(const ::llc::array_view<const ubyte_t	> & inputBinary	, ::llc::array_pod<char_t	> & out_base64	);
	::llc::error_t									base64Decode												(const ::llc::array_view<const char_t	> & in_base64	, ::llc::array_pod<ubyte_t	> & outputBinary);
}

#endif // LLC_ENCODING_H_209873982374
