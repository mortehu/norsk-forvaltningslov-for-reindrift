#include <iostream>
#include <stdexcept>
#include <string>
#include <string.h>
#include <fcntl.h>
#include <stdlib.h>
#include <limits.h>
#ifdef WIN32
#else
#	include <sys/ioctl.h>
#	include <sys/soundcard.h>
#	include <unistd.h>
#endif

#include <common.h>
#include <sound.h>

namespace cls
{
	char*  sound::s_Buffer;
	size_t sound::s_BufferSize;
	size_t sound::s_Offset;
	int    sound::s_FileDescriptor = -1;
	bool   sound::s_EOF;
	sound* sound::s_Stream = NULL;

	void sound::init()
	{
		std::cerr << "Opening /dev/dsp...";
		s_FileDescriptor = open("/dev/dsp", O_WRONLY);
		std::cerr << std::endl;

		if(s_FileDescriptor < 0)
		{
			s_FileDescriptor = -1;

			throw std::runtime_error(cls::system_error());
		}
	}

	void sound::stream_open(sound& p_Sound)
	{
#ifdef WIN32
		// XXX
#else
		if(s_FileDescriptor == -1)
			throw std::runtime_error("Audio device not open");
			
		int l_Format;

		// XXX: what about unsigned and mu-/a-law formats?

		if(p_Sound.m_SampleBitCount == 16 && !p_Sound.m_BigEndian)
			l_Format = AFMT_S16_LE;
		else if(p_Sound.m_SampleBitCount == 16 && p_Sound.m_BigEndian)
			l_Format = AFMT_S16_BE;
		else if(p_Sound.m_SampleBitCount == 8)
			l_Format = AFMT_S8;
#ifdef AFMT_S32_LE
		else if(p_Sound.m_SampleBitCount == 32 && !p_Sound.m_BigEndian)
			l_Format = AFMT_S32_LE;
		else if(p_Sound.m_SampleBitCount == 32 && p_Sound.m_BigEndian)
			l_Format = AFMT_S32_BE;
#endif
		else
			throw std::runtime_error("Audio format not supported");

		if(-1 == ioctl(s_FileDescriptor, SNDCTL_DSP_SETFMT, &l_Format))
			throw std::runtime_error( 
				  std::string("Failed to set audio format: ") 
				+ cls::system_error());
		
		int l_ChannelCount = p_Sound.m_ChannelCount;

		if(-1 == ioctl(s_FileDescriptor, SNDCTL_DSP_CHANNELS, &l_ChannelCount))
			throw std::runtime_error( 
				  std::string("Failed to set channel count: ") 
				+ cls::system_error());

		int l_SampleRate = p_Sound.m_SampleRate;

		if(-1 == ioctl(s_FileDescriptor, SNDCTL_DSP_SPEED, &l_SampleRate))
			throw std::runtime_error( 
				  std::string("Failed to set sample rate: ")
				+ cls::system_error());

		s_Stream = &p_Sound;

		struct audio_buf_info l_AudioBufInfo;

		if(-1 == ioctl(s_FileDescriptor, SNDCTL_DSP_GETOSPACE, &l_AudioBufInfo))
			throw std::runtime_error(
				  std::string("Failed to get buffer size: ") 
				+ cls::system_error());

		s_BufferSize = l_AudioBufInfo.bytes;

		s_Buffer = new char[s_BufferSize];

		s_EOF = false;
		s_Offset = 0;
#endif
	}

	void sound::stream_update()
	{
#ifdef WIN32
		// XXX
#else
		if(s_FileDescriptor == -1)
			return;

		struct audio_buf_info l_AudioBufInfo;

		ioctl(s_FileDescriptor, SNDCTL_DSP_GETOSPACE, &l_AudioBufInfo);

		size_t l_Size;

		if(s_EOF)
		{
			memset(s_Buffer, 0, l_AudioBufInfo.bytes);

			l_Size = l_AudioBufInfo.bytes;
		}
		else
		{
			l_Size = s_Stream->get_data(l_AudioBufInfo.bytes, s_Buffer);

			if(l_Size < (size_t) l_AudioBufInfo.bytes)
				s_EOF = true;
		}

		write(s_FileDescriptor, s_Buffer, l_Size);

		s_Offset += l_Size;
#endif
	}

	size_t sound::stream_position()
	{
#ifdef WIN32
		// XXX

		return 0;
#else
		int l_Delay;

		ioctl(s_FileDescriptor, SNDCTL_DSP_GETODELAY, &l_Delay);

		return (size_t) ((uint64_t) (s_Offset - l_Delay) * 1000 / 
			(s_Stream->m_SampleRate 
				* (s_Stream->m_SampleBitCount / CHAR_BIT * s_Stream->m_ChannelCount)));
#endif
	}

	void sound::stream_close()
	{
#ifdef WIN32
		// XXX
#else
		if(s_FileDescriptor != -1)
			close(s_FileDescriptor);

		s_FileDescriptor = -1;
#endif
	}

	sound::sound()
	{
		m_SampleBitCount = 16;
		m_SampleRate = 44100;
		m_ChannelCount = 2;
		m_BigEndian = false;
	}

	sound::~sound()
	{
	}

	size_t sound::get_data(const size_t p_Size, void* r_Data)
	{
		memset(&r_Data, 0, p_Size);

		return p_Size;
	}
};

