#define VECTOR_MACROS

#include <math.h>

#include <common.h>
#include <vector.h>
#include <raytracer.h>

namespace cls
{
	bool ray::intersect( 
		const vector3& p_Mins, 
		const vector3& p_Maxs) const
	{
		for(size_t i = 0; i < 3; i++)
		{
			float l_TNear = -HUGE_VAL;
			float l_TFar = HUGE_VAL;

			// *** Check whether ray is parallell to plane

			if((*this)(i) > -EPSILON && (*this)(i) < EPSILON)
			{
				// *** Check whether ray starts between planes

				if(m_Point(i) < p_Mins(i) || m_Point(i) > p_Maxs(i))
					return false;
			}
			else
			{
				float l_T1 = (p_Mins(i) - m_Point(i)) / (*this)(i);
				float l_T2 = (p_Maxs(i) - m_Point(i)) / (*this)(i);

				if(l_T1 > l_T2)
				{
					if(l_T2 > l_TNear)
						l_TNear = l_T2;

					if(l_T1 < l_TFar)
						l_TFar = l_T1;
				}
				else
				{
					if(l_T1 > l_TNear)
						l_TNear = l_T1;

					if(l_T2 < l_TFar)
						l_TFar = l_T2;
				}

				if(l_TFar < 0)
					return false;

				if(l_TNear > l_TFar)
					return false;
			}
		}

		return true;
	}

	float ray::hit( 
		float& r_Alpha, float& r_Beta, 
		const vector3& p_Point0, 
		const vector3& p_Point1, 
		const vector3& p_Point2, 
		const vector3& p_Normal) const
	{
		const float l_DotProduct = *this * p_Normal;

		if(l_DotProduct < EPSILON && l_DotProduct > -EPSILON)
			return HUGE_VAL;

		const float l_Distance = (p_Normal * p_Point0 - p_Normal * m_Point) 
		                       / l_DotProduct;

		if(l_Distance < 0)
			return HUGE_VAL;

		size_t i1, i2;

		if((fabs(p_Normal(0)) >= fabs(p_Normal(1)))
		&& (fabs(p_Normal(0)) >= fabs(p_Normal(2))))
			i1 = 1, i2 = 2;
		else if(fabs(p_Normal(1)) >= fabs(p_Normal(2)))
			i1 = 0, i2 = 2;
		else
			i1 = 0, i2 = 1;

		const vector2 l_Point( 
			m_Point(i1) + (*this)(i1) * l_Distance, 
			m_Point(i2) + (*this)(i2) * l_Distance);

		const vector3 l_U( 
			l_Point(0) - p_Point0(i1), 
			p_Point1(i1) - p_Point0(i1), 
			p_Point2(i1) - p_Point0(i1));

		const vector3 l_V( 
			l_Point(1) - p_Point0(i2), 
			p_Point1(i2) - p_Point0(i2), 
			p_Point2(i2) - p_Point0(i2));

		if(l_U(1) == 0)
		{
			r_Beta = l_U(0) / l_U(2);

			if(r_Beta < 0 || r_Beta > 1)
				return HUGE_VAL;

			r_Alpha = (l_V(0) - r_Beta * l_V(2)) / l_V(1);
		}
		else
		{
			r_Beta = (l_V(0) * l_U(1) - l_U(0) * l_V(1)) 
			       / (l_V(2) * l_U(1) - l_U(2) * l_V(1));

			if(r_Beta < 0 || r_Beta > 1)
				return HUGE_VAL;

			r_Alpha = (l_U(0) - r_Beta * l_U(2)) / l_U(1);
		}

		if(r_Alpha < 0 || (r_Alpha + r_Beta) > 1)
			return HUGE_VAL;

		return l_Distance;
	}
}

