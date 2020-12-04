#include <am.h>
#include <nemu.h>

static long boot_time_usec;
static long boot_time_sec;

void __am_timer_init() {
	boot_time_usec = inl(RTC_ADDR);
	boot_time_sec = inl(RTC_ADDR + 4);
}

void __am_timer_uptime(AM_TIMER_UPTIME_T *uptime) {
  long useconds = inl(RTC_ADDR) - boot_time_usec;
  long seconds = inl(RTC_ADDR + 4) - boot_time_sec;
  uptime->us = seconds * 1000000 + (useconds + 500);
  printf("%d\n", uptime-> us);
}

void __am_timer_rtc(AM_TIMER_RTC_T *rtc) {
  rtc->second = 0;
  rtc->minute = 0;
  rtc->hour   = 0;
  rtc->day    = 0;
  rtc->month  = 0;
  rtc->year   = 1900;
}
