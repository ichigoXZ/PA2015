#include <stdlib.h>
#include <time.h>
#include "common.h"
#include "burst.h"
#include "misc.h"

//L1cache 存储空间64KB, cache行64B, 8-GROUP set associate, 标志位（valid）, 随机替换算法, write through, no write allocate
#define BLOCK_WIDTH 6
#define GROUP_WIDTH 3
#define ROW_WIDTH 7
#define REM_WIDTH (27 - GROUP_WIDTH - ROW_WIDTH - BLOCK_WIDTH)

typedef union{
	struct 
	{
		uint32_t block 	: BLOCK_WIDTH;
		uint32_t group  	: GROUP_WIDTH;
		uint32_t row 	: ROW_WIDTH;
		uint32_t rem 	: REM_WIDTH;
	};
	uint32_t addr;
} cache_addr;

#define NR_GROUP (1 << GROUP_WIDTH)
#define NR_ROW (1 << ROW_WIDTH)
#define NR_BLOCK (1 << BLOCK_WIDTH)
#define NR_REM (1 << REM_WIDTH)

#define HW_MEM_SIZE (1 << (GROUP_WIDTH + ROW_WIDTH + BLOCK_WIDTH + REM_WIDTH))

typedef struct 
{
	struct {
		uint16_t rem 	: REM_WIDTH;
		uint16_t valid	: 1;
	};
	uint8_t data[NR_BLOCK];
}cache_block;

cache_block cache[NR_GROUP][NR_ROW];

uint32_t dram_read(hwaddr_t addr, size_t len);
void dram_write(hwaddr_t addr, size_t len, uint32_t data);

void init_cache() {
	memset(cache, 0, sizeof(cache));
}

static void ccr_read(hwaddr_t addr ,void * data) {
	Assert(addr < HW_MEM_SIZE, "physical address %x is outside of the physical memory!", addr);

	cache_addr temp;
	temp.addr = addr & ~BURST_MASK;
	uint32_t block = temp.block;
	uint32_t group = temp.group;
	uint32_t rem = temp.rem;

	uint32_t i,k;
	for(i=0; i<NR_ROW; i++){
		if(cache[group][i].rem == rem && cache[group][i].valid){
			/*read a block*/
			memcpy(data, &cache[group][i].data[block], BURST_LEN);
			//printf("cache_read branch1:\n");
			//printf("i = %d,group = %d, rem = %d\n", i,group , rem);
			return;
		}
	}
	for(i = 0; i < NR_ROW; i++) {
		if(cache[group][i].valid == 0) {
			cache[group][i].rem = rem;
			cache[group][i].valid = 1;
			for(k = 0;k<NR_BLOCK;k++) {
				cache[group][i].data[k]=dram_read((addr & ~(NR_BLOCK-1))+k, 1);	
			}
			memcpy(data, &cache[group][i].data[block], BURST_LEN);
			//printf("cache_read branch2:\n");
			//printf("i = %d,group = %d, rem = %d\n", i, group , rem);
			return ;
		}
	}
	srand(time(0));
	i = rand()%NR_BLOCK;
	cache[group][i].rem = rem;
	cache[group][i].valid = 1;
	for(k = 0;k<NR_BLOCK;k++) {
		cache[group][i].data[k]=dram_read((addr & ~(NR_BLOCK-1))+i, 1);				
	}
	memcpy(data, &cache[group][i].data[block], BURST_LEN);
}

static void ccr_write(hwaddr_t addr , void *data,uint8_t *mask) {
	Assert(addr < HW_MEM_SIZE, "physical address %x is outside of the physical memory!", addr);
	cache_addr temp;
	temp.addr = addr & ~BURST_MASK;

	uint32_t block = temp.block;
	uint32_t group = temp.group;
	uint32_t rem = temp.rem;

	uint32_t i;
	for(i = 0; i < NR_ROW; i++) {
		if(cache[group][i].valid && cache[group][i].rem == rem) {
			memcpy_with_mask(&cache[group][i].data[block],data,BURST_LEN, mask);
			return;
		}
	} 
}

uint32_t cache_read(hwaddr_t addr, size_t len) {
	uint32_t offset = addr & BURST_MASK;
	uint8_t temp[2 * BURST_LEN];

	ccr_read(addr,temp);;

	if(offset + len > BURST_LEN) {
		/* data cross the burst boundary */	
		ccr_read(addr + BURST_LEN, temp + BURST_LEN);
	}
	return unalign_rw(temp + offset, 4);
}

void cache_write(hwaddr_t addr, size_t len, uint32_t data) {
	uint32_t offset = addr & BURST_MASK;
	uint8_t temp[2 * BURST_LEN];
	uint8_t mask[2 * BURST_LEN];
	memset(mask, 0, 2 * BURST_LEN);

	*(uint32_t *)(temp + offset) = data;
	memset(mask + offset, 1, len);

	ccr_write(addr,temp,mask);

	if(offset + len > BURST_LEN) {
		/* data cross the brust boundary */
		ccr_write(addr + BURST_LEN, temp + BURST_LEN, mask + BURST_LEN);
	}
}

