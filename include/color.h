#ifndef _COLOR_H_
#define _COLOR_H_

#include <common.h>

namespace cls
{
	class color
	{
	public:
		uint8_t m_Alpha;
		uint8_t m_Red;
		uint8_t m_Green;
		uint8_t m_Blue;

		color() { }

		color(uint8_t p_Red, uint8_t p_Green, uint8_t p_Blue, uint8_t p_Alpha = 255)
		{
			m_Alpha = p_Alpha;
			m_Red = p_Red;
			m_Green = p_Green;
			m_Blue = p_Blue;
		}
	};
};

#endif

