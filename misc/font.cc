#include <stdlib.h>
#include <string.h>
#include <GL/gl.h>

#include <font.h>
#include <image.h>
#include <primitives.h>

namespace cls
{
	image       font::s_Font;
	const char* font::s_Chars;
	size_t      font::s_FontWidth;
	vector2     font::s_Scale;
	vector2     font::s_Offset;

	void font::load(
		const char* p_FileName, 
		const char* p_Chars, 
		const size_t p_FontWidth, 
		const vector2& p_Scale, 
		const vector2& p_Offset)
	{
		std::cerr << "Loading font " << p_FileName << "..." << std::endl;

		s_Font.load(p_FileName);
		s_Chars = p_Chars;
		s_FontWidth = p_FontWidth;
		s_Scale = p_Scale;
		s_Offset = p_Offset;
	}

	void font::put_text(
		const vector2& p_UpperLeft, 
		const vector2& p_LowerRight, 
		const char* p_Text)
	{
		s_Font.activate();

		vector2 l_Position = p_UpperLeft;
		vector2 l_Step(  
			(p_LowerRight(0) - p_UpperLeft(0)) / (float) strlen(p_Text), 
			p_LowerRight(1) - p_UpperLeft(1));
			

		const char* l_Char = p_Text;

		glDisable(GL_CULL_FACE);
		glActiveTextureARB(GL_TEXTURE0_ARB);
		glEnable(GL_TEXTURE_2D);
		glBegin(GL_QUADS);

		while(*l_Char)
		{
			size_t l_Offset = 0;

			if(strchr(s_Chars, *l_Char))
				l_Offset = strchr(s_Chars, *l_Char) - s_Chars;

			vector2 l_TextureCoord(
				(l_Offset % s_FontWidth) * s_Scale(0) + s_Offset(0),  
				(l_Offset / s_FontWidth) * s_Scale(1) + s_Offset(1));

			draw_billboard_texture(
				l_Position,          l_TextureCoord, 
				l_Position + l_Step, l_TextureCoord + s_Scale);

			/*
			glTexCoord2fv(l_TextureCoord.get_data());
			glVertex2fv(l_Position.get_data());

			glTexCoord2f(l_TextureCoord(0),              l_TextureCoord(1) + s_Scale(1));
			glVertex2f(l_Position(0),                    l_Position(1) + l_Step(1));

			glTexCoord2f(l_TextureCoord(0) + s_Scale(0), l_TextureCoord(1) + s_Scale(1));
			glVertex2f(l_Position(0) + l_Step(0),        l_Position(1) + l_Step(1));

			glTexCoord2f(l_TextureCoord(0) + s_Scale(0), l_TextureCoord(1));
			glVertex2f(l_Position(0) + l_Step(0),        l_Position(1));
			*/
			l_Position(0) += l_Step(0);
			l_Char++;
		}

		glEnd();
	}
};

