#include <proc.h>
#include <elf.h>
#include <common.h>
#include <memory.h>

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

static inline void* pg_alloc2(int n) {
	assert(n % PGSIZE == 0);
	void *p = new_page(n / PGSIZE);
	memset(p, 0, n);
	return p;	
}

uintptr_t loader(PCB *pcb, const char *filename) {
	  assert(pcb != NULL);
      uint32_t head_addr;
      if (filename == NULL) head_addr = 0;
	  else {
	     int fd = fs_open(filename, 0, 0);
		 if (fd == -1) {
			printf("Can't find the file!\n'");
			return 0;	 
		}
		 head_addr = get_head(fd);
		 fs_close(fd);			  
	  }
	  Elf_Ehdr elf_head;
	  ramdisk_read(&elf_head, head_addr, sizeof(Elf_Ehdr));
	  if (elf_head.e_machine != EXPECT_TYPE) {
		uint32_t tep = EXPECT_TYPE;   
		printf("%08x %08x\n", elf_head.e_machine, tep);
		printf("The ISA counldn't be matched!\n");
		return 0;	  
	  }
	  uintptr_t addr = elf_head.e_entry;
	  Elf_Phdr *phdr = (Elf_Phdr *)malloc(sizeof(Elf_Phdr) * elf_head.e_phnum);
	  ramdisk_read(phdr, elf_head.e_phoff + head_addr, sizeof(Elf_Phdr) * elf_head.e_phnum);
	  for (int i = 0; i < elf_head.e_phnum; i++){
		 uintptr_t VirtAddr = phdr[i].p_vaddr;
		 size_t FileSiz = phdr[i].p_filesz , Memsiz = phdr[i].p_memsz;
		 size_t offset = phdr[i].p_offset;
         
		 uintptr_t bss_addr = VirtAddr + FileSiz;
		 uintptr_t final_addr = VirtAddr + Memsiz;

	//	 uint32_t *fb = (uint32_t *)VirtAddr;
	//	 ramdisk_read(fb, offset + head_addr, FileSiz);

		 uintptr_t *tep;
		 tep = (uintptr_t *)pg_alloc2(PGSIZE);
		 assert(((uintptr_t)tep & 0xfff) == 0);

         map(&(pcb->as), (void *)(VirtAddr & 0xfffff000), tep, 0);
		
		 int remain_space = FileSiz;	
		 uint32_t current_loc = VirtAddr & 0xfff;
		 uint32_t current_len = 0xfff - (current_loc) + 1;
		 uint32_t current_disk = offset + head_addr;

		 if (current_len > remain_space) current_len = remain_space;

		 ramdisk_read((char *)tep + current_loc,current_disk,current_len);
		 remain_space -= current_len;
		 current_disk += current_len;
		 VirtAddr = (VirtAddr & 0xfffff000) + 0x00001000;
		 
		 while(remain_space) {
			assert(remain_space > 0);	 
			assert((VirtAddr & 0xfff) == 0);

			tep = (uintptr_t *)pg_alloc2(PGSIZE);
			assert(((uintptr_t)tep & 0xfff) == 0);
			map(&(pcb->as), (void *)VirtAddr, tep, 0);	 

			current_len = 0x1000;
			if (current_len > remain_space) current_len = remain_space;

			ramdisk_read((char *)tep, current_disk, current_len);
			remain_space -= current_len;
			current_disk += current_len;
		    VirtAddr = (VirtAddr & 0xfffff000) + 0x00001000;
		 }
		 assert(((current_len ) & 0xfff) == (bss_addr & 0xfff));

		 // memset((uint8_t *)fb + FileSiz, 0, Memsiz - FileSiz);

		 remain_space = Memsiz - FileSiz;
		 current_loc = bss_addr & 0xfff;
		 current_len = 0xfff - (current_loc) + 1;
		 
		 printf("%x %x\n", remain_space, final_addr & 0xfff); 
		 if (current_len > remain_space) current_len = remain_space;

		 if ((bss_addr & 0xfff) == 0) {
			tep = (uintptr_t *)pg_alloc2(PGSIZE);
			assert(((uintptr_t)tep & 0xfff)== 0);
			map(&(pcb->as), (void *)bss_addr, tep, 0);	 
		  }
         // memset
		 remain_space -= current_len;
		 bss_addr += (bss_addr & 0xfffff000) + 0x00001000;

		 while(remain_space) {
			assert(remain_space > 0);	 
			assert((bss_addr & 0xfff) == 0);

			tep = (uintptr_t *)pg_alloc2(PGSIZE);
			assert(((uintptr_t)tep & 0xfff) == 0);
			map(&(pcb->as), (void *)bss_addr, tep, 0);
			
			current_len = 0x1000;
			if (current_len > remain_space) current_len = remain_space;
			// memset
			remain_space -= current_len;
			bss_addr = (bss_addr & 0xfffff000) + 0x00001000;	 
		 }
		 printf("%x %x\n", current_len, final_addr & 0xfff); 
         assert(((current_len ) & 0xfff) == (final_addr & 0xfff));

	  }
	  return addr;
}

void naive_uload(PCB *pcb, const char *filename) {
  uintptr_t entry = loader(pcb, filename);
  if (entry == 0)return;
  Log("Jump to entry = %p", entry);
  ((void(*)())entry) ();
}

