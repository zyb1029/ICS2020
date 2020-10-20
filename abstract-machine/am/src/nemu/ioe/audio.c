#include <am.h>
#include <nemu.h>

#define AUDIO_FREQ_ADDR      (AUDIO_ADDR + 0x00)
#define AUDIO_CHANNELS_ADDR  (AUDIO_ADDR + 0x04)
#define AUDIO_SAMPLES_ADDR   (AUDIO_ADDR + 0x08)
#define AUDIO_SBUF_SIZE_ADDR (AUDIO_ADDR + 0x0c)
#define AUDIO_INIT_ADDR      (AUDIO_ADDR + 0x10)
#define AUDIO_COUNT_ADDR     (AUDIO_ADDR + 0x14)
#define AUDIO_HEAD_ADDR      (AUDIO_ADDR + 0x18)
#define AUDIO_TAIL_ADDR      (AUDIO_ADDR + 0x1c)
static int SBUF_SIZE_MAX;

void __am_audio_config(AM_AUDIO_CONFIG_T *cfg) {
  cfg -> present = true;
  SBUF_SIZE_MAX =  inl(AUDIO_SBUF_SIZE_ADDR);
  cfg -> bufsize = SBUF_SIZE_MAX;
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
	int count = inl(AUDIO_COUNT_ADDR);
	while(SBUF_SIZE_MAX - count < len) {
		count = inl(AUDIO_COUNT_ADDR);	
	}
	int nwrite = len;
	int head = inl(AUDIO_HEAD_ADDR), tail = inl(AUDIO_TAIL_ADDR);
	uint8_t *sbuf = (uint8_t *)(uintptr_t)AUDIO_SBUF_ADDR;
	uint8_t *buf = ctl->buf.start;
	if (nwrite + head < SBUF_SIZE_MAX) {
		for (int i = 0; i < nwrite; i++)
			sbuf[head + i] = buf[i];
		head += nwrite;
	}
	else {
		int first_cpy_len = SBUF_SIZE_MAX - head;
		for (int i = 0; i < first_cpy_len; i++)
			sbuf[head + i] = buf[i];
		for (int i = 0; i < nwrite - first_cpy_len; i++)
			sbuf[i] = buf[first_cpy_len + i];
		head = nwrite - first_cpy_len;		
	}
	outl(AUDIO_HEAD_ADDR, head);
	outl(AUDIO_TAIL_ADDR, tail);
	outl(AUDIO_COUNT_ADDR, count + nwrite);
}
