#include <GL/gl.h>

#include <particle.h>
#include <image.h>
#include <geom.h>
#include <primitives.h>

namespace cls
{
	image particle::s_Texture;

	void particle::init()
	{
		uint8_t* l_Data = new uint8_t[32 * 32 * 3];

		for(size_t l_Y = 0; l_Y < 32; l_Y++)
		{
			for(size_t l_X = 0; l_X < 32; l_X++)
			{
				l_Data[(l_Y * 32 + l_X) * 3] = 
				l_Data[(l_Y * 32 + l_X) * 3 + 1] = 
				l_Data[(l_Y * 32 + l_X) * 3 + 2] = 
					(uint8_t) (pow((sin(l_Y / 31.0 * M_PI) + sin(l_X / 31.0 * M_PI)) * 0.5, 10) * 255);
			}
		}

		s_Texture.set_repeat(false, false);
		s_Texture.set_pixel_format(image::RGB);
		s_Texture.set_geometry(32, 32);
		s_Texture.set_data(l_Data);
	}

	void particle::set_state(bool p_Activate)
	{
		if(p_Activate)
		{
			glEnable(GL_BLEND);
			glBlendFunc(GL_ONE, GL_ONE);
			glActiveTextureARB(GL_TEXTURE0_ARB);
			glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
			glEnable(GL_TEXTURE_2D);
			s_Texture.activate();
			glEnable(GL_DEPTH_TEST);
			glDepthMask(GL_FALSE);
			glBegin(GL_QUADS);
		}
		else
		{
			glEnd();
			glDisable(GL_BLEND);
			glDisable(GL_TEXTURE_2D);
			glDepthMask(GL_TRUE);
			glDisable(GL_DEPTH_TEST);
		}
	}

	void particle::render(const camera& p_Camera)
	{
		vector3 l_Up   = p_Camera.get_up()   * m_Scale;
		vector3 l_Left = p_Camera.get_left() * m_Scale;

		glColor3ub(m_Color.m_Red, m_Color.m_Green, m_Color.m_Blue);

		glTexCoord2f(0, 0);
		glVertex3fv(((*this) + l_Up + l_Left).get_data());
		glTexCoord2f(0, 1);
		glVertex3fv(((*this) - l_Up + l_Left).get_data());
		glTexCoord2f(1, 1);
		glVertex3fv(((*this) - l_Up - l_Left).get_data());
		glTexCoord2f(1, 0);
		glVertex3fv(((*this) + l_Up - l_Left).get_data());
	}
};

