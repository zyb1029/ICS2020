#include <common.h>

#ifdef HAS_IOE

#include <device/map.h>
#include <SDL2/SDL.h>

#define AUDIO_PORT 0x200 // Note that this is not the standard
#define AUDIO_MMIO 0xa1000200
#define STREAM_BUF 0xa0800000
#define STREAM_BUF_MAX_SIZE 65536


enum {
  reg_freq,
  reg_channels,
  reg_samples,
  reg_sbuf_size,
  reg_init,
  reg_count,
  nr_reg
};

static uint8_t *sbuf = NULL;
static uint32_t *audio_base = NULL;
static uint32_t count = 0;
static uint32_t tail = 0;
static inline void audio_play(void *userdata, uint8_t *stream, int len) {
	int nread = len;
	if (count < len) nread = count;
	
	if (nread + tail < STREAM_BUF_MAX_SIZE) {
		memcpy(stream, sbuf + tail, nread);
		tail += nread;	
	}
	else {
		int first_cpy_len = STREAM_BUF_MAX_SIZE - tail;
		memcpy(stream, sbuf + tail, first_cpy_len);
		memcpy(stream + first_cpy_len, sbuf, nread - first_cpy_len);
		tail = nread - first_cpy_len;
	}
	count -= nread;
	if (len > nread) {
		memset(stream + nread, 0, len - nread);
	}
}

SDL_AudioSpec s = {};

static void audio_io_handler(uint32_t offset, int len, bool is_write) {
	switch (offset){
		case 0:
			break;
		case 4:
			break;
		case 8:
			break;
		case 12:
			audio_base[reg_sbuf_size] = STREAM_BUF_MAX_SIZE;
			break;
		case 16:
			s.format = AUDIO_S16SYS;
			s.userdata = NULL;
			s.channels = audio_base[reg_channels];
			s.samples = audio_base[reg_samples];
			s.freq = audio_base[reg_freq];
			s.callback = audio_play;
		    int ret	= SDL_InitSubSystem(SDL_INIT_AUDIO);
			if(ret == 0) {
				SDL_OpenAudio(&s, NULL);
				SDL_PauseAudio(0);
			}
			count = 0;
			tail = 0;
			break;
		case 20: 
			if(is_write) count = audio_base[reg_count];
			else audio_base[reg_count] = count;
			break;
		default: TODO();
	}
	 
}

void init_audio() {
  uint32_t space_size = sizeof(uint32_t) * nr_reg;
  audio_base = (void *)new_space(space_size);
  add_pio_map("audio", AUDIO_PORT, (void *)audio_base, space_size, audio_io_handler);
  add_mmio_map("audio", AUDIO_MMIO, (void *)audio_base, space_size, audio_io_handler);

  sbuf = (void *)new_space(STREAM_BUF_MAX_SIZE);
  add_mmio_map("audio-sbuf", STREAM_BUF, (void *)sbuf, STREAM_BUF_MAX_SIZE, NULL);

}
#endif	/* HAS_IOE */
