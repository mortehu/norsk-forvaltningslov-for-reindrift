#ifndef _MUSHROOM_H_
#define _MUSHROOM_H_

#include <state.h>

namespace cls
{
	class mushroom : public state
	{
	public:
		mushroom(const char* p_Name);

		void on_enter(void* p_Parameter);
		void on_leave();
		void on_display();
	};
}

#endif

