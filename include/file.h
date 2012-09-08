#ifndef _FILE_H_
#define _FILE_H_

#include <string>

using namespace std;

namespace cls
{
	// For simplicity an instance of an archive handler can only
	// open one file in one archive at a time

	class archive_handler
	{
	public:
		virtual void    open_archive(const char* p_FileName);
		virtual void    close_archive();

		virtual void    open_file(const char* p_FileName);
		virtual void    close_file();
		virtual size_t  bytes_remaining();
		virtual void    read(void* p_Buffer, const size_t p_Length);

		virtual string::iterator file_list();
	};
};

#endif
