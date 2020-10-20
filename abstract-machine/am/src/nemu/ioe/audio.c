#include <am.h>
#include <nemu.h>

#define AUDIO_FREQ_ADDR      (AUDIO_ADDR + 0x00)
#define AUDIO_CHANNELS_ADDR  (AUDIO_ADDR + 0x04)
#define AUDIO_SAMPLES_ADDR   (AUDIO_ADDR + 0x08)
#define AUDIO_SBUF_SIZE_ADDR (AUDIO_ADDR + 0x0c)
#define AUDIO_INIT_ADDR      (AUDIO_ADDR + 0x10)
#define AUDIO_COUNT_ADDR     (AUDIO_ADDR + 0x14)

static int SBUF_SIZE_MAX;

void __am_audio_config(AM_AUDIO_CONFIG_T *cfg) {
  cfg -> present = true;
  SUB_SIZE_MAX =  inl(AUDIO_SBUF_SIZE_ADDR);
  cfg -> bufsize = SUB_SIZE_MAX;
}

void __am_audio_ctrl(AM_AUDIO_CTRL_T *ctrl) {
	outl(AUDIO_FREQ_ADDR, ctrl -> freq);
	outl(AUDIO_SAMPLES_ADDR, ctrl -> samples);
	outl(AUDIO_CHANNELS_ADDR, ctrl -> channels);
	outl(AUDIO_INIT_ADDR, 1);
}

void __am_audio_status(AM_AUDIO_STATUS_T *stat) {
  stat->count = inl(AUDIO_COUNT_ADDR);
}

void __am_audio_play(AM_AUDIO_PLAY_T *ctl) {
	int len = ctl->buf.end - ctl->buf.start;
	while(SUBF_SIZE_MAX - count < len);
	int nwrite = len;

}
