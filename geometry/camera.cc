#include <stdexcept>
#include <stdlib.h>
#include <GL/gl.h>

#include <geom.h>
#include <vector.h>

static void gluLookAt(GLdouble eyex, GLdouble eyey, GLdouble eyez,
	GLdouble centerx, GLdouble centery, GLdouble centerz,
	GLdouble upx, GLdouble upy, GLdouble upz);

namespace cls
{
	void camera::activate()
	{
		if(fabs(m_Direction(2)) < 1 - EPSILON)
		{
			m_Left = m_Direction.cross(vector3(0, 0, 1));

			m_Up = m_Left.cross(m_Direction) * matrix().rotate(m_Direction, m_Roll);
			m_Left = m_Up.cross(m_Direction);
		}
		else
		{
			// *** Impossible to determine yaw

			throw std::runtime_error("Camera pointing directly upwards");
		}
		m_Up.normalize();
		m_Left.normalize();

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();

		const float l_ZNear = 10.0;
		const float l_ZFar = 10000.0;
		const float l_Aspect = 4.0 / 3.0;

		vector2 l_Min, l_Max;

		l_Max(1) = l_ZNear * tan(m_FOV * M_PI / 360.0);
		l_Min(1) = -l_Max(1);

		l_Max(0) = l_Max(1) * l_Aspect;
		l_Min(0) = l_Min(1) * l_Aspect;

		glFrustum(l_Min(0), l_Max(0), l_Min(1), l_Max(1), l_ZNear, l_ZFar);

		gluLookAt(
			m_Position(0), m_Position(1), m_Position(2),
			m_Position(0) + m_Direction(0),
			m_Position(1) + m_Direction(1), 
			m_Position(2) + m_Direction(2),
			m_Up(0), m_Up(1), m_Up(2));

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		// XXX: adjust clipping planes to FOV

		m_Normals[0] = m_Direction * matrix().rotate(m_Up, /*m_FOV * (M_PI / 90)*/ - (M_PI - m_FOV * (M_PI / 90)));
		m_Normals[1] = m_Direction * matrix().rotate(m_Up, /*-m_FOV * (M_PI / 90)*/ + (M_PI - m_FOV * (M_PI / 90)));
		m_Normals[2] = m_Direction * matrix().rotate(m_Left, /*m_FOV * (M_PI / 90)*/ - (M_PI - m_FOV * (M_PI / 90)));
		m_Normals[3] = m_Direction * matrix().rotate(m_Left, /*-m_FOV * (M_PI / 90)*/ + (M_PI - m_FOV * (M_PI / 90)));

		for(size_t i = 0; i < 4; i++)
			m_Distance[i] = m_Normals[i] * m_Position;
	}

	bool camera::inside(const vector3& p_Mins, const vector3& p_Maxs) const
	{
		vector3 l_Vectors[8];

		l_Vectors[0] = p_Mins;
		l_Vectors[1] = vector3(p_Maxs(0), p_Mins(1), p_Mins(2));
		l_Vectors[2] = vector3(p_Mins(0), p_Maxs(1), p_Mins(2));
		l_Vectors[3] = vector3(p_Maxs(0), p_Maxs(1), p_Mins(2));
		l_Vectors[4] = vector3(p_Mins(0), p_Mins(1), p_Maxs(2));
		l_Vectors[5] = vector3(p_Maxs(0), p_Mins(1), p_Maxs(2));
		l_Vectors[6] = vector3(p_Mins(0), p_Maxs(1), p_Maxs(2));
		l_Vectors[7] = p_Maxs;

		for(size_t i = 0; i < 4; i++)
		{			
			if((l_Vectors[0] * m_Normals[i] < m_Distance[i])
			&& (l_Vectors[1] * m_Normals[i] < m_Distance[i])
			&& (l_Vectors[2] * m_Normals[i] < m_Distance[i])
			&& (l_Vectors[3] * m_Normals[i] < m_Distance[i])
			&& (l_Vectors[4] * m_Normals[i] < m_Distance[i])
			&& (l_Vectors[5] * m_Normals[i] < m_Distance[i])
			&& (l_Vectors[6] * m_Normals[i] < m_Distance[i])
			&& (l_Vectors[7] * m_Normals[i] < m_Distance[i]))
				return false;
		}

		return true;
	}
};

static void gluLookAt(GLdouble eyex, GLdouble eyey, GLdouble eyez,
	GLdouble centerx, GLdouble centery, GLdouble centerz,
	GLdouble upx, GLdouble upy, GLdouble upz)
{
	GLdouble m[16];
	GLdouble x[3], y[3], z[3];
	GLdouble mag;
	/* Make rotation matrix */
	/* Z vector */
	z[0] = eyex - centerx;
	z[1] = eyey - centery;
	z[2] = eyez - centerz;
	mag = sqrt(z[0] * z[0] + z[1] * z[1] + z[2] * z[2]);
	if (mag) {           /* mpichler, 19950515 */
		z[0] /= mag;
		z[1] /= mag;
		z[2] /= mag;
	}
										   /* Y vector */
	y[0] = upx;
	y[1] = upy;
	y[2] = upz;
	/* X vector = Y cross Z */
	x[0] = y[1] * z[2] - y[2] * z[1];
	x[1] = -y[0] * z[2] + y[2] * z[0];
	x[2] = y[0] * z[1] - y[1] * z[0];
	/* Recompute Y = Z cross X */
	y[0] = z[1] * x[2] - z[2] * x[1];
	y[1] = -z[0] * x[2] + z[2] * x[0];
	y[2] = z[0] * x[1] - z[1] * x[0];
	/* mpichler, 19950515 */
	/* cross product gives area of parallelogram, which is < 1.0 for
	*     * non-perpendicular unit-length vectors; so normalize x, y here
	*      */
	mag = sqrt(x[0] * x[0] + x[1] * x[1] + x[2] * x[2]);
	if (mag) {
		x[0] /= mag;
		x[1] /= mag;
		x[2] /= mag;
	}
	mag = sqrt(y[0] * y[0] + y[1] * y[1] + y[2] * y[2]);
	if (mag) {
		y[0] /= mag;
		y[1] /= mag;
		y[2] /= mag;
	}
#define M(row,col)  m[col*4+row]
	M(0, 0) = x[0];
	M(0, 1) = x[1];
	M(0, 2) = x[2];
	M(0, 3) = 0.0;
	M(1, 0) = y[0];
	M(1, 1) = y[1];
	M(1, 2) = y[2];
	M(1, 3) = 0.0;
	M(2, 0) = z[0];
	M(2, 1) = z[1];
	M(2, 2) = z[2];
	M(2, 3) = 0.0;
	M(3, 0) = 0.0;
	M(3, 1) = 0.0;
	M(3, 2) = 0.0;
	M(3, 3) = 1.0;
#undef M
	glMultMatrixd(m);
	/* Translate Eye to Origin */
	glTranslated(-eyex, -eyey, -eyez);
}


