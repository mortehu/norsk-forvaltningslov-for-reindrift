//
// 4x4 Matrix Class
//
// Copyright (C) Morten Hustveit 2000
//

#ifndef _MATRIX_H_
#define _MATRIX_H_

#include <math.h>
#include <vector>

namespace cls
{
	class matrix
	{
	private:	
		float m_Values[4][4];

	public:
		matrix()
		{
			for(size_t a = 0; a < 4; a++)
				for(size_t b = 0; b < 4; b++)
					m_Values[a][b] = (a == b) ? 1.0f : 0.0f;
		}

		~matrix() { }

		inline float& operator()(const size_t p_A, const size_t p_B)
		{
			return m_Values[p_A][p_B];
		}

		inline const float& operator()(const size_t p_A, const size_t p_B) const
		{
			return m_Values[p_A][p_B];
		}

		matrix operator*(const matrix p_Matrix) const
		{
			matrix r_Matrix;

			for(size_t a = 0; a < 4; a++)
				for(size_t b = 0; b < 4; b++)
				{
					r_Matrix(a, b) = 0;
					
					for(size_t c = 0; c < 4; c++)
						r_Matrix(a, b) += (*this)(a, c) * p_Matrix(c, b);
				}

			return r_Matrix;
		}

		// *** Procedural methods. Apply to identity matrices only
			
		matrix& operator*=(const matrix p_Matrix)
		{
			return (*this = *this * p_Matrix);
		}

		template<class vector_class>
		matrix& rotate(const vector_class& p_Axis, float p_Angle)
		{
			float sa = sin(p_Angle);
			float ca = cos(p_Angle);
			float dx = p_Axis(0) / p_Axis.magnitude();
			float dy = p_Axis(1) / p_Axis.magnitude();
			float dz = p_Axis(2) / p_Axis.magnitude();

			(*this)(0, 0) = ca + (1 - ca) * dx * dx;
			(*this)(0, 1) =      (1 - ca) * dx * dy - sa * dz;
			(*this)(0, 2) =      (1 - ca) * dx * dz + sa * dy;
			(*this)(1, 0) =      (1 - ca) * dy * dx + sa * dz;
			(*this)(1, 1) = ca + (1 - ca) * dy * dy;
			(*this)(1, 2) =      (1 - ca) * dy * dz - sa * dx;
			(*this)(2, 0) =      (1 - ca) * dz * dx - sa * dy;
			(*this)(2, 1) =      (1 - ca) * dz * dy + sa * dx;
			(*this)(2, 2) = ca + (1 - ca) * dz * dz;

			return *this;
		}
		
		matrix& scale(float p_X, float p_Y, float p_Z)
		{
			(*this)(0, 0) = p_X;
			(*this)(1, 1) = p_Y;
			(*this)(2, 2) = p_Z;

			return *this;
		}

		matrix& translate(float p_X, float p_Y, float p_Z)
		{
			(*this)(0, 3) = p_X;
			(*this)(1, 3) = p_Y;
			(*this)(2, 3) = p_Z;

			return *this;
		}
	};

	class matrix_stack
	{
	private:
		std::vector<matrix> m_Stack;
		matrix              m_Current;

	public:
		void push(const matrix p_Matrix)
		{
			m_Current *= p_Matrix;
			m_Stack.push_back(p_Matrix);
		}

		void pop()
		{
			m_Stack.pop_back();

			m_Current = matrix();

			std::vector<matrix>::iterator i_Matrix;

			for(i_Matrix = m_Stack.begin(); i_Matrix != m_Stack.end();
				i_Matrix++)
			{
				m_Current *= *i_Matrix;
			}
		}

		void clear()
		{
			m_Stack.clear();

			m_Current = matrix();
		}

		matrix current() const
		{
			return m_Current;
		}
	};
};

#endif

