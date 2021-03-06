#include <isa.h>
#include <memory/paddr.h>
#include <memory/vaddr.h>
#include <device/map.h>
#include <stdlib.h>
#include <time.h>

static uint8_t pmem[PMEM_SIZE] PG_ALIGN = {};

void* guest_to_host(paddr_t addr) { return &pmem[addr]; }
paddr_t host_to_guest(void *addr) { return (void *)pmem - addr; }

IOMap* fetch_mmio_map(paddr_t addr);

void init_mem() {
#ifndef DIFF_TEST
  srand(time(0));
  uint32_t *p = (uint32_t *)pmem;
  int i;
   for (i = 0; i < PMEM_SIZE / sizeof(p[0]); i ++) {
    p[i] = rand();
  }
#endif
}

static inline bool in_pmem(paddr_t addr) {
  return (PMEM_BASE <= addr) && (addr <= PMEM_BASE + PMEM_SIZE - 1);
}

static inline word_t pmem_read(paddr_t addr, int len) {
  void *p = &pmem[addr - PMEM_BASE];
  switch (len) {
    case 1: return *(uint8_t  *)p;
    case 2: return *(uint16_t *)p;
    case 4: return *(uint32_t *)p;
    default: assert(0);
  }
}

static inline void pmem_write(paddr_t addr, word_t data, int len) {
  void *p = &pmem[addr - PMEM_BASE];
  switch (len) {
    case 1: *(uint8_t  *)p = data; return;
    case 2: *(uint16_t *)p = data; return;
    case 4: *(uint32_t *)p = data; return;
    default: assert(0);
  }
}

/* Memory accessing interfaces */

inline word_t paddr_read(paddr_t addr, int len) {
  if (in_pmem(addr)) return pmem_read(addr, len);
  else return map_read(addr, len, fetch_mmio_map(addr));
}

inline void paddr_write(paddr_t addr, word_t data, int len) {
  if (in_pmem(addr)) pmem_write(addr, data, len);
  else map_write(addr, data, len, fetch_mmio_map(addr));
}

word_t vaddr_mmu_read(vaddr_t addr, int len, int type) {
	 paddr_t pg_base = isa_mmu_translate(addr, type, len);
	 if (pg_base != MEM_RET_CROSS_PAGE) {
		paddr_t paddr = pg_base;
		//assert(paddr == addr);
		return paddr_read(paddr, len);
	 }
	  else {
		word_t ans = 0, tep = 0;
		for (int i = 0; i < len; i++) {
			pg_base = isa_mmu_translate(addr + i,type, 1);
			paddr_t paddr = pg_base;
			tep = paddr_read(paddr, 1);
			ans = ans + (tep << (8 * i));
		}
		return ans;
	}
	return 0;
}

void vaddr_mmu_write(vaddr_t addr, word_t data, int len){
	 paddr_t pg_base = isa_mmu_translate(addr, data, len);
	 if (pg_base != MEM_RET_CROSS_PAGE) {
		paddr_t paddr = pg_base;
		//assert(paddr == addr);
		paddr_write(paddr, data, len);
	 }
	  else {
		assert(0);
	}
}

int isa_vaddr_check(vaddr_t vaddr, int type, int len) {
	if ((cpu.cr0 & 0x80000000) == 0)return MEM_RET_OK;
	else return MEM_RET_NEED_TRANSLATE;	
}

#define def_vaddr_template(bytes) \
word_t concat(vaddr_ifetch, bytes) (vaddr_t addr) { \
  int ret = isa_vaddr_check(addr, MEM_TYPE_IFETCH, bytes); \
  if (ret == MEM_RET_OK) return paddr_read(addr, bytes); \
  else if (ret == MEM_RET_NEED_TRANSLATE) return vaddr_mmu_read(addr, bytes, MEM_TYPE_READ); \
  return 0; \
} \
word_t concat(vaddr_read, bytes) (vaddr_t addr) { \
  int ret = isa_vaddr_check(addr, MEM_TYPE_READ, bytes); \
  if (ret == MEM_RET_OK) return paddr_read(addr, bytes); \
  else if (ret == MEM_RET_NEED_TRANSLATE) return vaddr_mmu_read(addr, bytes, MEM_TYPE_READ); \
  return 0; \
} \
void concat(vaddr_write, bytes) (vaddr_t addr, word_t data) { \
  int ret = isa_vaddr_check(addr, MEM_TYPE_WRITE, bytes); \
  if (ret == MEM_RET_OK) paddr_write(addr, data, bytes); \
  else if (ret == MEM_RET_NEED_TRANSLATE) vaddr_mmu_write(addr, data, bytes); \
}


def_vaddr_template(1)
def_vaddr_template(2)
def_vaddr_template(4)
