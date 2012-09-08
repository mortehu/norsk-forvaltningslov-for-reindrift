#include <iostream>
#include <stdexcept>
#include <string>
#include <string.h>
#include <fcntl.h>
#include <stdlib.h>
#include <limits.h>
#include <alsa/asoundlib.h>
#include <err.h>

#include <common.h>
#include <sound.h>

namespace cls
{
  static snd_pcm_t* playback_handle = 0;
  static snd_pcm_sw_params_t* sw_params = 0;
  static snd_pcm_hw_params_t* hw_params = 0;
  static snd_pcm_sframes_t buffer_size = 0;

  static snd_pcm_sframes_t written = 0;
  static snd_pcm_sframes_t delay = 0;

  static unsigned int rate = 44100;

  char*  sound::s_Buffer;
  size_t sound::s_BufferSize;
  int    sound::s_FileDescriptor = -1;
  bool   sound::s_EOF;
  sound* sound::s_Stream = NULL;

  void sound::init()
  {
    unsigned int buffer_time = 50000;
    const char* device;
    int err;

    device = getenv("ALSA_DEVICE");

    if(!device)
      device = "default";

    if(0 > (err = snd_pcm_open(&playback_handle, device,
            SND_PCM_STREAM_PLAYBACK, 0/*SND_PCM_NONBLOCK*/)))
      errx(EXIT_FAILURE, "Audio: Cannot open device %s: %s", device, snd_strerror(err));

    if(0 > (err = snd_pcm_sw_params_malloc(&sw_params)))
      errx(EXIT_FAILURE, "Audio: Could not allocate software parameter structure: %s",
          snd_strerror(err));

    if(0 > (err = snd_pcm_hw_params_malloc(&hw_params)))
      errx(EXIT_FAILURE, "Audio: Could not allocate hardware parameter structure: %s",
          snd_strerror(err));

    if(0 > (err = snd_pcm_hw_params_any(playback_handle, hw_params)))
      errx(EXIT_FAILURE, "Audio: Could not initializa hardware parameters: %s",
          snd_strerror(err));

    if(0 > (err = snd_pcm_hw_params_set_access(playback_handle, hw_params,
            SND_PCM_ACCESS_RW_INTERLEAVED)))
      errx(EXIT_FAILURE, "Audio: Could not set access type: %s", snd_strerror(err));

    if(0 > (err = snd_pcm_hw_params_set_format(playback_handle, hw_params,
            SND_PCM_FORMAT_S16)))
      errx(EXIT_FAILURE, "Audio: Could not set sample format to signed 16 bit "
          "native endian: %s", snd_strerror(err));

    if(0 > (err = snd_pcm_hw_params_set_rate_near(playback_handle, hw_params,
            &rate, 0)))
      errx(EXIT_FAILURE, "Audio: Could not set sample rate %uHz: %s", rate,
          snd_strerror(err));

    if(0 > (err = snd_pcm_hw_params_set_channels(playback_handle, hw_params, 2)))
      errx(EXIT_FAILURE, "Audio: Could not set channel count to %u: %s",
          2, snd_strerror(err));

    snd_pcm_hw_params_set_buffer_time_near(playback_handle, hw_params, &buffer_time, 0);

    if(0 > (err = snd_pcm_hw_params(playback_handle, hw_params)))
      errx(EXIT_FAILURE, "Audio: Could not set hardware parameters: %s", snd_strerror(err));

    fprintf(stderr, "Buffer time is %.3f seconds\n", buffer_time / 1.0e6);

    if(0 > (err = snd_pcm_sw_params_current(playback_handle, sw_params)))
      errx(EXIT_FAILURE, "Audio: Could not initialize software parameters: %s",
          snd_strerror(err));

    snd_pcm_sw_params_set_start_threshold(playback_handle, sw_params, 0);
    snd_pcm_sw_params_set_avail_min(playback_handle, sw_params, 1024);

    snd_pcm_uframes_t min;
    snd_pcm_sw_params_get_avail_min(sw_params, &min);
    fprintf(stderr, "Minimum %u\n", (unsigned) min);

    if(0 > (err = snd_pcm_sw_params(playback_handle, sw_params)))
      errx(EXIT_FAILURE, "Audio: Could not set software parameters: %s",
          snd_strerror(err));

    buffer_size = snd_pcm_avail_update(playback_handle);
  }

  void sound::stream_open(sound& p_Sound)
  {
    s_Stream = &p_Sound;
  }

  void sound::stream_update()
  {
    size_t avail, amount;
    int err;
    char *data;

    if (!s_Stream)
      return;

    avail = snd_pcm_avail (playback_handle) * 4;

    if (!avail)
      return;

    data = new char[avail];

    amount = s_Stream->get_data(avail, data);

    if(amount < avail)
      s_EOF = true;

    err = snd_pcm_writei (playback_handle, data, amount / 4);

    free (data);

    written += err;
  }

  size_t sound::stream_position()
  {
    snd_pcm_delay(playback_handle, &delay);

    return (written - delay) * 1000 / 44100;
  }

  void sound::stream_close()
  {
  }

  sound::sound()
  {
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

