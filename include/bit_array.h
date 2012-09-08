#ifndef _BIT_ARRAY_H_
#define _BIT_ARRAY_H_

#include <common.h>

namespace cls
{
	class bit_array
	{
	public:
		size_t   m_Size;
		uint8_t* m_Data;

		bit_array(size_t p_Size)
		{
			m_Size = (p_Size + 7) / 8;
			m_Data = new uint8_t[m_Size];
			memset(m_Data, 0, m_Size);
		}

		~bit_array()
		{
			delete [] m_Data;
		}

		uint8_t get(const size_t p_Index) const
		{
			return (m_Data[p_Index / 8] >> (p_Index & 7)) & 1;
		}

		void set(const size_t p_Index, const uint8_t p_Value)
		{
			if(p_Value)
				m_Data[p_Index / 8] |= 1 << (p_Index & 7);
			else
				m_Data[p_Index / 8] &= ~(1 << (p_Index & 7));
		}
	};
}

#endif

