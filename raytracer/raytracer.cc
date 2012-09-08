#include <iostream>

#include <raytracer.h>
#include <color.h>

namespace cls
{
	image& raytracer::render(
		const vector3& p_Eye, 
		const vector3& p_UpperLeft, 
		const vector3& p_XDirection, 
		const vector3& p_YDirection, 
		image& r_Image) const
	{
		r_Image.set_geometry(m_Width, m_Height);
		r_Image.set_pixel_format(image::RGB);

		uint8_t* l_Data = new uint8_t[m_Width * m_Height * r_Image.get_bytes_per_pixel()];

		size_t l_Offset = 0;

		size_t l_Last = 0;

		for(size_t l_Y = 0; l_Y < m_Height; l_Y++)
		{
			if(l_Y * 100 / m_Height > l_Last)
			{
				l_Last = l_Y * 100 / m_Height;

				std::cerr << "\r" << l_Last << "%";
			}
			for(size_t l_X = 0; l_X < m_Width; l_X++, l_Offset += 3)
			{
				vector3 l_Target = p_UpperLeft 
				                 + (float) l_X / (m_Width - 1) * p_XDirection 
				                 + (float) l_Y / (m_Height - 1) * p_YDirection;

				vector3 l_Direction = l_Target - p_Eye;

				color l_Color = m_Geometry->trace_ray(ray(p_Eye, l_Direction));

				l_Data[l_Offset]     = l_Color.m_Red;
				l_Data[l_Offset + 1] = l_Color.m_Green;
				l_Data[l_Offset + 2] = l_Color.m_Blue;
			}
		}

		std::cerr << std::endl;

		r_Image.set_data(l_Data);

		return r_Image;
	}	
};

