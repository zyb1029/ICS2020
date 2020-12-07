.section entry, "ax"
.globl _start
.type _start, @function

_start:
  mov $0, %ebp
  mov %eax, %esp
  call _trm_init                 # never return
