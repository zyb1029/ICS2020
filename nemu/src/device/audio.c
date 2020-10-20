#include <common.h>

#ifdef HAS_IOE

#include <device/map.h>
#include <SDL2/SDL.h>

#define AUDIO_PORT 0x200 // Note that this is not the standard
#define AUDIO_MMIO 0xa1000200
#define STREAM_BUF 0xa0800000
#define STREAM_BUF_MAX_SIZE 65536

SDL_AudioSpec s = {};

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
static inline void audio_play(void *userdata, uint8_t *stream, int len) {
}

static void audio_io_handler(uint32_t offset, int len, bool is_write) {
	switch (offset){
		case 0:
			s.freq = audio_base[0];
			break;
		case 1:
			s.channels = audio_base[1];
			break;
		case 2:
			s.samples = audio_base[2];
			break;
		case 3:
			audio_base[3] = STREAM_BUF_MAX_SIZE;
			break;
		case 4:
			s.format = AUDIO_S16SYS;
			s.userdata = NULL;
			s.callback = audio_play;
			SDL_InitSubSystem(SDL_INIT_AUDIO);
			SDL_OpenAudio(&s, 0);
			SDL_PauseAudio(0);
			break;
		case 5: 
			audio_base[5] = count;
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
