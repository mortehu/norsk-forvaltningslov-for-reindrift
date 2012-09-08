//
// 2D and 3D Vector Classes
//
// Copyright (C) Morten Hustveit 2000-2002
//

#ifndef _VECTOR_H_
#define _VECTOR_H_

#include <stdlib.h>
#include <math.h>

#ifndef M_PI
#define M_PI 3.1415926535897932384626433832795
#endif

#ifdef VECTOR_MACROS
#define x .cross
#endif

namespace cls
{
	class vector2
	{
	protected:
		float m_Data[2];

	public:

		vector2() { }

		vector2(
			const float p_U,
			const float p_V)
		{
			m_Data[0] = p_U;
			m_Data[1] = p_V;
		}

		inline float& operator()(const size_t p_Index)
		{
			return m_Data[p_Index];
		}

		inline const float& operator()(const size_t p_Index) const
		{
			return m_Data[p_Index];
		}

		const float* get_data() const
		{
			return m_Data;
		}

		float* get_data()
		{
			return m_Data;
		}

		vector2 operator+(const vector2& p_Vector) const
		{
			return vector2(
				m_Data[0] + p_Vector.m_Data[0],
				m_Data[1] + p_Vector.m_Data[1]);
		}

		vector2& operator+=(const vector2& p_Vector)
		{
			m_Data[0] += p_Vector.m_Data[0];
			m_Data[1] += p_Vector.m_Data[1];

			return *this;
		}

		vector2 operator-() const
		{
			return vector2(-m_Data[0], -m_Data[1]);
		}

		vector2 operator-(const vector2& p_Vector) const
		{
			return vector2(
				m_Data[0] - p_Vector.m_Data[0],
				m_Data[1] - p_Vector.m_Data[1]);
		}

		vector2& operator-=(const vector2& p_Vector)
		{
			m_Data[0] -= p_Vector.m_Data[0];
			m_Data[1] -= p_Vector.m_Data[1];

			return *this;
		}

		float operator*(const vector2& p_Vector) const
		{
			return m_Data[0] * p_Vector.m_Data[0]
			     + m_Data[1] * p_Vector.m_Data[1];
		}

		vector2 operator*(const float p_Value) const
		{
			return vector2(
				m_Data[0] * p_Value,
				m_Data[1] * p_Value);
		}

		vector2 operator*(const double p_Value) const
		{
			return vector2(
				m_Data[0] * p_Value,
				m_Data[1] * p_Value);
		}

		vector2& operator*=(const float p_Value)
		{
			m_Data[0] *= p_Value;
			m_Data[1] *= p_Value;

			return *this;
		}

		vector2& operator*=(const double p_Value)
		{
			m_Data[0] *= p_Value;
			m_Data[1] *= p_Value;

			return *this;
		}

		vector2& operator*=(const vector2& p_Vector)
		{
			m_Data[0] *= p_Vector.m_Data[0];
			m_Data[1] *= p_Vector.m_Data[1];

			return *this;
		}

		vector2 operator/(const float p_Value) const
		{
			return vector2(
				m_Data[0] / p_Value,
				m_Data[1] / p_Value);
		}

		vector2& operator/=(const float p_Value)
		{
			m_Data[0] /= p_Value;
			m_Data[1] /= p_Value;

			return *this;
		}

		vector2& operator/=(const vector2& p_Vector)
		{
			m_Data[0] /= p_Vector.m_Data[0];
			m_Data[1] /= p_Vector.m_Data[1];

			return *this;
		}

		vector2& operator=(const vector2& v)
		{
			m_Data[0] = v.m_Data[0];
			m_Data[1] = v.m_Data[1];

			return *this;
		}

		float square() const
		{
			return m_Data[0] * m_Data[0] + m_Data[1] * m_Data[1];
		}

		float magnitude() const
		{
			return sqrt(square());
		}

		vector2& normalize()
		{
			float l_Magnitude = magnitude();

			m_Data[0] /= l_Magnitude;
			m_Data[1] /= l_Magnitude;
			
			return *this;
		}

		bool operator==(const vector2& p_Vector) const
		{
			return (m_Data[0] == p_Vector.m_Data[0])
			    && (m_Data[1] == p_Vector.m_Data[1]);
		}

		bool operator>=(const vector2& p_Vector) const
		{
			return (m_Data[0] >= p_Vector.m_Data[0])
			    && (m_Data[1] >= p_Vector.m_Data[1]);
		}

		bool operator<=(const vector2& p_Vector) const
		{
			return (m_Data[0] <= p_Vector.m_Data[0])
			    && (m_Data[1] <= p_Vector.m_Data[1]);
		}
	};

	class vector3
	{
	protected:
		float m_Data[3];

	public:
		
		vector3() { }
		
		vector3(float p_X, float p_Y, float p_Z)
		{
			m_Data[0] = p_X;
			m_Data[1] = p_Y;
			m_Data[2] = p_Z;
		}

		inline float& operator()(const size_t p_Index)
		{
			return m_Data[p_Index];
		}

		inline const float& operator()(const size_t p_Index) const
		{
			return m_Data[p_Index];
		}

		const float* get_data() const
		{
			return m_Data;
		}

		float* get_data()
		{
			return m_Data;
		}

		vector3 operator+(const vector3 p_Vector) const
		{
			return vector3(
				m_Data[0] + p_Vector.m_Data[0],
				m_Data[1] + p_Vector.m_Data[1],
				m_Data[2] + p_Vector.m_Data[2]);
		}

		vector3& operator+=(const vector3& p_Vector)
		{
			m_Data[0] += p_Vector.m_Data[0];
			m_Data[1] += p_Vector.m_Data[1];
			m_Data[2] += p_Vector.m_Data[2];

			return *this;
		}

