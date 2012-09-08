#ifndef _FONT_H_
#define _FONT_H_

#include <stdlib.h>

#include <image.h>
#include <vector.h>

namespace cls
{
	class font
	{
	private:
		static image       s_Font;
		static const char* s_Chars;
		static size_t      s_FontWidth;
		static vector2     s_Scale;
		static vector2     s_Offset;
	
	public:
		static void load(
			const char* p_FileName, 
			const char* p_Chars, 
			const size_t p_FontWidth, 
			const vector2& p_Scale, 
			const vector2& p_Offset);

		static void put_text(
			const vector2& p_UpperLeft, 
			const vector2& p_LowerRight, 
			const char* p_Text);
	};
};

#endif

