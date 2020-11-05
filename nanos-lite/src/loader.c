#include <proc.h>
#include <elf.h>
#include <common.h>

#define Unsupported_ISA 0

#if defined(__ISA_NATIVE__)
#define EXPECT_TYPE EM_X86_64 
#elif defined(__ISA_X86__)
#define EXPECT_TYPE EM_386
#else
#define EXPECT_TYPE  EM_X86_64 
#endif


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
   if (elf_head.e_machine != EXPECT_TYPE) {
	uint32_t tep = EXPECT_TYPE;   
	printf("%08x %08x\n", elf_head.e_machine, tep);
	panic("The ISA counldn't be matched!");	  
  }
  Elf_Phdr *phdr = (Elf_Phdr *)malloc(sizeof(Elf_Phdr) * elf_head.e_phnum);
  ramdisk_read(phdr, elf_head.e_phoff, sizeof(Elf_Phdr) * elf_head.e_phnum);
  uintptr_t addr = elf_head.e_entry;
  for (int i = 0; i < elf_head.e_phnum; i++){
	 uint32_t type = phdr[i].p_type; 
     uintptr_t VirtAddr = phdr[i].p_vaddr;
	 size_t FileSiz = phdr[i].p_filesz , Memsiz = phdr[i].p_memsz;
	 size_t offset = phdr[i].p_offset;
	 uint32_t *fb = (uint32_t *)VirtAddr;
	 if (type != 1) continue;
	// printf("%08x %08x %08x %08x %08x %08x\n", fb, type, offset, VirtAddr, FileSiz, Memsiz); 
	 ramdisk_read(fb, offset, FileSiz);
	 memset(fb + FileSiz, 0, Memsiz - FileSiz);
  }
  return addr;
}

void naive_uload(PCB *pcb, const char *filename) {
  uintptr_t entry = loader(pcb, filename);
  Log("Jump to entry = %p", entry);
  ((void(*)())entry) ();
}

