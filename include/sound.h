#ifndef _SOUND_H_
#define _SOUND_H_

#include <stdlib.h>

namespace cls
{
	// *** Unmutable sound class

	class sound
	{
	protected:
		static char*  s_Buffer;
		static size_t s_BufferSize;
		static size_t s_Offset;
		static int    s_FileDescriptor;
		static bool   s_EOF;
		static sound* s_Stream;

		size_t m_SampleBitCount;
		size_t m_SampleRate;
		size_t m_ChannelCount;
		bool   m_BigEndian;

	public:

		// *** Initialize audio

		static void init();

		// *** Determine whether audio device is open

		static bool is_open()
		{
			return (s_FileDescriptor != -1);
		}

		// *** Open audio output stream suitable for playing the 
		//     given sound

		static void stream_open(sound& p_Sound);

		// *** Update stream, do nothing if no stream is currently open

		static void stream_update();

		// *** Return the number of milliseconds of actual output

		static size_t stream_position();

		// *** Close currently open stream

		static void stream_close();

		sound();
		virtual ~sound();

		size_t get_sample_bit_count() const
		{
			return m_SampleBitCount;
		}

		size_t get_sample_rate() const
		{
			return m_SampleRate;
		}

		size_t get_channel_count() const
		{
			return m_ChannelCount;
		}

		bool is_big_endian() const
		{
			return m_BigEndian;
		}

		virtual size_t get_data(const size_t p_Size, void* r_Data);
	};
};

#endif // !_SOUND_H_

