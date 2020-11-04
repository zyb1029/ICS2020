#include <proc.h>
#include <elf.h>
#include <common.h>

#ifdef __LP64__
# define Elf_Ehdr Elf64_Ehdr
# define Elf_Phdr Elf64_Phdr
#else
# define Elf_Ehdr Elf32_Ehdr
# define Elf_Phdr Elf32_Phdr
#endif

static uintptr_t loader(PCB *pcb, const char *filename) {
  Elf_Ehdr elf_head;
  ramdisk_read(&elf_head, 0, 1);
//  Elf_Phdr *phdr = (Elf_Phdr *)malloc(sizeof(Elf_Phdr) * elf_head.e_phnum);
  printf("%d\n", elf_head.e_phnum);
  return 0;
}

void naive_uload(PCB *pcb, const char *filename) {
  loader(NULL, NULL);return;
  uintptr_t entry = loader(pcb, filename);
  Log("Jump to entry = %p", entry);
  ((void(*)())entry) ();
}

