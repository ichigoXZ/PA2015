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
void update_cache(hwaddr_t addr, void *data, size_t len);

void init_cache() {
	memset(cache, 0, sizeof(cache));
}

uint32_t cache_read(hwaddr_t addr, size_t len) {
	cache_addr caddr;
	caddr.addr = addr;
	uint32_t temp,i;
	for(i=0; i<NR_ROW; i++){
		if(cache[caddr.group][i].rem == caddr.rem && cache[caddr.group][i].valid){
			/*read a block*/
			if(len + caddr.block <= NR_BLOCK){
				memcpy(&temp, &cache[caddr.group][i].data[caddr.block], len);
				printf("cache_read branch1:\n");
				printf("content = %x, group = %d, rem = %d\n", temp, caddr.group , caddr.rem);
				return temp;
			}
		}
	}
	for(i = 0; i < NR_ROW; i++) {
		if(cache[caddr.group][i].valid == 0) {
			cache[caddr.group][i].rem = caddr.rem;
			cache[caddr.group][i].valid = 1;
			update_cache(addr,cache[caddr.group][i].data,NR_BLOCK);
			printf("cache_read branch2:\n");
			printf("content = %x, group = %d, rem = %d\n", temp, caddr.group , caddr.rem);
			return dram_read(addr, len);
		}
	}
	srand(time(0));
	i = rand()%NR_BLOCK;
	cache[caddr.group][i].rem = caddr.rem;
	cache[caddr.group][i].valid = 1;
	update_cache(addr,cache[caddr.group][i].data,NR_BLOCK);
	printf("cache_read branch3:\n");
	printf("content = %x, group = %d, rem = %d\n", temp, caddr.group , caddr.rem);
	return dram_read(addr, len);
}

void cache_write(hwaddr_t addr, size_t len, uint32_t data) {
	cache_addr caddr;
	caddr.addr = addr;
	uint32_t i;
	for(i = 0; i < NR_ROW; i++) {
		if(cache[caddr.group][i].valid && cache[caddr.group][i].rem == caddr.rem) {
			memcpy(&cache[caddr.group][i].data[caddr.block], &data, len);
			dram_write(addr, len, data);
			return;
		}
	}
}

