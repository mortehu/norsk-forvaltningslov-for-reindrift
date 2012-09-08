#ifdef WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#else
#include <sys/time.h>
#endif
#include <iostream>
#include <profiler.h>

namespace cls
{
	std::map<string, double> profiler::s_Time;

	void profiler::print_stats()
	{
		std::map<string, double>::iterator i_Item;

		for(i_Item = s_Time.begin(); i_Item != s_Time.end(); i_Item++)
		{
			std::cerr << i_Item->first << ":\t" << i_Item->second << std::endl;
		}
	}

	profiler::profiler(string p_Name)
	{
		m_Current = p_Name;

#ifdef WIN32
		m_StartTime = GetTickCount() / 1000.0;
#else
		// XXX: Won't handle midnight properly (gettimeofday() flips over)

		struct timeval l_TimeOfDay;
		gettimeofday(&l_TimeOfDay, NULL);

		m_StartTime =
			l_TimeOfDay.tv_sec + (l_TimeOfDay.tv_usec / 1000000.0);
#endif
	}

	profiler::~profiler()
	{
#ifdef WIN32
		s_Time[m_Current] += GetTickCount() / 1000.0 - m_StartTime;
#else
		struct timeval l_TimeOfDay;
		gettimeofday(&l_TimeOfDay, NULL);
		
		s_Time[m_Current] +=
			(l_TimeOfDay.tv_sec + (l_TimeOfDay.tv_usec / 1000000.0))
		  - m_StartTime;
#endif
	}
};

