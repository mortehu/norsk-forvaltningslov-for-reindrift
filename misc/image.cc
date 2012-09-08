#include <stdexcept>
#include <string>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>
#include <GL/gl.h>
#ifdef HAS_MAGICK
#include <Magick++.h>
#endif

#include <image.h>
#include <common.h>
#ifndef HAS_MAGICK
#include <files/jpeg.h>
#include <files/tga.h>
#include <files/png.h>
#endif

namespace cls
{
	void image::activate(size_t p_Pass)
	{
		glActiveTextureARB(GL_TEXTURE0_ARB + p_Pass);

		if(!m_GLHandle && get_data())
		{
			glGenTextures(1, &m_GLHandle);
			
			glBindTexture(GL_TEXTURE_2D, m_GLHandle);

			if(m_MipMap)
			{
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, 
					GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, 
					GL_LINEAR_MIPMAP_LINEAR);
			}
			else
			{
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, 
					GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, 
					GL_LINEAR);
			}

			glTexParameteri( 
				GL_TEXTURE_2D, 
				GL_TEXTURE_WRAP_S, 
				m_XRepeat ? GL_REPEAT : GL_CLAMP);

			glTexParameteri( 
				GL_TEXTURE_2D, 
				GL_TEXTURE_WRAP_T, 
				m_YRepeat ? GL_REPEAT : GL_CLAMP);

			glTexImage2D( 
				GL_TEXTURE_2D, 
				0, 
				get_bytes_per_pixel(), 
				get_width(), get_height(), 
				0, 
				  (get_pixel_format() == RGB)  ? GL_RGB 
				: (get_pixel_format() == RGBA) ? GL_RGBA 
				: (get_pixel_format() == GRAY) ? GL_LUMINANCE 
				: 0, 
				GL_UNSIGNED_BYTE, 
				get_data());

