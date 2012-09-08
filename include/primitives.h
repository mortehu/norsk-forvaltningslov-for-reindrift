#ifndef _PRIMITIVES_H_
#define _PRIMITIVES_H_

#include <GL/gl.h>

namespace cls
{
	// *** Caller must call glBegin(); and glEnd(); himself

	static inline void draw_billboard(
		const vector2& p_UpperLeft, 
		const vector2& p_LowerRight)
	{
		glTexCoord2f(0, 0);
		glVertex2fv(p_UpperLeft.get_data());

		glTexCoord2f(0, 1);
		glVertex2f(p_UpperLeft(0), p_LowerRight(1));

		glTexCoord2f(1, 1);
		glVertex2fv(p_LowerRight.get_data());

		glTexCoord2f(1, 0);
		glVertex2f(p_LowerRight(0), p_UpperLeft(1));
	}

	static inline void draw_billboard_texture(
		const vector2& p_UpperLeft, 
		const vector2& p_UpperLeftTexture, 
		const vector2& p_LowerRight, 
		const vector2& p_LowerRightTexture)
	{
		glTexCoord2fv(p_UpperLeftTexture.get_data());
		glVertex2fv(p_UpperLeft.get_data());

		glTexCoord2f(p_UpperLeftTexture(0), p_LowerRightTexture(1));
		glVertex2f(p_UpperLeft(0), p_LowerRight(1));

		glTexCoord2fv(p_LowerRightTexture.get_data());
		glVertex2fv(p_LowerRight.get_data());

		glTexCoord2f(p_LowerRightTexture(0), p_UpperLeftTexture(1));
		glVertex2f(p_LowerRight(0), p_UpperLeft(1));
	}
};

#endif // !_PRIMITIVES_H_

