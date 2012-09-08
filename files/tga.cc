// Original version: Copyright (C) 1999-2001 Lev Povalahev
//
// Modified to fit Celsius by Morten Hustveit, 2002

#include <iostream>
#include <stdexcept>
#include <stdlib.h>
#include <stdio.h>

#include <common.h>
#include <files/swap.h>
#include <files/tga.h>

namespace tga
{
	void image::put_data(size_t p_Offset, uint8_t* p_Color, size_t p_Count)
	{
		while(p_Count--)
		{
			for(size_t i = 0; i < get_bytes_per_pixel(); i++)
			{
#if __BYTE_ORDER == __BIG_ENDIAN
				*(get_data() + p_Offset++) = p_Color[get_bytes_per_pixel() - i - 1];
#else
				*(get_data() + p_Offset++) = p_Color[i];
#endif
			}
		}
	}

	image::image(const char* p_FileName)
	{
  		FILE* l_File = fopen(p_FileName, "rb");

		if(!l_File)
			throw std::runtime_error(cls::system_error());

		bool l_RLE = false;

		uint8_t l_IDLength;
		uint8_t l_IDColorMapType;
		uint8_t l_IDImageType;

		fread(&l_IDLength, sizeof(l_IDLength), 1, l_File);
		fread(&l_IDColorMapType, sizeof(l_IDColorMapType), 1, l_File);

		if(l_IDColorMapType == 1)
			throw std::runtime_error("Unsupported pixel format");

  		fread(&l_IDImageType, sizeof(l_IDImageType), 1, l_File);

		switch(l_IDImageType)
		{
		case 2:

        	break;

		case 10:

			l_RLE = true;

        	break;

		default:

			throw std::runtime_error("Unsupported image type");
		}

		// *** Skip color map data and X- and Y-origin data

		fseek(l_File, 9, SEEK_CUR);

		// *** Read image geometry

		uint16_t l_Width, l_Height;

		fread(&l_Width, sizeof(l_Width), 1, l_File);
		fread(&l_Height, sizeof(l_Height), 1, l_File);

		byte_swap(l_Width);
		byte_swap(l_Height);

		set_geometry(l_Width, l_Height);

		// *** Read pixel format

		uint8_t l_PixelFormat;

		fread(&l_PixelFormat, sizeof(l_PixelFormat), 1, l_File);

		switch(l_PixelFormat)
		{
		case 24:

			set_pixel_format(RGB);

			break;

		case 32:

			set_pixel_format(RGBA);

			break;

		default:

			throw std::runtime_error("Unsupported pixel format");
		}

		// *** Skip (redundant?) alpha information

		fseek(l_File, 1, SEEK_CUR);

		fseek(l_File, l_IDLength, 1);

		set_data(new uint8_t[get_width() * get_height() * get_bytes_per_pixel()]);

		if(!l_RLE)
		{
			fread(get_data(), get_bytes_per_pixel(), get_width() * get_height(), l_File);
		}
		else
		{
			size_t   l_CurrentOffset = 0;
			uint8_t* l_Color = new uint8_t[get_bytes_per_pixel()];
			uint8_t  l_Buf1, l_RunLength;

			while(l_CurrentOffset < get_width() * get_height() * get_bytes_per_pixel())
			{
				fread(&l_Buf1, sizeof(l_Buf1), 1, l_File);

				l_RunLength = (l_Buf1 & 0x7F) + 1;

				if(l_Buf1 & 0x80)
				{
					fread(l_Color, get_bytes_per_pixel(), 1, l_File);
					
					put_data(l_CurrentOffset, l_Color, l_RunLength);

					l_CurrentOffset += l_RunLength * get_bytes_per_pixel();
				}
				else
				{
					while(l_RunLength--)
					{
						fread(l_Color, get_bytes_per_pixel(), 1, l_File);

						put_data(l_CurrentOffset, l_Color);

						l_CurrentOffset += get_bytes_per_pixel();
					}
				}
			}

			delete [] l_Color;
		}

		fclose(l_File);
	}
}

