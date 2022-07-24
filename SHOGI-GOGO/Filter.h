#pragma once

#ifndef __FILTER_H_INCLUDED__
#define __FILTER_H_INCLUDED__

#include "MyHead.h"

namespace irr {
	namespace utils {
		ITexture* filter(IVideoDriver* drv, ITexture* texture, s32 materialType);
		ITexture* scaling(IVideoDriver* drv, ITexture* texture, dimension2du size);
		ITexture* blend(IVideoDriver* drv, ITexture* foundation, ITexture* ingredient, vector2di pos);
		ITexture* singleColor(IVideoDriver* drv, SColor color, dimension2du size);
	}
}

#endif // !__FILTER_H_INCLUDED__