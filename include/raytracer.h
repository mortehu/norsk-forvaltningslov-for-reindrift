#ifndef _RAYTRACER_H_
#define _RAYTRACER_H_

#include <common.h>
#include <vector.h>
#include <image.h>
#include <geom.h>

namespace cls
{
	class ray : public vector3
	{
	private:
		vector3 m_Point;

	public:
		ray(const vector3& p_Point, const vector3& p_Direction)
		{
			m_Point = p_Point;
			m_Data[0] = p_Direction(0);
			m_Data[1] = p_Direction(1);
			m_Data[2] = p_Direction(2);
		}

		inline const vector3& get_point() const
		{
			return m_Point;
		}

		// *** Determine whether the ray will intersect the given 
		//     axis aligned box.

		bool intersect( 
			const vector3& p_Mins, 
			const vector3& p_Maxs) const;

		// *** Determine whether and where the ray will hit the given 
		//     triangle.

		float hit( 
			float& r_Alpha, float& r_Beta, 
			const vector3& p_Point0, 
			const vector3& p_Point1, 
			const vector3& p_Point2, 
			const vector3& p_Normal) const;
	};

	class raytracer
	{
	private:
		geom*   m_Geometry;
		size_t  m_Width;
		size_t  m_Height;

	public:
		raytracer(geom& p_Geometry, const size_t p_Width, const size_t p_Height)
		{
			m_Geometry = &p_Geometry;
			m_Width = p_Width;
			m_Height = p_Height;
		}

		image& render(
			const vector3& p_Eye, 
			const vector3& p_UpperLeft, 
			const vector3& p_XDirection, 
			const vector3& p_YDirection, 
			image& r_Image) const;
	};
}

#endif // !_GEOM_H_

