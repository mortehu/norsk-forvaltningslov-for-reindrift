#ifndef _TGA_H_
#define _TGA_H_

#include <common.h>
#include <image.h>

namespace tga
{
	class image: public cls::image
	{
	private:
		void put_data(size_t p_Offset, uint8_t* p_Color, size_t p_Count = 1);

	public:
		image(const char* p_FileName);
	};
};

#endif