		vector3 operator-() const
		{
			return vector3(-m_Data[0], -m_Data[1], -m_Data[2]);
		}

		vector3 operator-(const vector3 p_Vector) const
		{
			return vector3(
				m_Data[0] - p_Vector.m_Data[0],
				m_Data[1] - p_Vector.m_Data[1],
				m_Data[2] - p_Vector.m_Data[2]);
		}

		vector3& operator-=(const vector3& p_Vector)
		{
			m_Data[0] -= p_Vector.m_Data[0];
			m_Data[1] -= p_Vector.m_Data[1];
			m_Data[2] -= p_Vector.m_Data[2];

			return *this;
		}

		float operator*(const vector3& p_Vector) const
		{
			return m_Data[0] * p_Vector.m_Data[0]
			     + m_Data[1] * p_Vector.m_Data[1]
			     + m_Data[2] * p_Vector.m_Data[2];
		}

		vector3 operator*(const float p_Value) const
		{
			return vector3(
				m_Data[0] * p_Value,
				m_Data[1] * p_Value,
				m_Data[2] * p_Value);
		}

		vector3 operator*(const double p_Value) const
		{
			return vector3(
				m_Data[0] * p_Value,
				m_Data[1] * p_Value,
				m_Data[2] * p_Value);
		}

		template<class matrix_class>
		vector3 operator*(const matrix_class p_Matrix) const
		{
			return vector3(
				  m_Data[0] * p_Matrix(0, 0)
				+ m_Data[1] * p_Matrix(0, 1)
				+ m_Data[2] * p_Matrix(0, 2) + p_Matrix(0, 3),
				  m_Data[0] * p_Matrix(1, 0)
				+ m_Data[1] * p_Matrix(1, 1)
				+ m_Data[2] * p_Matrix(1, 2) + p_Matrix(1, 3),
				  m_Data[0] * p_Matrix(2, 0)
				+ m_Data[1] * p_Matrix(2, 1)
				+ m_Data[2] * p_Matrix(2, 2) + p_Matrix(2, 3));
		}

		vector3& operator*=(const float p_Value)
		{
			m_Data[0] *= p_Value;
			m_Data[1] *= p_Value;
			m_Data[2] *= p_Value;

			return *this;
		}

		vector3& operator*=(const double p_Value)
		{
			m_Data[0] *= p_Value;
			m_Data[1] *= p_Value;
			m_Data[2] *= p_Value;

			return *this;
		}

		vector3& operator*=(const vector3& p_Vector)
		{
			m_Data[0] *= p_Vector.m_Data[0];
			m_Data[1] *= p_Vector.m_Data[1];
			m_Data[2] *= p_Vector.m_Data[2];

			return *this;
		}

		template<class matrix_class>
		vector3& operator*=(const matrix_class p_Matrix)
		{
			return (*this = *this * p_Matrix);
		}

		vector3 operator/(const float p_Value) const
		{
			return vector3(
				m_Data[0] / p_Value,
				m_Data[1] / p_Value,
				m_Data[2] / p_Value);
		}

		vector3& operator/=(const float p_Value)
		{
			m_Data[0] /= p_Value;
			m_Data[1] /= p_Value;
			m_Data[2] /= p_Value;

			return *this;
		}

		vector3& operator/=(const vector3& p_Vector)
		{
			m_Data[0] /= p_Vector.m_Data[0];
			m_Data[1] /= p_Vector.m_Data[1];
			m_Data[2] /= p_Vector.m_Data[2];

			return *this;
		}

		vector3& operator=(const vector3& v)
		{
			m_Data[0] = v.m_Data[0];
			m_Data[1] = v.m_Data[1];
			m_Data[2] = v.m_Data[2];

			return *this;
		}

		vector3 cross(const vector3& p_Vector) const
		{
			return vector3(
				m_Data[1] * p_Vector.m_Data[2] - p_Vector.m_Data[1] * m_Data[2],
				m_Data[2] * p_Vector.m_Data[0] - p_Vector.m_Data[2] * m_Data[0],
				m_Data[0] * p_Vector.m_Data[1] - p_Vector.m_Data[0] * m_Data[1]);
		}

		float square() const
		{
			return m_Data[0] * m_Data[0] + m_Data[1] * m_Data[1] + m_Data[2] * m_Data[2];
		}

		float magnitude() const
		{
			return sqrt(square());
		}

		vector3& normalize()
		{
			float l_Magnitude = magnitude();

			m_Data[0] /= l_Magnitude;
			m_Data[1] /= l_Magnitude;
			m_Data[2] /= l_Magnitude;
			
			return *this;
		}

		bool operator==(const vector3& p_Vector) const
		{
			return (m_Data[0] == p_Vector.m_Data[0])
			    && (m_Data[1] == p_Vector.m_Data[1])
			    && (m_Data[2] == p_Vector.m_Data[2]);
		}

		bool operator>=(const vector3& p_Vector) const
		{
			return (m_Data[0] >= p_Vector.m_Data[0])
			    && (m_Data[1] >= p_Vector.m_Data[1])
			    && (m_Data[2] >= p_Vector.m_Data[2]);
		}

		bool operator<=(const vector3& p_Vector) const
		{
			return (m_Data[0] <= p_Vector.m_Data[0])
			    && (m_Data[1] <= p_Vector.m_Data[1])
			    && (m_Data[2] <= p_Vector.m_Data[2]);
		}
	};
};

cls::vector2 operator*(const float p_Value, const cls::vector2& p_Vector);
cls::vector3 operator*(const float p_Value, const cls::vector3& p_Vector);

#endif

