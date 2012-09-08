#ifndef _PARTICLE_H_
#define _PARTICLE_H_

#include <vector.h>
#include <color.h>

namespace cls
{
	class image;
	class camera;

	class particle : public vector3
	{
	private:
		static image s_Texture;

	public:
		color m_Color;
		float m_Scale;

		static void init();

		particle() { }

		particle(const vector3& p_Position, const color& p_Color, const float p_Scale = 2.0)
		{
			(*this)(0) = p_Position(0);
			(*this)(1) = p_Position(1);
			(*this)(2) = p_Position(2);
			m_Color = p_Color;
			m_Scale = p_Scale;
		}

		void set_state(bool p_Activate = true);
		void render(const camera& p_Camera);
	};
};

#endif

