#include <proc.h>
#include <elf.h>
#include <common.h>


#if defined(__ISA_AM_NATIVE__)
#define EXPECT_TYPE EM_X86_64 
#elif defined(__ISA_X86__)
#define EXPECT_TYPE EM_386
#else
#error Unsupported_ISA
#endif


#ifdef __LP64__
# define Elf_Ehdr Elf64_Ehdr
# define Elf_Phdr Elf64_Phdr
#else
# define Elf_Ehdr Elf32_Ehdr
# define Elf_Phdr Elf32_Phdr
#endif

static uintptr_t loader(PCB *pcb, const char *filename) {
      int head_addr;
      if (filename == NULL) head_addr = 0;
	  else {
	     int fd = fs_open(filename, 0, 0);
		 assert(fd != -1);
		 head_addr = get_head(fd);
		 fs_close(fd);			  
	  }
	  Elf_Ehdr elf_head;
	  ramdisk_read(&elf_head, head_addr, sizeof(Elf_Ehdr));
	   if (elf_head.e_machine != EXPECT_TYPE) {
		uint32_t tep = EXPECT_TYPE;   
		printf("%08x %08x\n", elf_head.e_machine, tep);
		panic("The ISA counldn't be matched!");	  
	  }
	  uintptr_t addr = elf_head.e_entry;
	  printf("%08x\n", addr);
	  Elf_Phdr *phdr = (Elf_Phdr *)malloc(sizeof(Elf_Phdr) * elf_head.e_phnum);
	  ramdisk_read(phdr, elf_head.e_phoff + head_addr, sizeof(Elf_Phdr) * elf_head.e_phnum);
	  for (int i = 0; i < elf_head.e_phnum; i++){
		 uint32_t type = phdr[i].p_type; 
		 uintptr_t VirtAddr = phdr[i].p_vaddr;
		 size_t FileSiz = phdr[i].p_filesz , Memsiz = phdr[i].p_memsz;
		 size_t offset = phdr[i].p_offset;
		 uint32_t *fb = (uint32_t *)VirtAddr;
		 if (type != 1) continue;
		// printf("%08x %08x %08x %08x %08x %08x\n", fb, type, offset, VirtAddr, FileSiz, Memsiz); 
		 ramdisk_read(fb, offset + head_addr, FileSiz);
		 memset(fb + FileSiz, 0, Memsiz - FileSiz);
	  }
	  return addr;
}
void naive_uload(PCB *pcb, const char *filename) {
  uintptr_t entry = loader(pcb, filename);
  Log("Jump to entry = %p", entry);
  ((void(*)())entry) ();
}

