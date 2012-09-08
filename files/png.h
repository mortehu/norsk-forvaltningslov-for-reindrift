#ifndef _PNG_H_
#define _PNG_H_

#include <image.h>

namespace png
{
	class image: public cls::image
	{
	public:
		image(const char* p_FileName);
	};
};

#endif

