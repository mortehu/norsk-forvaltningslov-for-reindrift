#include <stdio.h>
#include <iostream>
#include <algorithm>
#include <unistd.h>
#include <GL/gl.h>

#include <state.h>
#include <matrix.h>
#include <font.h>
#include <states/mushroom.h>

namespace cls
{
	mushroom::mushroom(const char* p_Name) : state(p_Name)
	{
	}

	void mushroom::on_enter(void* p_Parameter)
	{
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glDisable(GL_DEPTH_TEST);
		glEnable(GL_BLEND);
		glBlendFunc(GL_ONE, GL_ONE);
		glEnable(GL_TEXTURE_2D);
	}

	void mushroom::on_leave()
	{
	}

	void mushroom::on_display()
	{
		size_t l_Time = get_time();

		if(l_Time > BEAT * 38)
			state::leave();

		glClear(GL_COLOR_BUFFER_BIT);

		if(l_Time > BEAT * 32)
			return;

		glColor3f(0.8, 1.0, 0.8);

		font::put_text(
			vector2(1 - (l_Time / (BEAT * 32)) * (161 * 0.4 + 2), 0.2), 
			vector2(1 + 161 * 0.4 - (l_Time / (BEAT * 32)) * (161 * 0.4 + 2), -0.2), 
			"You have been watching fadeout's contribution to the TG 2002 demo competation. We ran out of time making this, so the final effect is just this unfancy scroller");
	}
}
 
