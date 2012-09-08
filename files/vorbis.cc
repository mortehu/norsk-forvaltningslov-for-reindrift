#include <stdexcept>
#include <stdio.h>
#include <stdlib.h>
#include <vorbis/codec.h>
#include <vorbis/vorbisfile.h>

#include <common.h>
#include <files/vorbis.h>

namespace vorbis
{
	sound::sound(const char* p_FileName)
	{
		FILE* l_File = fopen(p_FileName, "rb");

		if(!l_File)
			throw std::runtime_error(cls::system_error());

		if(ov_open(l_File, &m_File, NULL, 0) < 0)
		{
			fclose(l_File);

			throw std::runtime_error("File isn't an Ogg Vorbis bitstream");
		}

		vorbis_info* l_Info = ov_info(&m_File, -1);

		m_SampleBitCount = 16;
		m_SampleRate = l_Info->rate;
		m_ChannelCount = l_Info->channels;
		m_BigEndian = false;
		m_CurrentSection = 0;
	}

	sound::~sound()
	{
		ov_clear(&m_File);
	}

	size_t sound::get_data(const size_t p_Size, void* r_Data)
	{
		size_t l_Offset = 0;

		while(l_Offset < p_Size)
		{
			int l_RetVal;

			l_RetVal = ov_read(
				&m_File,
				(char*) r_Data + l_Offset,
				p_Size - l_Offset,
				0,
				2,
				1,
				&m_CurrentSection);

			if(l_RetVal == 0)
			{
				break;
			}
			if(l_RetVal < 0)
			{
				// XXX: handle error

				break;
			}

			l_Offset += l_RetVal;
		}

		return l_Offset;
	}
};
