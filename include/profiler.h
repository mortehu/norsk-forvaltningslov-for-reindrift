#ifndef _PROFILER_H_
#define _PROFILER_H_

#include <map>
#include <fstream>
#include <string>

using namespace std;

#define PROFILE(name) cls::profiler __profile(name)

namespace cls
{
	class profiler
	{
	private:
		static  map<string, double> s_Time;
		
		string  m_Current;
		double  m_StartTime;

	public:
		static void print_stats();

		profiler(string p_Name);
		~profiler();
	};
};

#endif

