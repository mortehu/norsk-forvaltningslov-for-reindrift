#ifndef _JPEG_H_
#define _JPEG_H_

#include <image.h>

namespace jpeg
{
	class image: public cls::image
	{
	public:
		image(const char* p_FileName);
	};
};

#endif

