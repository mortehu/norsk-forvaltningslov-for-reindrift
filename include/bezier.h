#ifndef _BEZIER_H_
#define _BEZIER_H_

#include <math.h>
#include <stdlib.h>

namespace cls
{
	class patch_strips
	{
	private:
		size_t m_Width;
		size_t m_Height;

	public:
		patch_strips(size_t p_Width, size_t p_Height)
		{
			m_Width = p_Width;
			m_Height = p_Height;
		}

		size_t size() const
		{
			return (m_Width - 1) * m_Height * 2;
		}

		signed int operator[](size_t p_Index) const
		{
			size_t l_Row   = p_Index / (m_Height << 1);
			size_t l_Index = p_Index % (m_Height << 1);

			return (l_Index == 0) ? (-((l_Row + 1) * m_Height))
			                      : ((l_Row + (~l_Index & 1)) * m_Height + (l_Index >> 1));
		}
	};

	template<class point>
	class bezier
	{
	private:
		size_t       m_Width;
		size_t       m_Height;
		const point* m_ControlPoints;

		size_t fact(size_t p_Value) const
		{
			if(p_Value == 0)
				return 1;

			size_t r_Value = p_Value;

			while(--p_Value)
				r_Value *= p_Value;

			return r_Value;
		}

		double coeff(
			const double p_Offset,
			const size_t p_Index,
			const size_t p_Level) const
		{
			return pow(p_Offset, (double) p_Index) * pow(1 - p_Offset, (double) p_Level - p_Index - 1)
				* fact(p_Level - 1) / fact(p_Index) / fact(p_Level - p_Index - 1);
		}

	public:
		bezier(
			const size_t p_Width,
			const size_t p_Height,
			const point* p_ControlPoints)
		{
			m_Width = p_Width;
			m_Height = p_Height;
			m_ControlPoints = p_ControlPoints;
		}


		// *** Returns the 3D point at the specified 2D coordinate of the
		//     patch.

		//     0 <= p_X <= (m_Width - 1)
		//     0 <= p_Y <= (m_Height - 1)

		point get_point(
			const size_t p_X,
			const size_t p_Y) const
		{
			return m_ControlPoints[p_Y * m_Width + p_X];
		}

		//     0 <= p_X <= 1
		//     0 <= p_Y <= (m_Height - 1)

		point get_point(
			const double p_X,
			const size_t p_Y) const
		{
			point r_Point = get_point(size_t(0), p_Y) * coeff(p_X, 0, m_Width);

			for(size_t l_X = 1; l_X < m_Width; l_X++)
				r_Point += get_point(l_X, p_Y) * coeff(p_X, l_X, m_Width);

			return r_Point;
		}

		//     0 <= p_X <= 1
		//     0 <= p_Y <= 1

		point get_point(
			const double p_X,
			const double p_Y) const
		{
			point r_Point = get_point(p_X, size_t(0)) * coeff(p_Y, 0, m_Height);

			for(size_t l_Y = 1; l_Y < m_Height; l_Y++)
				r_Point += get_point(p_X, l_Y) * coeff(p_Y, l_Y, m_Height);

			return r_Point;
		}


		// *** Fills [p_Width * p_Height] vertices into specified array.
		//
		//     p_Width and p_Height must be odd.
		//     The array must be preallocated by the user.

		void create_vertices(
			const size_t p_Width,
			const size_t p_Height,
			point*       r_Vertices) const
		{
			double l_YScale = 1.0 / (p_Height - 1);
			double l_XScale = 1.0 / (p_Width - 1);

			for(size_t l_X = 0; l_X < p_Width; l_X++)
				for(size_t l_Y = 0; l_Y < p_Height; l_Y++)
					r_Vertices[l_X * p_Height + l_Y] = get_point(l_X * l_XScale, l_Y * l_YScale);
		}
	};
}

#endif

