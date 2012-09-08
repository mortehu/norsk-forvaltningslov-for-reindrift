#ifndef _SWAP_H_
#define _SWAP_H_

#ifdef WIN32

template<class T>
inline static void byte_swap(T& r_Value) { }

#define array_swap(x)

#else

#include <endian.h>
#include <stdlib.h>
#include <common.h>

template<class T>
inline static void byte_swap(T& r_Value)
{
#if __BYTE_ORDER == __BIG_ENDIAN
	if(sizeof(T) == 2)
	{
		uint16_t l_Temp = *((uint16_t*) &r_Value);

		l_Temp = ((uint16_t(l_Temp) & 0xFF00) >> 8)
		       | ((uint16_t(l_Temp) & 0x00FF) << 8);

		r_Value = *((T*) &l_Temp);
	}
	else if(sizeof(T) == 4)
	{
		uint32_t l_Temp = *((uint32_t*) &r_Value);

		l_Temp = ((uint32_t(l_Temp) & 0xFF000000UL) >> 24)
		       | ((uint32_t(l_Temp) & 0x00FF0000UL) >> 8)
			   | ((uint32_t(l_Temp) & 0x0000FF00UL) << 8)
			   | ((uint32_t(l_Temp) & 0x000000FFUL) << 24);

		r_Value = *((T*) &l_Temp);
	}
#elif __BYTE_ORDER == __PDP_ENDIAN
	if(sizeof(T) == 2)
	{
		uint16_t l_Temp = *((uint16_t*) &r_Value);

		l_Temp = ((uint16_t(l_Temp) & 0xFF00) >> 8)
		       | ((uint16_t(l_Temp) & 0x00FF) << 8);

		r_Value = *((T*) &l_Temp);
	}
	else if(sizeof(T) == 4)
	{
		uint32_t l_Temp = *((uint32_t*) &r_Value);

		l_Temp = ((uint32_t(l_Temp) & 0xFFFF0000UL) >> 16)
			   | ((uint32_t(l_Temp) & 0x0000FFFFUL) << 16);

		r_Value = *((T*) &l_Temp);
	}
#endif
}

#if __BYTE_ORDER != __LITLE_ENDIAN
#	define array_swap(p_Array) \
	do { \
		for(size_t l_Index = 0; l_Index < sizeof(p_Array) / sizeof(p_Array[0]); l_Index++) \
			byte_swap(p_Array[l_Index]); \
	} while(0)
#else
#error "her"
#	define array_swap(p_Array)
#endif
	
#endif // !WIN32

#endif // !_SWAP_H_

