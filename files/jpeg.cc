#include <stdexcept>
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
extern "C"
{
	#include <jpeglib.h>
}

#include <common.h>
#include <files/jpeg.h>


namespace jpeg
{
	image::image(const char* p_FileName)
	{
		FILE* l_File = fopen(p_FileName, "rb");

		if(!l_File)
			throw std::runtime_error(cls::system_error());

		struct jpeg_error_mgr l_ErrorManager;
		struct jpeg_decompress_struct l_DecompressInfo;

		l_DecompressInfo.err = jpeg_std_error(&l_ErrorManager);

		jpeg_create_decompress(&l_DecompressInfo);

		jpeg_stdio_src(&l_DecompressInfo, l_File);

		jpeg_read_header(&l_DecompressInfo, TRUE);

		jpeg_start_decompress(&l_DecompressInfo);

		if(l_DecompressInfo.output_components == 3)
			set_pixel_format(RGB);
		else if(l_DecompressInfo.output_components == 4)
			set_pixel_format(RGBA);
		else
			throw std::runtime_error("Unsupported pixel format");

		set_geometry(l_DecompressInfo.output_width, l_DecompressInfo.output_height);

		set_data(new uint8_t[get_width() * get_height() * get_bytes_per_pixel()]);

		size_t l_Pitch = l_DecompressInfo.output_width
		               * l_DecompressInfo.output_components;

		while(l_DecompressInfo.output_scanline < l_DecompressInfo.output_height)
		{
			uint8_t* l_Ptr = get_data() + l_DecompressInfo.output_scanline * l_Pitch;

			jpeg_read_scanlines(&l_DecompressInfo, (JSAMPLE**) &l_Ptr, 1);
		}

		jpeg_finish_decompress(&l_DecompressInfo);
	}
};
