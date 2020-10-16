#ifndef __RTL_PSEUDO_H__
#define __RTL_PSEUDO_H__

#ifndef __RTL_RTL_H__
#error "Should be only included by <rtl/rtl.h>"
#endif

/* RTL pseudo instructions */

static inline def_rtl(li, rtlreg_t* dest, const rtlreg_t imm) {
  rtl_addi(s, dest, rz, imm);
}

static inline def_rtl(mv, rtlreg_t* dest, const rtlreg_t *src1) {
  if (dest != src1) rtl_add(s, dest, src1, rz);
}

static inline def_rtl(not, rtlreg_t *dest, const rtlreg_t* src1) {
  // dest <- ~src1
  TODO();
}

static inline def_rtl(neg, rtlreg_t *dest, const rtlreg_t* src1) {
	*dest = *src1 * (-1);  
}

static inline def_rtl(sext, rtlreg_t* dest, const rtlreg_t* src1, int width) {
  assert(width == 1 || width == 2 || width ==4);
  if (width == 1) *dest = (int8_t)*src1;
  else if (width == 2) *dest = (int16_t)*src1;
  else *dest = (int32_t)*src1;
}

static inline def_rtl(zext, rtlreg_t* dest, const rtlreg_t* src1, int width) {
  // dest <- zeroext(src1[(width * 8 - 1) .. 0])
  TODO();
}

static inline def_rtl(msb, rtlreg_t* dest, const rtlreg_t* src1, int width) {
  // dest <- src1[width * 8 - 1]
  TODO();
}

#endif
