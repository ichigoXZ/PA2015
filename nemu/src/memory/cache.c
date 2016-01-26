#include <stdlib.h>
#include <time.h>

#include <common.h>

#define BLOCK_WIDTH 6
#define GROUP_WIDTH 3
#define ROW_WIDTH 7
#define FLAG_WIDTH (27-BLOCK_WIDTH-GROUP_WIDTH-ROW_WIDTH)

#define NR_BLOCK (1 << BLOCK_WIDTH)
#define NR_GROUP (1 << GROUP_WIDTH) 
#define NR_ROW (1 << ROW_WIDTH)

uint32_t dram_read(hwaddr_t addr, size_t len);
void dram_write(hwaddr_t addr, size_t len, uint32_t data);

typedef union {
	struct {
		uint32_t block	:BLOCK_WIDTH;
		uint32_t group  	:GROUP_WIDTH;
		uint32_t row  	:ROW_WIDTH;
		uint32_t flag 	:FLAG_WIDTH;
	};
	uint32_t addr;
}cache_addr;

typedef  struct {
	struct {
		uint32_t row	:ROW_WIDTH;
		uint32_t flag 	:FLAG_WIDTH;
		uint32_t valid	:1;
	};
	uint8_t block[NR_BLOCK];
}cache_block;

cache_block cache[NR_GROUP][NR_ROW];	

void init_cache() {
	int i, j;
	for(i = 0; i < NR_GROUP; i ++) {
		for(j = 0; j < NR_ROW; j ++) {
			cache[i][j].valid = 0;
		}
	}
}


uint32_t cache_read(hwaddr_t addr,  size_t len) {
	int i,j;
	cache_addr temp;
	temp.addr = addr;
	uint32_t data;
	for (i=0;i<NR_ROW;i++) {
		if (cache[temp.group][i].row == temp.row && cache[temp.group][i].flag == temp.flag && cache[temp.group][i].valid == 1) {
			if (len + temp.block <= NR_BLOCK) {
				memcpy(&data, &cache[temp.group][i].block[temp.block], len);
				return data;
			}
			
		} 
	}
	for (i=0;i<NR_ROW;i++) {
		if (cache[temp.group][i].valid == 0) {
			cache[temp.group][i].row = temp.row;
			cache[temp.group][i].flag = temp.flag;
			cache[temp.group][i].valid = 1;
			//update_cache(addr, cache[temp.group][i].block, NR_BLOCK);
			for (j=0; j<NR_BLOCK; j++) {
			cache[temp.group][i].block[j]=dram_read((addr & ~(NR_BLOCK-1))+j, 1);
		//	cache[temp.group][i].block[j]=L2chche_read((addr & ~(NR_BLOCK-1))+j, 1);
			}
			return dram_read(addr, len);
		} 
	}
	srand(time(0));
	i = rand()%NR_ROW;
	cache[temp.group][i].row = temp.row;
	cache[temp.group][i].flag = temp.flag;
	cache[temp.group][i].valid = 1;
	for (j=0; j<NR_BLOCK; j++) {
		cache[temp.group][i].block[j]=dram_read((addr & ~(NR_BLOCK-1))+j, 1);
	//	cache[temp.group][i].block[j]=L2chche_read((addr & ~(NR_BLOCK-1))+j, 1);
		}
	return dram_read(addr, len);
}

/*void pretend_cache_read(hwaddr_t addr, size_t len) {
	int i;
	cache_addr temp;
	temp.addr = addr;
	uint32_t data;
	for (i=0;i<GROUP_WIDTH;i++) {
		if (cache[temp.row][i].q == temp.group && cache[temp.row][i].f == temp.flag && cache[temp.row][i].valid == 1) {
			if (len + temp.block <= NR_BLOCK) {
				memcpy(&data, &cache[temp.row][i].block[temp.block], len);
				printf("content = %x, f = %d, q = %d\n", data, temp.flag , temp.group);
				return ;
			}
			
		} 
	}
	printf("can't find in the cache");
	return ;
}
*/
void cache_write(hwaddr_t addr, size_t len, uint32_t data) {
	int i;
	cache_addr temp;
	temp.addr = addr;
	for (i=0;i<NR_ROW;i++)
		if (cache[temp.group][i].row == temp.row && cache[temp.group][i].flag == temp.flag && cache[temp.group][i].valid == 1) 
			memcpy(&cache[temp.group][i].block[temp.block], &data, len);
	dram_write(addr, len, data);
}


