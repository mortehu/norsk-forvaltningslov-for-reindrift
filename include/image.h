#ifndef _IMAGE_H_
#define _IMAGE_H_

#include <iostream>
#include <assert.h>
#include <stdlib.h>

#include <common.h>

namespace cls
{
	class camera;

	class dataproxy
	{
	public:
		size_t   m_RefCount;
		uint8_t* m_Data;

		dataproxy()
		{
			m_RefCount = 1;
			m_Data = NULL;
		}

		~dataproxy()
		{
			assert(!m_RefCount);

			if(m_Data)
				delete [] m_Data;
		}
	};

	class image
	{
	public:
		enum pixel_format
		{
			RGB = 1, RGBA = 2, GRAY = 3, 
		};
	
	private:
		size_t       m_GLHandle;
		bool         m_XRepeat, m_YRepeat;
		bool         m_MipMap;
		pixel_format m_PixelFormat;	
		size_t       m_Width, m_Height;
		dataproxy*   m_DataProxy;
	
	public:
		image()
		{
			m_XRepeat = true;
			m_YRepeat = true;
			m_MipMap = true;
			m_GLHandle = 0;
			m_DataProxy = NULL;
		}

		~image()
		{
			if(!m_DataProxy)
				return;

			if(0 == --m_DataProxy->m_RefCount)
				delete m_DataProxy;
		}

		bool get_x_repeat() const
		{
			return m_XRepeat;
		}

		bool get_y_repeat() const
		{
			return m_YRepeat;
		}

		void set_repeat(const bool p_XRepeat, const bool p_YRepeat)
		{
			m_XRepeat = p_XRepeat;
			m_YRepeat = p_YRepeat;

			// XXX: if(m_GLHandle) update...
		}

		pixel_format get_pixel_format() const
		{
			return m_PixelFormat;
		}

		void set_pixel_format(const pixel_format p_PixelFormat)
		{
			m_PixelFormat = p_PixelFormat;
		}

		size_t get_width() const
		{
			return m_Width;
		}

		size_t get_height() const
		{
			return m_Height;
		}

		double get_aspect() const
		{
			return double(m_Width) / m_Height;
		}

		size_t get_bytes_per_pixel() const
		{
			return (m_PixelFormat == RGB)  ? 3 
			     : (m_PixelFormat == RGBA) ? 4 
				 : (m_PixelFormat == GRAY) ? 1 
				 : 0;
		}
	
		void set_geometry(const size_t p_Width, const size_t p_Height)
		{
			m_Width = p_Width;
			m_Height = p_Height;
		}

		void set_data(uint8_t* p_Data)
		{
			if(!m_DataProxy)
			{
				m_DataProxy = new dataproxy();
			}
			else if(m_DataProxy->m_Data)
			{
				delete [] m_DataProxy->m_Data;
			}

			m_DataProxy->m_Data = p_Data;
		}

		uint8_t* get_data() const
		{
			if(m_DataProxy == (void*)0xe)
				return NULL;
			return m_DataProxy ? m_DataProxy->m_Data : NULL;
		}

		image& operator=(const image& p_Image)
		{
			p_Image.m_DataProxy->m_RefCount++;

			if(m_DataProxy)
			{
				if(0 == --m_DataProxy->m_RefCount)
					delete m_DataProxy;
			}

			m_DataProxy = p_Image.m_DataProxy;
			
			set_geometry(p_Image.get_width(), p_Image.get_height());
			set_pixel_format(p_Image.get_pixel_format());

			return *this;
		}

		void activate(size_t p_Pass = 0);

		image& load(const char* p_FileName);
	};
};

#endif

