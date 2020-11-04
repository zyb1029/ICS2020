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
  ramdisk_read(&elf_head, 0, sizeof(Elf_Ehdr));
  Elf_Phdr *phdr = (Elf_Phdr *)malloc(sizeof(Elf_Phdr) * elf_head.e_phnum);
  ramdisk_read(phdr, elf_head.e_phoff, sizeof(Elf_Phdr) * elf_head.e_phnum);
  for (int i = 0; i < elf_head.e_phnum; i++){
	 uint32_t type = phdr[i].p_type; 
     uintptr_t VirtAddr = phdr[i].p_vaddr;
	 size_t FileSiz = phdr[i].p_filesz , Memsiz = phdr[i].p_memsz;
	 size_t offset = phdr[i].p_offset;
	 printf("%08x %08x %08x %08x %08x\n", type, offset, VirtAddr, FileSiz, Memsiz); 
  }
  return 0;
}

void naive_uload(PCB *pcb, const char *filename) {
  loader(NULL, NULL);return;
  uintptr_t entry = loader(pcb, filename);
  Log("Jump to entry = %p", entry);
  ((void(*)())entry) ();
}

