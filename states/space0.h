#ifndef _SPACE0_H_
#define _SPACE0_H_

#include <state.h>
#include <geom.h>

namespace cls
{
	class image;

	class space0 : public state
	{
	private:
		image  m_Planet;
		image  m_Asteroid;
		image  m_Sun;
		image  m_Stars;
		image  m_Flares[5];
		vertex_geom* m_Sphere;


	public:
		space0(const char* p_Name);

		void on_enter(void* p_Parameter);
		void on_leave();
		void on_display();
	};
}

#endif