			if((get_width() > 1 || get_height() > 1) && m_MipMap)
			{
				size_t l_BPP = get_bytes_per_pixel();
				size_t l_MipLevel = 1;
				size_t l_Width = get_width();
				size_t l_Height = get_height();
				uint8_t* l_Data = new uint8_t[l_Width * l_Height * l_BPP / 4];

				while(l_Width > 1 || l_Height > 1)
				{
					size_t l_OldWidth = l_Width;

					size_t l_XScale = (l_Width > 1) ? 2 : 1;
					size_t l_YScale = (l_Height > 1) ? 2 : 1;

					l_Width /= l_XScale;
					l_Height /= l_YScale;

					const uint8_t* l_RefData = 
						  (l_MipLevel == 1) ? get_data() : l_Data;

					size_t l_Offset = 0;
					for(size_t l_Y = 0; l_Y < l_Height; l_Y++)
					{
						size_t l_OldY = l_Y * l_XScale;

						for(size_t l_X = 0; l_X < l_Width; l_X++)
						{
							size_t l_OldX = l_X * l_YScale;

							for(size_t l_C = 0; l_C < l_BPP; l_C++, l_Offset++)
							{
								l_Data[l_Offset] = 
									 (l_RefData[ 
										(l_OldY * l_OldWidth + l_OldX) 
										* l_BPP + l_C] 
									+ l_RefData[ 
										(l_OldY * l_OldWidth + (l_OldX + 1)) 
										* l_BPP + l_C] 
									+ l_RefData[ 
										((l_OldY + 1) * l_OldWidth + l_OldX) 
										* l_BPP + l_C] 
									+ l_RefData[ 
										((l_OldY + 1) * l_OldWidth + (l_OldX + 1)) 
										* l_BPP + l_C]) >> 2;
							}
						}
					}

					glTexImage2D(
						GL_TEXTURE_2D, 
						l_MipLevel, 
						l_BPP, 
						l_Width, l_Height, 
						0, 
						  (get_pixel_format() == RGB)  ? GL_RGB 
						: (get_pixel_format() == RGBA) ? GL_RGBA 
						: 0,
						GL_UNSIGNED_BYTE, 
						l_Data);

					l_MipLevel++;
				}

				delete [] l_Data;
			}
		}
		else
		{
			glBindTexture(GL_TEXTURE_2D, m_GLHandle);
		}
	}

	image& image::load(const char* p_FileName)
	{
		char* l_Path = strdup(p_FileName);
		char* l_FileName = strrchr(l_Path, '/');

		*l_FileName = '\0';

		l_FileName++;

		DIR* l_Directory = opendir(l_Path);

		if(!l_Directory)
		{
			free(l_Path);

			throw std::runtime_error(std::string("Directory inaccessible: ") + cls::system_error());
		}

		struct dirent* l_DirEntry = readdir(l_Directory);

		for(; l_DirEntry; l_DirEntry = readdir(l_Directory))
		{
			char* l_TestName = strdup(l_DirEntry->d_name);
			char* l_Ext = strrchr(l_TestName, '.');

			if((!l_Ext) || (l_Ext == l_TestName))
			{
				free(l_TestName);

				continue;
			}
			
			*l_Ext = '\0';
			l_Ext++;

			//if(strstr(l_FileName, l_TestName) == l_FileName)
			if(strcmp(l_FileName, l_TestName) == 0)
			{
#ifdef HAS_MAGICK
				Magick::Image l_Image(std::string(l_Path) + std::string("/") + std::string(l_DirEntry->d_name));

				const Magick::PixelPacket* l_PixelPacket = l_Image.getConstPixels(0, 0, l_Image.columns(), l_Image.rows());

				set_pixel_format(l_Image.matte() ? RGBA : RGB);
				set_geometry(l_Image.columns(), l_Image.rows());
				uint8_t* l_Data = new uint8_t[get_width() * get_height() * get_bytes_per_pixel()];
				size_t l_Offset = 0;
				for(size_t l_Y = 0; l_Y < get_height(); l_Y++)
					for(size_t l_X = 0; l_X < get_width(); l_X++, l_Offset++)
					{
#if QuantumDepth == 8
						l_Data[l_Offset * get_bytes_per_pixel()] = 
							l_PixelPacket[l_Offset].red;
						l_Data[l_Offset * get_bytes_per_pixel() + 1] = 
							l_PixelPacket[l_Offset].green;
						l_Data[l_Offset * get_bytes_per_pixel() + 2] = 
							l_PixelPacket[l_Offset].blue;
#elif QuantumDepth == 16
						l_Data[l_Offset * get_bytes_per_pixel()] = 
							l_PixelPacket[l_Offset].red >> 8;
						l_Data[l_Offset * get_bytes_per_pixel() + 1] = 
							l_PixelPacket[l_Offset].green >> 8;
						l_Data[l_Offset * get_bytes_per_pixel() + 2] = 
							l_PixelPacket[l_Offset].blue >> 8;
#else
#	error "Specified value of QuantumDepth is not supported"
#endif
					}

				set_data(l_Data);
#else // !HAS_MAGICK
				if(0 == strcasecmp(l_Ext, "jpg"))
				{
					*this = jpeg::image((std::string(l_Path) + std::string("/") + std::string(l_DirEntry->d_name)).c_str());
				}
				else if(0 == strcasecmp(l_Ext, "png"))
				{
					*this = png::image((std::string(l_Path) + std::string("/") + std::string(l_DirEntry->d_name)).c_str());
				}
				else if(0 == strcasecmp(l_Ext, "tga"))
				{
					*this = tga::image((std::string(l_Path) + std::string("/") + std::string(l_DirEntry->d_name)).c_str());
				}
				else
				{
					free(l_TestName);
					free(l_Path);

					closedir(l_Directory);

					throw std::runtime_error("Unknown file extension");
				}
#endif

				free(l_TestName);
				free(l_Path);

				closedir(l_Directory);

				return *this;
			}

			free(l_TestName);
		}

		closedir(l_Directory);

		free(l_Path);

		throw std::runtime_error("File not found");
	}
};

