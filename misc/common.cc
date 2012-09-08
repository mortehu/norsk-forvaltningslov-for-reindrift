#include <string>
#include <string.h>
#include <errno.h>

#include <common.h>

namespace cls
{
	std::string system_error()
	{
		return std::string(strerror(errno));
	}
};

