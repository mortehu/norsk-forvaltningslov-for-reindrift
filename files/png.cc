#include <stdexcept>
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <png.h>

#include <common.h>
#include <files/png.h>

namespace png
{
	static const size_t s_HeaderSize = 8;

	image::image(const char* p_FileName)
	{
		FILE* l_File = fopen(p_FileName, "rb");

		if(!l_File)
			throw std::runtime_error(cls::system_error());

		unsigned char l_Header[s_HeaderSize];

		fread(l_Header, 1, s_HeaderSize, l_File);

		if(png_sig_cmp(l_Header, 0, s_HeaderSize))
		{
			fclose(l_File);

			throw std::runtime_error("Not a PNG file");
		}

		png_structp l_PngPointer = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);

		if(!l_PngPointer)
		{
			fclose(l_File);

			throw std::runtime_error("Out of memory"); // ?
		}

		png_infop l_PngInfo = png_create_info_struct(l_PngPointer);

		if(!l_PngInfo)
		{
			png_destroy_read_struct(&l_PngPointer, (png_infopp) NULL, (png_infopp) NULL);

			fclose(l_File);

			throw std::runtime_error("Out of memory"); // ?
		}

		png_infop l_PngEndInfo = png_create_info_struct(l_PngPointer);

		if(!l_PngEndInfo)
		{
			png_destroy_read_struct(&l_PngPointer, (png_infopp) NULL, (png_infopp) NULL);

			fclose(l_File);

			throw std::runtime_error("Out of memory"); // ?
		}

		if(setjmp(png_jmpbuf(l_PngPointer))) // XXX: is this bad?
		{
			png_destroy_read_struct(&l_PngPointer, (png_infopp) NULL, (png_infopp) NULL);

			fclose(l_File);

			throw std::runtime_error("Unkonwn error"); // XXX
		}

		png_init_io(l_PngPointer, l_File);

		png_set_sig_bytes(l_PngPointer, s_HeaderSize);

		png_read_png(l_PngPointer, l_PngInfo, 0, NULL);

		if((l_PngInfo->bit_depth == 8)
		&& (l_PngInfo->color_type == PNG_COLOR_TYPE_RGB))
			set_pixel_format(RGB);
		else if((l_PngInfo->bit_depth == 8)
		&& (l_PngInfo->color_type == PNG_COLOR_TYPE_RGB_ALPHA))
			set_pixel_format(RGBA);
		else
		{
			png_destroy_read_struct(&l_PngPointer, (png_infopp) NULL, (png_infopp) NULL);

			fclose(l_File);

			throw std::runtime_error("Invalid pixel format");
		}

		set_geometry(l_PngInfo->width, l_PngInfo->height);

		uint8_t* l_Data = new uint8_t[l_PngInfo->height * l_PngInfo->rowbytes];

		for(size_t i_Row = 0; i_Row < l_PngInfo->height; i_Row++)
		{
			memcpy(
				&l_Data[i_Row * l_PngInfo->rowbytes],
				l_PngInfo->row_pointers[i_Row],
				l_PngInfo->rowbytes);
		}

		png_destroy_read_struct(&l_PngPointer, &l_PngInfo, &l_PngEndInfo);

		fclose(l_File);

		set_data(l_Data);
	}
};

