#ifndef _SPLINE_H_
#define _SPLINE_H_

// Based on code by Tao Chen (http://www.codeguru.com/gdi/Spline.html)

#include <algorithm>

#include <vector.h>

namespace cls
{
	template<class point>
	class spline
	{
	public:
		size_t   m_PointCount;
		point*   m_Points;

		point*   m_A;
		point*   m_B;
		point*   m_C;
		float*   m_K;
		float*   m_Mat[3];

		spline(const point* p_Points, size_t p_PointCount)
		{
			m_PointCount = p_PointCount;

			m_Points = new point[m_PointCount];

			std::copy(p_Points, p_Points + p_PointCount, m_Points);

			m_A = new point[m_PointCount];
			m_B = new point[m_PointCount];
			m_C = new point[m_PointCount];
			m_K = new float[m_PointCount];
			m_Mat[0] = new float[m_PointCount];
			m_Mat[1] = new float[m_PointCount];
			m_Mat[2] = new float[m_PointCount];
			
		}

		~spline()
		{
			delete [] m_Points;
			delete [] m_A;
			delete [] m_B;
			delete [] m_C;
			delete [] m_K;
			delete [] m_Mat[0];
			delete [] m_Mat[1];
			delete [] m_Mat[2];
		}

		void generate()
		{
			for(size_t i = 0; i < m_PointCount - 1; i++)
			{
				m_A[i] = m_Points[i + 1] - m_Points[i];
			}

			float AMagOld = m_A[0].magnitude();

			for(size_t i = 0; i < m_PointCount - 2; i++)
			{
				float AMag = m_A[i + 1].magnitude();
	
				m_K[i] = AMagOld / AMag;

				AMagOld = AMag;
			}

			m_K[m_PointCount - 2] = 1.0f;

			for(size_t i = 1; i < m_PointCount - 1; i++)
			{
				m_Mat[0][i] = 1.0;
				m_Mat[1][i] = 2.0 * m_K[i - 1] * (1.0 + m_K[i - 1]);
				m_Mat[2][i] = m_K[i - 1] * m_K[i - 1] * m_K[i];
			}

			m_Mat[1][0] = 2.0;
			m_Mat[2][0] = m_K[0];
			m_Mat[0][m_PointCount - 1] = 1.0;
			m_Mat[1][m_PointCount - 1] = 2.0 * m_K[m_PointCount - 2];

			for(size_t i = 1; i < m_PointCount - 1; i++)
			{
				m_B[i] = 3.0 * (m_A[i - 1] + m_K[i - 1] * m_K[i - 1] * m_A[i]);
			}

			m_B[0] = 3.0 * m_A[0];

			m_B[m_PointCount - 1] = 3.0 * m_A[m_PointCount - 2];

			matrix_solve(m_B);

			for(size_t i = 0; i < m_PointCount - 1; i++)
			{
				m_C[i] = m_K[i] * m_B[i + 1];
			}
		}

		void matrix_solve(point* B) 
		{
			point* Work = new point[m_PointCount];
			point* WorkB = new point[m_PointCount];

			for(size_t i=0;i<=m_PointCount-1;i++) 
			{
				Work[i] = B[i] / m_Mat[1][i];
				WorkB[i] = Work[i];
			}

			for(size_t j=0 ; j<10 ; j++) 
			{
				///  need convergence judge

				Work[0] = (B[0] - m_Mat[2][0]*WorkB[1])/m_Mat[1][0];

				for(size_t i=1; i<m_PointCount-1 ; i++ ) 
				{
					Work[i] = (B[i]-m_Mat[0][i]*WorkB[i-1]-m_Mat[2][i]*WorkB[i+1])
					        / m_Mat[1][i];
				}

				Work[m_PointCount-1] = 
					(B[m_PointCount-1] - m_Mat[0][m_PointCount-1]*WorkB[m_PointCount-2])
					/ m_Mat[1][m_PointCount-1];

				for(size_t i=0 ; i<=m_PointCount-1 ; i++ ) 
				{
					WorkB[i] = Work[i];
				}
			}

			for(size_t i=0 ; i<=m_PointCount-1 ; i++ ) 
			{
				B[i] = Work[i];
			}

			delete [] Work;
			delete [] WorkB;
		}

		point get_point(size_t p_Index, float t) const
		{
			float f = t * t * (3.0 - 2.0 * t);
			float g = t * (t - 1.0) * (t - 1.0);
			float h = t * t * (t - 1.0);

			
			point r_Point = m_Points[p_Index] 
			              + m_A[p_Index] * f 
			              + m_B[p_Index] * g 
			              + m_C[p_Index] * h;

			return r_Point;
		}

		point get_point(float p_Offset) const
		{
			double l_Index;
			float l_Offset = modf(p_Offset * (m_PointCount - 1), &l_Index); 

			return get_point((size_t) l_Index, l_Offset);
		}
	};
};

#endif

