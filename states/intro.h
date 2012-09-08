#ifndef _INTRO_H_
#define _INTRO_H_

#include <state.h>
#include <geom.h>

namespace cls
{
	class intro : public state
	{
	private:
		geom*   m_Geometry;

	public:
		intro(const char* p_Name, geom* p_Geometry);

		void on_enter(void* p_Parameter);
		void on_leave();
		void on_display();
	};
}

#endif

