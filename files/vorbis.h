#ifndef _VORBIS_H_
#define _VORBIS_H_

#include <vorbis/vorbisfile.h>

#include <sound.h>

namespace vorbis
{
	class sound : public cls::sound
	{
	private:
		OggVorbis_File m_File;
		int            m_CurrentSection;

		int m_Whoa;

	public:
		sound(const char* p_FileName);
		~sound();
		
		size_t get_data(const size_t p_Size, void* r_Data);
	};
};

#endif // !_VORBIS_H_

