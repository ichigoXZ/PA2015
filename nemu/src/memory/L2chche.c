#include <stdlib.h>
#include <time.h>

#include <common.h>

   /* L2cache block存储空间的大小为64B
    L2cache存储空间的大小为4MB
    16-way set associative
    标志位包括valid bit和dirty bit
    替换算法采用随机方式
    write back
    write allocate*/

#define BLOCK_WIDTH 6
#define ROW_WIDTH 4
#define GROUP_WIDTH 12
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
}L2cache_addr;

typedef  struct {
	struct {
		uint32_t row	:ROW_WIDTH;
		uint32_t flag 	:FLAG_WIDTH;
		uint32_t valid	:1;
		uint32_t dirty 	:1;
	};
	uint8_t block[NR_BLOCK];
}L2cache_block;

L2cache_block L2cache[NR_GROUP][NR_ROW];	

void init_L2cache() {
	int i, j;
	for(i = 0; i < NR_GROUP; i ++) {
		for(j = 0; j < NR_ROW; j ++) {
			L2cache[i][j].valid = 0;
			L2cache[i][j].dirty = 0;
		}
	}
}


uint32_t L2cache_read(hwaddr_t addr,  size_t len) {
	int i,j;
	L2cache_addr temp;
	temp.addr = addr;
	uint32_t data;
	for (i=0;i<NR_ROW;i++) {
		if (L2cache[temp.group][i].row == temp.row && L2cache[temp.group][i].flag == temp.flag && L2cache[temp.group][i].valid == 1) {
			if (len + temp.block <= NR_BLOCK) {
				memcpy(&data, &L2cache[temp.group][i].block[temp.block], len);
				return data;
			}
			
		} 
	}
	for (i=0;i<NR_ROW;i++) {
		if (L2cache[temp.group][i].valid == 0) {
			L2cache[temp.group][i].row = temp.row;
			L2cache[temp.group][i].flag = temp.flag;
			L2cache[temp.group][i].valid = 1;
			L2cache[temp.group][i].dirty = 0;
			for (j=0; j<NR_BLOCK; j++) {
			L2cache[temp.group][i].block[j]=dram_read((addr & ~(NR_BLOCK-1))+j, 1);
			}
			return dram_read(addr, len);
		} 
	}
	srand(time(0));
	i = rand()%NR_ROW;
	if (L2cache[temp.group][i].dirty) {
		L2cache_addr daddr;
		daddr.addr = addr;
		daddr.block = 0;
		for (j=0; j<NR_BLOCK; j++) 
			dram_write((daddr.addr & ~(NR_BLOCK-1))+j, 1, L2cache[temp.group][i].block[j]);
	}
	L2cache[temp.group][i].row = temp.row;
	L2cache[temp.group][i].flag = temp.flag;
	L2cache[temp.group][i].valid = 1;
	L2cache[temp.group][i].dirty = 0;
	for (j=0; j<NR_BLOCK; j++) {
		L2cache[temp.group][i].block[j]=dram_read((addr & ~(NR_BLOCK-1))+j, 1);
		}
	return dram_read(addr, len);
}

void L2cache_write(hwaddr_t addr, size_t len, uint32_t data) {
	int i;
	L2cache_addr temp;
	temp.addr = addr;
	for (i=0;i<NR_ROW;i++)
		if (L2cache[temp.group][i].row == temp.row && L2cache[temp.group][i].flag == temp.flag && L2cache[temp.group][i].valid == 1) {
			memcpy(&L2cache[temp.group][i].block[temp.block], &data, len);
			L2cache[temp.group][i].dirty = 1;
			return;
		}
	dram_write(addr, len, data);
}